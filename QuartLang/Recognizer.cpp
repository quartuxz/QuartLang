#include "Recognizer.h"
#include <stdexcept>



//note that this will only end prematurely if the grammar is invalid
void Recognizer::m_makeProgram()
{

	Token currentToken = m_parser->getNextToken();
	Subprogram *currentProgram = &m_program;
	size_t currentStructure = 1;
	size_t currentScopeNesting = 0;
	while (currentToken != Token::endTok) {
		switch (currentToken)
		{
		case Token::ifTok:

			break;
		case Token::divideTok:
		case Token::multiplyTok:
		case Token::subtractTok:
		case Token::addTok:
		{
			
			programContent stt;
			stt.isStatement = true;
			stt.orderedID = currentStructure;
			stt.type = statementType::arithmeticOperationSttt;
			currentProgram->m_contents.push_back(stt);

			Token operationType = currentToken;

			std::vector<operand> operands;
			for (size_t i = 0; i < 2; i++)
			{
				currentToken = m_parser->getNextToken();
				if (tokenIsLiteral(currentToken)) {
					operands.push_back(operand(m_program.m_addLiteral(m_parser->getLiteralTokPosMinus(), currentToken)));
				}
				else if (currentToken == Token::tagTok) {
					operands.push_back(operand(m_parser->getTagStringTokPosMinus()));
				}
				else if (currentToken == Token::resultTok) {
					operands.push_back(operand());
				}
			}

			currentToken = m_parser-> getNextToken();

			arithmeticOperationType arithmeticOpType;

			switch (operationType) {
			case Token::divideTok:
				arithmeticOpType = arithmeticOperationType::divide;
				break;
			case Token::multiplyTok:
				arithmeticOpType = arithmeticOperationType::multiply;
				break;
			case Token::subtractTok:
				arithmeticOpType = arithmeticOperationType::subtract;
				break;
			case Token::addTok:
				arithmeticOpType = arithmeticOperationType::add;
				break;
			}
			currentProgram->m_arithmeticOperations[currentStructure] = new arithmeticOperation(currentStructure, arithmeticOpType, operands[0], operands[1]);
		}

		break;
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
			//setting to variable
			case Token::tagTok:
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure,varTag,operand(m_parser->getTagStringTokPosMinus()));
				break;
			//setting to result
			case Token::resultTok:
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag, operand());
				break;
			default:
				break;
			}

			//setting to literal
			if (tokenIsLiteral(currentToken)) {
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag,m_program.m_addLiteral(m_parser->getLiteralTokPosMinus(),currentToken));
			}

			//setOperation setOp(currentStructure, varTag);
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
				currentProgram->m_variables[currentStructure] = new variableDeclaration(currentStructure, m_parser->getTagStringTokPosMinus(), currentScopeNesting);
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
			LOG_ISHLENG((*m_logger), "Recognizer", m_parser->getTagString(m_parser->getCurrentTokenPosition()));
			std::map<std::string, operand> args;
			operand op;
			currentToken = m_parser->getNextToken();
			currentToken = m_parser->getNextToken();

			size_t it = 0;
			while(tokenIsLiteralOrTag(currentToken) || currentToken == Token::resultTok) {

				//first we parse the argument
				if (it % 2 == 0) {
					op = operand();
					//we detect a variable
					if (currentToken == Token::tagTok) {
						numberOfVariableArgs++;
						op.type = operandType::variable;
						op.varName = m_parser->getTagStringTokPosMinus();
					}
					else if (currentToken == Token::resultTok) {
						op.type = operandType::result;
					}
					//we detect a literal
					else {
						op.type = operandType::literal;
						op.literalData = m_program.m_addLiteral(m_parser->getLiteralTokPosMinus(), currentToken);
						LOG_ISHLENG((*m_logger), "Recognizer, Literal", m_parser->getLiteralTokPosMinus());
						
					}
				}
				//then we parse the place where it is put
				else {
					//we populate the args
					args[m_parser->getTagStringTokPosMinus()] = op;
					LOG_ISHLENG((*m_logger), "Recognizer", m_parser->getTagStringTokPosMinus());
				}
				it++;
				currentToken = m_parser->getNextToken();
				
			}
			LOG_ISHLENG((*m_logger), "Recognizer", m_parser->getTagString(functionTagTokenPos));
			//we insert a function call at the specified orderer place
			currentProgram->m_functionCalls[currentStructure] = new functionCall(currentStructure, m_parser->getTagString(functionTagTokenPos), args, numberOfVariableArgs);
			LOG_ISHLENG((*m_logger), "Recognizer, Function tag", currentProgram->m_functionCalls[currentStructure]->getFunctionCalledTag());
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
	m_program.m_includedBuiltins["print_new_line"] = new print_new_line_BIF();
}

const Program* Recognizer::getProgram() const noexcept
{
	return &m_program;
}

Program* Recognizer::getProgram() noexcept
{
	return &m_program;
}