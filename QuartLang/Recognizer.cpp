#include "Recognizer.h"
#include <stdexcept>



//note that this will only end prematurely if the grammar is invalid
void Recognizer::m_makeProgram()
{

	Token currentToken = m_parser->getNextToken();
	Subprogram *currentProgram = &m_program;
	size_t currentStructure = 1;
	while (currentToken != Token::endTok) {
		switch (currentToken)
		{
		case Token::setTok:
		{
			programContent stt;
			stt.isStatement = true;
			stt.orderedID = currentStructure;
			stt.type = statementType::setOperationSttt;
			currentProgram->m_contents.push_back(stt);

			currentToken = m_parser->getNextToken();
			std::string varTag = m_parser->getTagStringTokPosMinus();
			currentToken = m_parser->getNextToken();


			switch (currentToken) {
			case Token::tagTok:
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure,varTag,m_parser->getTagStringTokPosMinus());
				break;
			case Token::resultToken:
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag);
				break;
			default:
				break;
			}

			if (tokenIsLiteral(currentToken)) {
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag,m_program.m_addLiteral(m_parser->getLiteralTokPosMinus(),currentToken));
			}

			setOperation setOp(currentStructure, varTag);
			break;
		}
		case Token::lambdaTok:
			currentToken = m_parser->getNextToken();
			break;
		case Token::declareTok:
			currentToken = m_parser->getNextToken();
			switch (currentToken)
			{
			case Token::variableTok:
			{
				programContent stt;
				stt.isStatement = true;
				stt.orderedID = currentStructure;
				stt.type = statementType::variableDeclarationSttt;
				currentToken = m_parser->getNextToken();
				currentProgram->m_addVariableDeclaration(new variableDeclaration(currentStructure, m_parser->getTagStringTokPosMinus(), nullptr));
				currentProgram->m_contents.push_back(stt);
			}
				break;
			default:
				break;
			}
			break;
		case Token::callFunctionTok:
		{
			programContent stt;

			stt.isStatement = true;
			stt.orderedID = currentStructure;
			stt.type = statementType::functionCallSttt;
			
			size_t numberOfVariableArgs = 0;
			currentProgram->m_contents.push_back(stt);
			size_t functionTagTokenPos = m_parser->getCurrentTokenPosition();
			m_logger->log("Recognizer", m_parser->getTagString(m_parser->getCurrentTokenPosition()));
			std::map<std::string, DataStructure*> args;
			DataStructure *dat = nullptr;
			currentToken = m_parser->getNextToken();
			currentToken = m_parser->getNextToken();

			size_t it = 0;
			while(tokenIsLiteralOrTag(currentToken)) {
				//first we parse the argument
				if (it % 2 == 0) {
					//we detect a variable
					if (currentToken == Token::tagTok) {
						numberOfVariableArgs++;
						dat = m_program.m_addTag(m_parser->getTagStringTokPosMinus());
					}
					//we detect a literal
					else {
						dat = m_program.m_addLiteral(m_parser->getLiteralTokPosMinus(), currentToken);
						m_logger->log("Recognizer, Literal", m_parser->getLiteral(m_parser->getCurrentTokenPosition()-1));
						
					}
				}
				//then we parse the place where it is put
				else {
					//we populate the args
					args[m_parser->getTagStringTokPosMinus()] = dat;
					m_logger->log("Recognizer, Tag", m_parser->getTagStringTokPosMinus());
				}
				it++;
				currentToken = m_parser->getNextToken();
				
			}
			m_logger->log("Recognizer", m_parser->getTagString(functionTagTokenPos));
			//we insert a function call at the specified orderer place
			currentProgram->m_functionCalls[currentStructure] = new functionCall(currentStructure, m_parser->getTagString(functionTagTokenPos), args, numberOfVariableArgs);
			m_logger->log("Recognizer, FunctionTag", currentProgram->m_functionCalls[currentStructure]->getFunctionCalledTag());
		}
			break;
		default:
			currentToken = m_parser->getNextToken();
			break;
		}

		LOG_ISHLENG((*m_logger),"Recognizer",std::to_string(++currentStructure));
	}

}

Recognizer::Recognizer(Parser* parser, Logger *logger):
	m_parser(parser),
	m_program(),
	m_logger(logger)
{
	m_makeProgram();
	//we include all the basic BIFs
	m_program.m_includedBuiltins["print"] = new print_BIF();
}

const Program* Recognizer::getProgram() const noexcept
{
	return &m_program;
}

Program* Recognizer::getProgram() noexcept
{
	return &m_program;
}