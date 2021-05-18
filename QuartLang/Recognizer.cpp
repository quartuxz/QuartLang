#include "Recognizer.h"
#include <stdexcept>



//note that this will only end prematurely if the grammar is invalid
void Recognizer::m_makeProgram()
{



	auto createLiteral = [](const std::string &literalStr, Token token) {
		//IMPLEMENT THE OTHER CASES
		switch (token)
		{
		case Token::stringLiteralTok:
			return DataStructure(literalStr);
			break;
		case Token::intLiteralTok:
			return DataStructure(std::atoi(literalStr.c_str()));
			break;
		case Token::floatLiteralTok:
			return DataStructure((float)std::atof(literalStr.c_str()));
			break;
		case Token::boolLiteralTok:
			return DataStructure((literalStr == "true" ? true : false));
			break;
		default:
			throw std::invalid_argument("Token is not a literal!");
			break;
		}

		return DataStructure();
	};

	auto createOperand = [&createLiteral](Token currentToken, const Parser* parser) {

		operand retval;

		if (tokenIsLiteral(currentToken)) {
			retval = operand(createLiteral(parser->getLiteralTokPosMinus(), currentToken));
		}
		else if (currentToken == Token::tagTok) {
			retval = operand(parser->getTagStringTokPosMinus());
		}
		//this is not strictly neccesary
		else if (currentToken == Token::resultTok) {
			retval = operand();
		}

		return retval;
	};


	Token currentToken = m_parser->getNextToken();
	Subprogram *currentProgram = &m_program;
	size_t currentStructure = 0;
	size_t currentScopeNesting = 0;

	auto addProgramContent = [&](statementType stttType){
		programContent stt;
		stt.isStatement = true;
		stt.orderedID = currentStructure;
		stt.type = stttType;
		currentProgram->m_contents.push_back(stt);
		
	};

	while (currentToken != Token::endTok) {
		switch (currentToken)
		{
		case Token::finallyTok:
		{
			addProgramContent(statementType::finallySttt);
			currentToken = m_parser->getNextToken();
			
			finallyType fType = finallyType::end;

			bool hasOperand = false;

			switch (currentToken)
			{
			case Token::endBlockTok:
				fType = finallyType::end;
				break;
			case Token::repeatTok:
				hasOperand = true;
				fType = finallyType::repeat;
				break;
			case Token::giveTok:
				hasOperand = true;
				fType = finallyType::give;
				break;
			default:
				break;
			}

			operand op;
			if (hasOperand) {
				currentToken = m_parser->getNextToken();
				op = createOperand(currentToken, m_parser);
			}


			currentProgram->m_finallySttts[currentStructure] = new finallySttt(currentStructure,fType, op);

			--currentScopeNesting;
			currentProgram = currentProgram->m_parent;
		}
			break;
		case Token::ifTok:
		{
			programContent blockSttt;
			blockSttt.orderedID = currentStructure;
			blockSttt.isStatement = false;
			currentProgram->m_contents.push_back(blockSttt);
			currentScopeNesting++;
			currentToken = m_parser->getNextToken();
			
			Subprogram *newBlock = new conditionalBlock(currentStructure,createOperand(currentToken,m_parser));
			currentProgram->m_addSubprogram(currentStructure, newBlock);
			currentProgram = newBlock;
		}
			break;
		case Token::divideTok:
		case Token::multiplyTok:
		case Token::subtractTok:
		case Token::addTok:
		{
			addProgramContent(statementType::arithmeticOperationSttt);

			Token operationType = currentToken;

			std::vector<operand> operands;
			for (size_t i = 0; i < 2; i++)
			{
				currentToken = m_parser->getNextToken();
				operands.push_back(createOperand(currentToken, m_parser));
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
			addProgramContent(statementType::setOperationSttt);

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
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag,createLiteral(m_parser->getLiteralTokPosMinus(),currentToken));
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
				addProgramContent(statementType::variableDeclarationSttt);
				currentToken = m_parser->getNextToken();
				currentProgram->m_variables[currentStructure] = new variableDeclaration(currentStructure, m_parser->getTagStringTokPosMinus(), currentScopeNesting);
			}
				break;
			default:
				break;
			}
			break;
		case Token::callFunctionTok:
		{
			addProgramContent(statementType::functionCallSttt);
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
					op = createOperand(currentToken, m_parser);
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
			currentProgram->m_functionCalls[currentStructure] = new functionCall(currentStructure, m_parser->getTagString(functionTagTokenPos), args);
			LOG_ISHLENG((*m_logger), "Recognizer, Function tag", currentProgram->m_functionCalls[currentStructure]->getFunctionCalledTag());
		}
			break;
		default:
			//this gets called if the token is not a valid start of a new statement and also pretty much once after every statement
			//as the last token is kept as current.
			currentToken = m_parser->getNextToken();
			break;
		}
		++currentStructure;
		LOG_ISHLENG((*m_logger),"Recognizer",std::to_string(currentStructure));
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