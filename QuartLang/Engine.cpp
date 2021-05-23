#include "Engine.h"


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




Engine::Engine(Program* program, Logger *logger):
    m_program(program),
    m_logger(logger)
{
}

variableDeclaration* getVariable(const std::map<std::string, std::vector<variableDeclaration*>> &variables, const std::string &varName, size_t currentScopedBlock) {
    if (variables.find(varName) != variables.end()) {
        for (size_t i = 0; i < variables.at(varName).size(); i++)
        {
            auto currentVar = variables.at(varName)[i];
            if (currentScopedBlock >= currentVar->getScopeNesting()) {
                return currentVar;
            }
        }
    }
    return nullptr;
}




#define START_FROM_ZERO 0
#define START_FROM_LAST_STATEMENT -1


runType Engine::m_run(const Subprogram* currentProgram, int startPoint, DataStructure *p_result, const std::map<std::string, DataStructure*>& p_variables)
{
    std::vector<programContent> currentContents = currentProgram->getContent();

    std::map<std::string, DataStructure*> variables = p_variables;
    std::vector<DataStructure*> newVariables;


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
    

    auto opToDat = [&](const operand& op) {
        DataStructure retval;
        switch (op.type)
        {
        case operandType::literal:
            retval = op.literalData;
            break;
        case operandType::variable:
        {
            retval = *variables[op.varName];
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

    auto clearStack = [&]() {
        
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
            case statementType::referOperationSttt:
            {
                const referOperation* referOp = currentProgram->getReferOperation(currentContentsOrderedId);

                auto place = opToDat(referOp->getWhere());
                
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
                auto appOpContent = opToDat(appOp->getContent());
                switch (appType) {
                case appendType::map_to:
                    
                    variables[appOp->getVarName()]->addNamedSubobject(appOpContent, opToDat(appOp->getPlace()).getString());

                    break;
                case appendType::push_back:
                    variables[appOp->getVarName()]->pushBackSubobject(appOpContent);
                    break;
                case appendType::push_front:
                    
                    break;
                }
            }
                break;
            case statementType::flipOperationSttt:
            {
                const flipOperation* flipOp = currentProgram->getFlipOperation(currentContentsOrderedId);
                
                const operand &op = flipOp->getOperand();

                *result = DataStructure(!opToDat(op).getBoolData());

            }   
                

                break;
            case statementType::evaluateOperationSttt:
            {
                const evaluateOperation* evalOp = currentProgram->getEvaluateOperation(currentContentsOrderedId);
                auto lhs = opToDat(evalOp->getLhs());
                auto rhs = opToDat(evalOp->getRhs());
                
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
                case finallyType::end:
                    
                    break;
                case finallyType::repeat:
                    if (opToDat(fSttt->getOptionalOperand()).getBoolData()) {
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
                auto lhs = opToDat(arithmeticOp->getLhs());
                auto rhs = opToDat(arithmeticOp->getRhs());

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
                *variables[setOp->getSettedName()] = opToDat(setOp->getToSet());
            }
            break;
            case statementType::functionCallSttt:
            {

                auto fCall = currentProgram->getFunctionCall(currentContentsOrderedId);
                auto args = fCall->getArgs();
                std::map<std::string, DataStructure> realArgs;
                for (auto x : args) {
                    realArgs[x.first] = opToDat(x.second);
                }

                builtinFunction* builtin = m_program->getIncludedBuiltin(fCall->getFunctionCalledTag());
                m_logger->log("Engine", fCall->getFunctionCalledTag());
                //a builtin matching the function was found
                if (builtin != nullptr) {

                    *result = builtin->call(realArgs);
                }
                //a builtin was not found
                else {

                }
            }
            break;
            case statementType::variableDeclarationSttt:
            {
                auto var = currentProgram->getVariable(currentContents[i].orderedID);
                if (variables.find(var->getTag()) != variables.end()) {
                    //THROW EXCEPTION HERE(VARIABLE REDECLARATION)
                }
                auto newVar = new DataStructure(var->getData());
                variables[var->getTag()] = newVar;
                newVariables.push_back(newVar);
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
                if (opToDat(condBlock->getCondition()).getBoolData()) {
                    m_run(condBlock,START_FROM_ZERO,result,variables);
                }
            }
                break;
            case subprogramType::functionBlock:
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


runType Engine::run()
{
    
    return m_run(m_program);
}
