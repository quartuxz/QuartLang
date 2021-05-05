#include "Engine.h"

Engine::Engine(Program* program, Logger *logger):
    m_program(program),
    m_logger(logger)
{
}

runType Engine::run()
{
    std::vector<programContent> currentContents = m_program->getContent();
    Subprogram* currentProgram = m_program;

    std::map<std::string, DataStructure> lastFunctionReturn;
    for (size_t i = 0; i < currentContents.size(); i++)
    {
        size_t currentContentsOrderedId = currentContents[i].orderedID;
        if (currentContents[i].isStatement) {
            switch (currentContents[i].type)
            {
            case statementType::setOperationSttt:
            {
                 setOperation *setOp = currentProgram->getSetOperation(currentContentsOrderedId);
                 switch (setOp->getSetType()) {
                 case setType::toLiteral:
                     currentProgram->getVariableUpwards(setOp->getSettedName(),currentContentsOrderedId)->setData(setOp->getToLiteralData());
                     break;
                 case setType::toResult:
                     break;
                 case setType::toVariable:
                     break;
                 default:
                     break;
                 }
            }
            break;
            case statementType::functionCallSttt:
            {

                auto fCall = currentProgram->getFunctionCall(currentContentsOrderedId);
                auto args = fCall->getArgs();
                size_t numOfVarArgs = fCall->getVariableArgsNum();
                if (numOfVarArgs > 0) {
                    for (auto x : args) {
                        if (x.second.getTypeOrPrimitiveTag() == "tagString") {
                            variableDeclaration *tempVar = currentProgram->getVariableUpwards(*(std::string*)x.second.getData(),currentContentsOrderedId);
                            args[x.first] = *tempVar->getData();
                        }
                    }
                }
                
                builtinFunction* builtin = m_program->getIncludedBuiltin(fCall->getFunctionCalledTag());
                m_logger->log("Engine", fCall->getFunctionCalledTag());
                //a builtin matching the function was found
                if (builtin != nullptr) {

                    lastFunctionReturn = builtin->call(args);
                }
                //a builtin was not found
                else {
                    
                }
                
                break;
            }
            case statementType::variableDeclarationSttt:
                
                break;
            default:
                break;
            }
        }
    }
    return runType::success;
}
