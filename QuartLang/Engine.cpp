#include "Engine.h"

Engine::Engine(const Program* program, Logger *logger):
    m_program(program),
    m_logger(logger)
{
}

runType Engine::run()
{
    std::vector<programContent> contents = m_program->getContent();
    for (size_t i = 0; i < contents.size(); i++)
    {
        if (contents[i].isStatement) {
            switch (contents[i].type)
            {
            case statementType::functionCallSttt:
            {

                auto fCall = m_program->getFunctionCall(contents[i].orderedID);
                builtinFunction* builtin = m_program->getIncludedBuiltin(fCall->getFunctionCalledTag());
                m_logger->log("Engine", fCall->getFunctionCalledTag());
                //a builtin matching the function was found
                if (builtin != nullptr) {

                    builtin->call(fCall->getArgs());
                }
                //a builtin was not found
                else {

                }
                
                break;
            }
            default:
                break;
            }
        }
    }
    return runType::success;
}
