#include "Engine.h"


#define DO_ARITHEMTIC_OPERATION(lhs, rhs, op)\
            if (lhs.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) { \
                    if (rhs.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) { \
                        result = DataStructure(lhs.getFloatData() op rhs.getFloatData());\
                    }\
                    else if (rhs.getTypeOrPrimitiveTag() == INT_TYPE_STR) { \
                        result = DataStructure(lhs.getFloatData() op rhs.getIntData());\
                    }\
            }\
            else if (lhs.getTypeOrPrimitiveTag() == INT_TYPE_STR) {\
                    if (rhs.getTypeOrPrimitiveTag() == FLOAT_TYPE_STR) {\
                        result = DataStructure(lhs.getIntData() op rhs.getFloatData());\
                    }\
                    else if (rhs.getTypeOrPrimitiveTag() == INT_TYPE_STR) {\
                        result = DataStructure(lhs.getIntData() op rhs.getIntData());\
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

runType Engine::run()
{
    std::vector<programContent> currentContents = m_program->getContent();
    Subprogram* currentProgram = m_program;
    std::map<std::string, std::vector<variableDeclaration*>> variables;
    
    size_t currentScopeNested = 0;
    DataStructure result;
    
    auto opToDat = [&](const operand &op){
        DataStructure retval;
        switch (op.type)
        {
        case operandType::literal:
            retval = op.literalData;
            break;
        case operandType::variable:
        {
            variableDeclaration* tempVar = getVariable(variables, op.varName, currentScopeNested);
            retval = tempVar->getData();
        }
            break;
        case operandType::result:
            retval = result;
            break;
        default:
            break;
        }
        return retval;
    };
    
    for (size_t i = 0; i < currentContents.size(); i++)
    {
        size_t currentContentsOrderedId = currentContents[i].orderedID;
        if (currentContents[i].isStatement) {
            switch (currentContents[i].type)
            {
            case statementType::arithmeticOperationSttt:
            {
                const arithmeticOperation* arithmeticOp = currentProgram->getArithmeticOperation(currentContentsOrderedId);
                auto lhs = opToDat(arithmeticOp->getLhs());
                auto rhs = opToDat(arithmeticOp->getRhs());

                switch (arithmeticOp->getOperationType())
                {
                case arithmeticOperationType::add:
                    DO_ARITHEMTIC_OPERATION(lhs, rhs, +);
                    break;
                case arithmeticOperationType::subtract:
                    DO_ARITHEMTIC_OPERATION(lhs, rhs, -);
                    break;
                case arithmeticOperationType::multiply:
                    DO_ARITHEMTIC_OPERATION(lhs, rhs, *);
                    break;
                case arithmeticOperationType::divide:
                    DO_ARITHEMTIC_OPERATION(lhs, rhs, /);
                    break;
                default:
                    break;
                }
            }
                break;
            case statementType::setOperationSttt:
            {
                 const setOperation *setOp = currentProgram->getSetOperation(currentContentsOrderedId);
                 getVariable(variables, setOp->getSettedName(), currentScopeNested)->setData(opToDat(setOp->getToSet()));
                 
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

                    result = builtin->call(realArgs);
                }
                //a builtin was not found
                else {
                    
                }
                
                break;
            }
            case statementType::variableDeclarationSttt:
            {
                auto var = currentProgram->getVariable(currentContents[i].orderedID);
                if (variables.find(var->getTag())==variables.end()) {
                    variables[var->getTag()] = std::vector<variableDeclaration*>();
                }
                variables[var->getTag()].push_back(new variableDeclaration(*var));
            }
                break;
            default:
                break;
            }
        }
    }
    return runType::success;
}
