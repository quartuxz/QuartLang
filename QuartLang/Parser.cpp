#include "Parser.h"
#include <stdexcept>



//note that this will only end prematurely if the grammar is invalid
void Parser::m_makeProgram()
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
		case Token::charLiteralTok:
			return DataStructure(literalStr[0]);
			break;
		default:
			throw std::invalid_argument("Token is not a literal!");
			break;
		}

		return DataStructure();
	};


	std::string lastTag;


	auto createOperand = [&createLiteral, &lastTag](Token currentToken, const Lexer* lexer) {

		operand retval;

		if (tokenIsLiteral(currentToken)) {
			retval = operand(createLiteral(lexer->getLiteralTokPosMinus(), currentToken));
		}
		else if (currentToken == Token::tagTok) {
			retval = operand(lexer->getTagStringTokPosMinus());
		}
		//this is not strictly neccesary
		else if (currentToken == Token::resultTok) {
			retval = operand();
		}
		else if (currentToken == Token::itTok) {
			retval = operand(lastTag);
		}

		return retval;
	};

	auto getVariableName = [&lastTag](Token currentToken, const Lexer* lexer) {

		std::string varTag;

		if (tokenIsLiteralOrTag(currentToken)) {
			varTag = lexer->getTagStringTokPosMinus();
		}
		else if (currentToken == Token::itTok) {
			varTag = lastTag;
		}
		return varTag;
	};


	Token currentToken = m_lexer->getNextToken();
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

	auto addProgramBlock = [&](Subprogram* subprogram) {
		programContent stt;
		stt.isStatement = false;
		stt.orderedID = currentStructure;
		currentProgram->m_contents.push_back(stt);
		currentScopeNesting++;
		currentProgram->m_addSubprogram(currentStructure, subprogram);
		currentProgram = subprogram;
	};


	auto addFunctionCallStatement = [&](bool isMultithreaded = false, const std::string &threadInstanceName = "") {
		addProgramContent(statementType::functionCallSttt);
		size_t functionTagTokenPos = m_lexer->getCurrentTokenPosition();
		LOG_ISHLENG((*m_logger), "Parser", m_lexer->getTagString(m_lexer->getCurrentTokenPosition()));
		std::map<std::string, operand> args;
		operand op;
		currentToken = m_lexer->getNextToken();
		currentToken = m_lexer->getNextToken();

		size_t it = 0;
		while (tokenIsOperand(currentToken)) {

			//first we parse the argument
			if (it % 2 == 0) {
				op = createOperand(currentToken, m_lexer);
			}
			//then we parse the place where it is put
			else {
				//we populate the args
				args[m_lexer->getTagStringTokPosMinus()] = op;
				LOG_ISHLENG((*m_logger), "Parser", m_lexer->getTagStringTokPosMinus());
			}
			it++;
			currentToken = m_lexer->getNextToken();

		}
		LOG_ISHLENG((*m_logger), "Parser", m_lexer->getTagString(functionTagTokenPos));
		//we insert a function call at the specified orderer place
		currentProgram->m_functionCalls[currentStructure] = new functionCall(currentStructure, m_lexer->getTagString(functionTagTokenPos), args, isMultithreaded,threadInstanceName);
		LOG_ISHLENG((*m_logger), "Parser, Function tag", currentProgram->m_functionCalls[currentStructure]->getFunctionCalledTag());
	};

	while (currentToken != Token::endTok) {
		switch (currentToken)
		{
		case Token::finishTok:
		{
			addProgramContent(statementType::finishSttt);
			

			currentToken = m_lexer->getNextToken();

			currentProgram->m_finishOperations[currentStructure] = new finishOperation(currentStructure,m_lexer->getTagStringTokPosMinus());
		}
			break;
		case Token::referTok:
		{
			addProgramContent(statementType::referOperationSttt);

			
			
			currentToken = m_lexer->getNextToken();
			auto op = createOperand(currentToken, m_lexer);
			currentToken = m_lexer->getNextToken();


			auto varName = getVariableName(currentToken, m_lexer);

			currentToken = m_lexer->getNextToken();
			auto referantName = m_lexer->getTagStringTokPosMinus();

			lastTag = referantName;

			currentProgram->m_referOperations[currentStructure] = new referOperation(currentStructure, varName,op, referantName);
		}
			break;
		
		case Token::appendTok:
		{
			addProgramContent(statementType::appendOperationSttt);


			currentToken = m_lexer->getNextToken();
			auto content = createOperand(currentToken, m_lexer);

			currentToken = m_lexer->getNextToken();

			operand placeAt;

			appendType appType;

			if (!tokenIsLiteralOrTag(currentToken)) {
				switch (currentToken) {
				case Token::backTok:
					appType = appendType::push_back;
					break;
				case Token::frontTok:
					appType = appendType::push_front;
					break;
				}
			}
			else {
				appType = appendType::map_to;
				placeAt = createOperand(currentToken, m_lexer);
			}

			currentToken = m_lexer->getNextToken();
			auto varName = getVariableName(currentToken, m_lexer);

			currentProgram->m_appendOperations[currentStructure] = new appendOperation(currentStructure,appType,placeAt,content,varName);
		}
			break;

		case Token::flipTok:
		{
			addProgramContent(statementType::flipOperationSttt);
			currentToken = m_lexer->getNextToken();
			currentProgram->m_flipOperations[currentStructure] = new flipOperation(currentStructure,createOperand(currentToken, m_lexer));
		}
			break;
		case Token::evaluateTok:
		{
			addProgramContent(statementType::evaluateOperationSttt);
			//the first operand is read after the evaluate token
			currentToken = m_lexer->getNextToken();
			//declare both operands assign the first
			auto lhs = createOperand(currentToken, m_lexer);
			operand rhs;


			currentToken = m_lexer->getNextToken();
			evalType eType = evalType::equal;
			if(!tokenIsOperand(currentToken)){

				//the first operation is read if the token is not an operand(no the form: evaluate operand1 operand2; but the form: evaluate operand operation ... operand) after the first operand

				auto firstOperation = currentToken;
				Token secondOperation = Token::lambdaTok;
			
				currentToken = m_lexer->getNextToken();

				//if the next token is an operand the evaluation is parsed
				if (tokenIsOperand(currentToken)) {

					rhs = createOperand(currentToken, m_lexer);
				}
				//if the next token is not an operand the evaluation has a second operation
				else{
					//optional or token to separate operations
					if (currentToken == Token::orTok) {
						//we just skip it
						currentToken = m_lexer->getNextToken();
					}
					//if the optional token was not there, we read current one as the operation
					secondOperation = currentToken;
					currentToken = m_lexer->getNextToken();
					//next and final is the second operand
					rhs = createOperand(currentToken, m_lexer);
				}


				bool foundType = true;
				if (secondOperation == Token::lambdaTok) {
					switch (firstOperation)
					{
					case Token::andTok:
						eType = evalType::andEval;
						break;
					case Token::orTok:
						eType = evalType::orEval;
						break;
					case Token::equalToTok:
						eType = evalType::equal;
						break;
					case Token::lessThanTok:
						eType = evalType::lessThan;
						break;
					case Token::moreThanTok:
						eType = evalType::moreThan;
						break;
					default:
						foundType = false;
						break;
					}
				}
				else {
					bool matched = false;
					for (size_t i = 0; i < 2; i++)
					{
						if (firstOperation == Token::lessThanTok && secondOperation == Token::equalToTok) {
							eType = evalType::lessThanOrEqual;
							matched = true;
							break;
						}
						else if (firstOperation == Token::moreThanTok && secondOperation == Token::equalToTok) {
							eType = evalType::moreThanOrEqual;
							matched = true;
							break;
						}
						auto temp = secondOperation;
						firstOperation = secondOperation;
						secondOperation = temp;
					}
					if (!matched) {
						foundType = false;
					}
				}

				//ill formed evaluation statement
				if (!foundType) {
					//THROW EXCEPTION HERE
				}
			}
			else {
				rhs = createOperand(currentToken,m_lexer);
			}

			
			currentProgram->m_evaluateOperations[currentStructure] = new evaluateOperation(currentStructure,eType,lhs,rhs);
		}
			break;
		case Token::finallyTok:
		{
			addProgramContent(statementType::finallySttt);
			currentToken = m_lexer->getNextToken();
			
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
				currentToken = m_lexer->getNextToken();
				op = createOperand(currentToken, m_lexer);
			}


			currentProgram->m_finallySttts[currentStructure] = new finallySttt(currentStructure,fType, op);

			--currentScopeNesting;
			currentProgram = currentProgram->m_parent;
		}
			break;
		case Token::ifTok:
		{

			currentToken = m_lexer->getNextToken();

			addProgramBlock(new conditionalBlock(currentStructure, createOperand(currentToken, m_lexer)));

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
				currentToken = m_lexer->getNextToken();
				operands.push_back(createOperand(currentToken, m_lexer));
			}

			currentToken = m_lexer-> getNextToken();

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

			currentToken = m_lexer->getNextToken();
			std::string varTag = getVariableName(currentToken, m_lexer);
			currentToken = m_lexer->getNextToken();

			
			switch (currentToken) {
			//setting to variable
			case Token::tagTok:
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure,varTag,operand(m_lexer->getTagStringTokPosMinus()));
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
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag,createLiteral(m_lexer->getLiteralTokPosMinus(),currentToken));
			}

			//setOperation setOp(currentStructure, varTag);
			break;
		}
		case Token::lambdaTok:
			currentToken = m_lexer->getNextToken();
			break;
		case Token::declareTok:
			currentToken = m_lexer->getNextToken();
			switch (currentToken)
			{
			case Token::variableTok:
			{
				addProgramContent(statementType::variableDeclarationSttt);
				currentToken = m_lexer->getNextToken();
				currentProgram->m_variables[currentStructure] = new variableDeclaration(currentStructure, m_lexer->getTagStringTokPosMinus(), currentScopeNesting);
				lastTag = m_lexer->getTagStringTokPosMinus();
			}
				break;
			case Token::functionTok:
			{
				currentToken = m_lexer->getNextToken();
				lastTag = m_lexer->getTagStringTokPosMinus();
				addProgramBlock(new functionBlock(currentStructure,lastTag));
				
			}
			default:
				break;
			}
			break;
		case Token::launchTok:
			currentToken = m_lexer->getNextToken();
			
			addFunctionCallStatement(true,m_lexer->getTagStringTokPosMinus());
			break;
		case Token::callFunctionTok:
		{
			addFunctionCallStatement();
		}
			break;
		default:
			//this gets called if the token is not a valid start of a new statement and also pretty much once after every statement
			//as the last token is kept as current.
			currentToken = m_lexer->getNextToken();
			break;
		}
		++currentStructure;
		LOG_ISHLENG((*m_logger),"Parser",std::to_string(currentStructure));
	}

}

Parser::Parser(Lexer* Lexer, Logger *logger):
	m_lexer(Lexer),
	m_program(),
	m_logger(logger)
{
	m_makeProgram();
	//we include all the basic BIFs
	m_program.m_includedBuiltins["print-anything"] = new print_anything_BIF();
	m_program.m_includedBuiltins["print-new-line"] = new print_new_line_BIF();
	m_program.m_includedBuiltins["is-empty"] = new is_empty_BIF();
}

const Program* Parser::getProgram() const noexcept
{
	return &m_program;
}

Program* Parser::getProgram() noexcept
{
	return &m_program;
}