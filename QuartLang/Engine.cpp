#include "Engine.h"
#include "Ishleng.h"
#include <mutex>
#include <thread>



std::map<engineErrorType, std::string> engineErrorTypeNames = {
    {engineErrorType::tagNotFound, "tag not found"}

};

#define DO_OPERATION_ISHLENG(lhs, rhs, op)\
            if (lhs.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) { \
                    if (rhs.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) { \
                        *result = DataStructure(lhs.getFloatData() op rhs.getFloatData());\
                    }\
                    else if (rhs.getTypeOrPrimitiveTag() == INT_TYPE_STR) { \
                        *result = DataStructure(lhs.getFloatData() op rhs.getIntData());\
                    }\
            }\
            else if (lhs.getTypeOrPrimitiveTag() == INT_TYPE_STR) {\
                    if (rhs.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) {\
                        *result = DataStructure(lhs.getIntData() op rhs.getFloatData());\
                    }\
                    else if (rhs.getTypeOrPrimitiveTag() == INT_TYPE_STR) {\
                        *result = DataStructure(lhs.getIntData() op rhs.getIntData());\
                    }\
             }\




Engine::Engine(const Program* program, const DictionaryLexer* dict,Logger *logger):
    m_program(program),
    m_logger(logger),
    m_dict(dict)
{
}


std::map<size_t, std::pair<DataStructure*, std::thread*> > runningThreads;

std::mutex runningThreadsMutex;

size_t biggestID = 0;

size_t getNewID() {
    std::lock_guard<std::mutex> lck(runningThreadsMutex);
    return biggestID++;
}


void addThread(size_t ID,const std::pair<DataStructure*, std::thread*>& thread) {
    runningThreadsMutex.lock();
    runningThreads[ID] = thread;
    runningThreadsMutex.unlock();
}

std::pair < DataStructure*, std::thread*>& getRunningThread(size_t id) {
    std::lock_guard<std::mutex> lck(runningThreadsMutex);
    return runningThreads[id];
}


#define START_FROM_ZERO 0
#define START_FROM_LAST_STATEMENT -1


runType m_run(Logger* logger, const DictionaryLexer* dict, const Program* mainProgram,
    const Subprogram* currentProgram,
    const std::vector<std::string> &p_stackTrace,
    int startPoint = 0,
    DataStructure* p_result = nullptr, 
    const std::map<std::string, DataStructure*>& p_variables = std::map<std::string, DataStructure*>(),
    const std::map<std::string, const functionBlock*>&p_functions = std::map<std::string, const functionBlock*>())
{
    std::vector<programContent> currentContents = currentProgram->getContent();

    std::map<std::string, DataStructure*> variables = p_variables;
    std::vector<DataStructure*> newVariables;

    std::map<std::string, const functionBlock*> functions = p_functions;


    if (startPoint == START_FROM_LAST_STATEMENT) {
        startPoint = currentContents.size() - 1;
    }


    size_t currentScopeNested = 0;
    DataStructure *result;
    if (p_result == nullptr) {
        result = new DataStructure();
    }
    else {
        result = p_result;
    }
    

    auto opToDat = [&](const operand& op, size_t currentContentID) {
        DataStructure retval;
        switch (op.type)
        {
        case operandType::literal:
            retval = op.literalData;
            break;
        case operandType::tag:
        {
            if (variables.find(op.tagStr) != variables.end()) {
                retval = *variables[op.tagStr];
                
            }
            else if (functions.find(op.tagStr) != functions.end()) {
                retval = DataStructure(functions[op.tagStr]);
            }
            else {
                auto* candidateBuiltin = mainProgram->getIncludedBuiltin(op.tagStr);
                if (candidateBuiltin != nullptr) {
                    retval = DataStructure(candidateBuiltin);
                }
                else {
                    std::stringstream ss;

                    ss << "the given tag was not declared: " << op.tagStr;

                    throw engineError(engineErrorType::tagNotFound, currentContents[currentContentID], ss.str(),p_stackTrace);
                }
            }

        }
        break;
        case operandType::result:
            retval = *result;
            break;
        default:
            break;
        }
        return retval;
    };

    auto getFunction = [&](const std::string& functionName) {
        if (functions.find(functionName) != functions.end()) {
            return functions[functionName];
        }
        else {
            return variables[functionName]->getFunction();
        }

    };
    
    auto getBuiltin = [&](const std::string& functionName) {
        auto candidateBuiltin = mainProgram->getIncludedBuiltin(functionName);
        if (candidateBuiltin == nullptr && variables.find(functionName) != variables.end()) {
            candidateBuiltin = variables[functionName]->getBuiltinFunction();
        }
        return candidateBuiltin;
    };

    auto clearStack = [&]() {
        variables = p_variables;
        for (auto x : newVariables)
        {

            delete x;
            
        }
        newVariables.clear();
    };

    for (int i = startPoint; i < currentContents.size(); i++)
    {
        size_t currentContentsOrderedId = currentContents[i].orderedID;
        if (currentContents[i].isStatement) {
            switch (currentContents[i].type)
            {
            case statementType::importSttt:
            {
                auto importOp = currentProgram->getImportOperation(currentContents[i].orderedID);


                Ishleng nestedIshleng(logger,dict, opToDat(importOp->getCode(),i).getString(),false);

                nestedIshleng.lex();
                nestedIshleng.validate();
                
                auto importedFuncs = Parser::getFunctionsOnly(nestedIshleng.getLexer(),logger,currentProgram);


                for (auto x : importedFuncs) {
                    functions[x->getTag()] = x;
                }
                
            }
                break;
            case statementType::finishSttt:
            {
                const finishOperation* finishOp = currentProgram->getFinishOperation(currentContentsOrderedId);
                
                auto runningThread = getRunningThread(opToDat(finishOp->getThreadID(),i).getIntData());
                runningThread.second->join();
                *result = *runningThread.first;

            }
                break;
            case statementType::referOperationSttt:
            {
                const referOperation* referOp = currentProgram->getReferOperation(currentContentsOrderedId);

                auto place = opToDat(referOp->getWhere(),i);
                
                auto varFrom = variables[referOp->getVarName()];


                if (place.getTypeOrPrimitiveTag() == INT_TYPE_STR) {
                    variables[referOp->getReferantName()] = &varFrom->getSubobject(place.getIntData());
                }else if (place.getTypeOrPrimitiveTag() == STRING_TYPE_STR) {
                    variables[referOp->getReferantName()] = &varFrom->getNamedSubobject(place.getString());
                }
            }
                break;
            case statementType::appendOperationSttt:
            {
                const appendOperation* appOp = currentProgram->getAppendOperation(currentContentsOrderedId);
                appendType appType = appOp->getAppendType();
                auto appOpContent = opToDat(appOp->getContent(),i);
                switch (appType) {
                case appendType::map_to:
                    
                    variables[appOp->getVarName()]->addNamedSubobject(appOpContent, opToDat(appOp->getPlace(),i).getString());
                    break;
                case appendType::push_back:
                    variables[appOp->getVarName()]->pushBackSubobject(appOpContent);
                    break;
                //IMPLEMENT THIS
                case appendType::push_front:
                    
                    break;
                }
            }
                break;
            case statementType::flipOperationSttt:
            {
                const flipOperation* flipOp = currentProgram->getFlipOperation(currentContentsOrderedId);
                
                auto op = opToDat(flipOp->getOperand(),i);


                if (op.getTypeOrPrimitiveTag() == BOOL_TYPE_STR) {
                    *result = DataStructure(!op.getBoolData());
                }
                else if (op.getTypeOrPrimitiveTag() == INT_TYPE_STR) {
                    *result = DataStructure(-op.getIntData());
                }
                else if (op.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) {
                    *result = DataStructure(-op.getFloatData());
                }




            }   
                

                break;
            case statementType::evaluateOperationSttt:
            {
                const evaluateOperation* evalOp = currentProgram->getEvaluateOperation(currentContentsOrderedId);
                auto lhs = opToDat(evalOp->getLhs(),i);
                auto rhs = opToDat(evalOp->getRhs(),i);
                
                switch (evalOp->getEvalType())
                {
                case evalType::equal:
                    DO_OPERATION_ISHLENG(lhs, rhs, ==);
                    break;
                case evalType::lessThan:
                    DO_OPERATION_ISHLENG(lhs, rhs, < );
                    break;
                case evalType::moreThan:
                    DO_OPERATION_ISHLENG(lhs, rhs, > );
                    break;
                case evalType::orEval:
                    DO_OPERATION_ISHLENG(lhs, rhs, || );
                    break;
                case evalType::andEval:
                    DO_OPERATION_ISHLENG(lhs, rhs, && );
                    break;
                case evalType::lessThanOrEqual:
                    DO_OPERATION_ISHLENG(lhs, rhs, <= );
                    break;
                case evalType::moreThanOrEqual:
                    DO_OPERATION_ISHLENG(lhs, rhs, >= );
                    break;
                case evalType::moreThanOrLessThan:
                    DO_OPERATION_ISHLENG(lhs, rhs, != );
                    break;
                default:
                    break;
                }
            }
                break;

            case statementType::finallySttt:
            {
                const finallySttt* fSttt = currentProgram->getFinallySttt(currentContentsOrderedId);
                switch (fSttt->getFinallyType())
                {
                case finallyType::give:
                    *result = opToDat(fSttt->getOptionalOperand(),i);
                    break;
                case finallyType::end:
                    
                    break;
                case finallyType::repeat:
                    if (opToDat(fSttt->getOptionalOperand(),i).getBoolData()) {
                        i = -1;
                        clearStack();
                        continue;
                    }
                    break;
                default:
                    break;
                }
            }
                break;
            case statementType::arithmeticOperationSttt:
            {
                const arithmeticOperation* arithmeticOp = currentProgram->getArithmeticOperation(currentContentsOrderedId);
                auto lhs = opToDat(arithmeticOp->getLhs(),i);
                auto rhs = opToDat(arithmeticOp->getRhs(),i);

                switch (arithmeticOp->getOperationType())
                {
                case arithmeticOperationType::add:
                    DO_OPERATION_ISHLENG(lhs, rhs, +);
                    break;
                case arithmeticOperationType::subtract:
                    DO_OPERATION_ISHLENG(lhs, rhs, -);
                    break;
                case arithmeticOperationType::multiply:
                    DO_OPERATION_ISHLENG(lhs, rhs, *);
                    break;
                case arithmeticOperationType::divide:
                    DO_OPERATION_ISHLENG(lhs, rhs, / );
                    break;
                default:
                    break;
                }
            }
            break;
            case statementType::setOperationSttt:
            {
                const setOperation* setOp = currentProgram->getSetOperation(currentContentsOrderedId);
                *variables[setOp->getSettedName()] = opToDat(setOp->getToSet(),i);
            }
            break;
            case statementType::functionCallSttt:
            {

                auto fCall = currentProgram->getFunctionCall(currentContentsOrderedId);
                auto args = fCall->getArgs();
                std::map<std::string, DataStructure*> realArgs;
                for (auto x : args) {
                    newVariables.push_back(new DataStructure(opToDat(x.second,i)));
                    realArgs[x.first] = newVariables.back();
                }

                auto stackTracePlusThisFunction = p_stackTrace;
                stackTracePlusThisFunction.push_back(fCall->getFunctionCalledTag());

                builtinFunction* builtin = getBuiltin(fCall->getFunctionCalledTag());
                logger->log("Engine", fCall->getFunctionCalledTag());
                //a builtin matching the function was found
                if(!fCall->getIsMultithreaded()){
                    if (builtin != nullptr) {

                        builtin->call(realArgs, result);
                    }
                    //a builtin was not found
                    else {
                        m_run(logger,dict,mainProgram,getFunction(fCall->getFunctionCalledTag()), stackTracePlusThisFunction,START_FROM_ZERO,result,realArgs,functions);
                    }
                }
                //multithreaded function call.
                else {
                    auto threadInstanceID = getNewID();
                    if (builtin != nullptr) {
                        DataStructure* mtRes = new DataStructure();
                        addThread(threadInstanceID, std::pair<DataStructure*, std::thread*>(mtRes, new std::thread(&builtinFunction::call, builtin, realArgs, mtRes)));
                        
                    }
                    //a builtin was not found
                    else {
                        DataStructure* mtRes = new DataStructure();
                        addThread(threadInstanceID, std::pair<DataStructure*, std::thread*>(mtRes, new std::thread(m_run, logger, dict, mainProgram, getFunction(fCall->getFunctionCalledTag()), stackTracePlusThisFunction,START_FROM_ZERO, mtRes, realArgs, functions)));
                    }
                    *result = DataStructure((int)threadInstanceID);
                }
            }
            break;
            case statementType::variableDeclarationSttt:
            {
                auto var = currentProgram->getVariable(currentContents[i].orderedID);
                if (variables.find(var->getTag()) != variables.end()) {
                    //we allow variable redeclaration here to allow default values for functions
                }
                else {
                    auto newVar = new DataStructure(var->getData());
                    variables[var->getTag()] = newVar;
                    newVariables.push_back(newVar);
                }

            }
            break;
            default:
                break;
            }
        }
        //all subprograms
        else {
            const Subprogram* candidateNext = currentProgram->getSubprogram(currentContents[i].orderedID);
            switch (candidateNext->getType())
            {
            case subprogramType::conditionalBlock:
            {
                const conditionalBlock* condBlock = dynamic_cast<const conditionalBlock*>(candidateNext);
                if (opToDat(condBlock->getCondition(),i).getBoolData()) {
                    m_run(logger, dict,mainProgram,condBlock,p_stackTrace,START_FROM_ZERO,result,variables,functions);
                }
            }
                break;
            case subprogramType::functionBlock:
            {
                const functionBlock* fBlock = dynamic_cast<const functionBlock*>(candidateNext);
                functions[fBlock->getTag()] = fBlock;

            }
                break;
            default:
                break;
            }
        }
    }

    //we cleanup
    clearStack();

    //if we didnt pass any result to this function it created a new result and must be disposed of.
    if (p_result == nullptr) {
        delete result;
    }
    return runType::success;
}


void Engine::cleanUpThreads()
{
    for (auto x : runningThreads) {
        delete x.second.first;
        delete x.second.second;
    }
}

runType Engine::run()
{
    return m_run(m_logger, m_dict, m_program, m_program, {"start"});
}

engineError::engineError(engineErrorType errorType, const programContent& errorOrigin, const std::string& message, const std::vector<std::string>& stackTrace):
    m_errorType(errorType),
    m_errorOrigin(errorOrigin),
    m_message(message),
    m_stackTrace(stackTrace)
{

    std::stringstream stackTraceMessage;

    for (auto x : m_stackTrace)
    {
        stackTraceMessage << "->" << x;
    }

    std::stringstream ss;
    ss << "ENGINE ERROR!: \n\t"<<"|error type: " << engineErrorTypeNames[m_errorType] << ", " << m_errorOrigin.getString() <<", stack trace: "<< stackTraceMessage.str() <<", message: (" << m_message << ")|";
    m_errorMessage = ss.str();
}

const char* engineError::what() const
{
    return m_errorMessage.c_str();
}
