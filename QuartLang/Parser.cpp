#include "Parser.h"
#include <stdexcept>
#include <fstream>
#include "Ishleng.h"

void Parser::m_makeProgram(const Lexer* lexer)
{

	currentToken = lexer->getNextToken();


	auto createLiteral = [](const std::string& literalStr, Token token) {
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



	auto getBinding = [&m_bindings = m_bindings](Token currentTok, const Lexer* lex) {
		if (tokenIsLiteral(currentTok)) {
			return std::make_pair(currentTok, lex->getLiteralTokPosMinus());
		}
		else if (currentTok == Token::tagTok) {
			return m_bindings[lex->getTagStringTokPosMinus()];
		}
		else {
			//ERROR
		}
	};


	auto createOperand = [&createLiteral, &lastTag = lastTag, &m_bindings = m_bindings](Token currentToken, const Lexer* lexer) {

		operand retval;

		if (tokenIsLiteral(currentToken)) {
			retval = operand(createLiteral(lexer->getLiteralTokPosMinus(), currentToken));
		}
		else if (currentToken == Token::tagTok) {
			if (m_bindings.find(lexer->getTagStringTokPosMinus()) != m_bindings.end()) {
				//we search for the binding tag and get its literal and literal type
				auto temp = m_bindings[lexer->getTagStringTokPosMinus()];
				retval = operand(createLiteral(temp.second, temp.first));
			}
			else {
				retval = operand(lexer->getTagStringTokPosMinus());
			}

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

	auto getVariableName = [&lastTag = lastTag](Token currentToken, const Lexer* lexer) {

		std::string varTag;

		if (tokenIsLiteralOrTag(currentToken)) {
			varTag = lexer->getTagStringTokPosMinus();
		}
		else if (currentToken == Token::itTok) {
			varTag = lastTag;
		}
		return varTag;
	};




	auto addProgramContent = [&currentStructure = currentStructure,&currentProgram = currentProgram](statementType stttType) {
		programContent stt;
		stt.isStatement = true;
		stt.orderedID = currentStructure;
		stt.type = stttType;
		currentProgram->m_contents.push_back(stt);

	};

	auto addProgramBlock = [&currentScopeNesting = currentScopeNesting, &currentStructure = currentStructure, &currentProgram = currentProgram](Subprogram* subprogram) {
		programContent stt;
		stt.isStatement = false;
		stt.orderedID = currentStructure;
		stt.type = statementType::notASttt;
		currentProgram->m_contents.push_back(stt);
		currentScopeNesting++;
		currentProgram->m_addSubprogram(currentStructure, subprogram);
		currentProgram = subprogram;
	};


	auto addFunctionCallStatement = [&addProgramContent,&lexer = lexer,&m_logger = m_logger,&currentToken = currentToken,&createOperand, &currentProgram = currentProgram, &currentStructure = currentStructure](bool isMultithreaded = false) {
		
		addProgramContent(statementType::functionCallSttt);
		size_t functionTagTokenPos = lexer->getCurrentTokenPosition();
		LOG_ISHLENG((*m_logger), "Parser", lexer->getTagString(lexer->getCurrentTokenPosition()));
		std::map<std::string, operand> args;
		operand op;
		currentToken = lexer->getNextToken();
		currentToken = lexer->getNextToken();

		size_t it = 0;
		while (tokenIsOperand(currentToken)) {

			//first we parse the argument
			if (it % 2 == 0) {
				op = createOperand(currentToken, lexer);
			}
			//then we parse the place where it is put
			else {
				//we populate the args
				args[lexer->getTagStringTokPosMinus()] = op;
				LOG_ISHLENG((*m_logger), "Parser", lexer->getTagStringTokPosMinus());
			}
			it++;
			currentToken = lexer->getNextToken();

		}
		LOG_ISHLENG((*m_logger), "Parser", lexer->getTagString(functionTagTokenPos));
		//we insert a function call at the specified orderer place
		currentProgram->m_functionCalls[currentStructure] = new functionCall(currentStructure, lexer->getTagString(functionTagTokenPos), args, isMultithreaded);
		LOG_ISHLENG((*m_logger), "Parser, Function tag", currentProgram->m_functionCalls[currentStructure]->getFunctionCalledTag());
	};



	while (currentToken != Token::endTok) {
		switch (currentToken)
		{
		case Token::importTok:
		{

			
			addProgramContent(statementType::importSttt);
			currentToken = m_lexer->getNextToken();
			currentProgram->m_importOperations[currentStructure] = new importOperation(currentStructure,createOperand(currentToken,m_lexer));
			
			
			
		}
			break;
		case Token::openTok:
		case Token::bindTok:
		{
			auto keywordTok = currentToken;
			std::string bindingName;
			std::pair<Token, std::string> literalContents;

			//the next token is a literal
			currentToken = lexer->getNextToken();
			//we get the literal itself or expand a binding with the given tag to get it
			auto contents = getBinding(currentToken, lexer);
			//we open a file and bind its contents
			if (keywordTok == Token::openTok) {
				const std::string& filepath = contents.second;

				std::ifstream t(filepath);
				literalContents = std::make_pair(Token::stringLiteralTok, std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>()));

			}
			//we just bind whatever is passed.
			else {
				literalContents = contents;
			}


			//we parse the tag
			currentToken = lexer->getNextToken();
			bindingName = lexer->getTagStringTokPosMinus();


			m_bindings[bindingName] = literalContents;
		}
			break;
		case Token::includeTok:
		{
			currentToken = lexer->getNextToken();


			Ishleng nestedIshleng(m_logger, lexer->getDictionaryLexer(), getBinding(currentToken, lexer).second,false);

			nestedIshleng.lex();
			//TODO: use syntactic validator here
			m_makeProgram(nestedIshleng.getLexer());
			currentToken = lexer->getNextToken();
		}
			break;
		case Token::finishTok:
		{
			addProgramContent(statementType::finishSttt);


			currentToken = lexer->getNextToken();

			currentProgram->m_finishOperations[currentStructure] = new finishOperation(currentStructure, createOperand(currentToken,lexer));
		}
		break;
		case Token::referTok:
		{
			addProgramContent(statementType::referOperationSttt);



			currentToken = lexer->getNextToken();
			auto op = createOperand(currentToken, lexer);
			currentToken = lexer->getNextToken();


			auto varName = getVariableName(currentToken, lexer);

			currentToken = lexer->getNextToken();
			auto referantName = lexer->getTagStringTokPosMinus();

			lastTag = referantName;

			currentProgram->m_referOperations[currentStructure] = new referOperation(currentStructure, varName, op, referantName);
		}
		break;

		case Token::appendTok:
		{
			addProgramContent(statementType::appendOperationSttt);


			currentToken = lexer->getNextToken();
			auto content = createOperand(currentToken, lexer);

			currentToken = lexer->getNextToken();

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
				placeAt = createOperand(currentToken, lexer);
			}

			currentToken = lexer->getNextToken();
			auto varName = getVariableName(currentToken, lexer);

			currentProgram->m_appendOperations[currentStructure] = new appendOperation(currentStructure, appType, placeAt, content, varName);
		}
		break;

		case Token::flipTok:
		{
			addProgramContent(statementType::flipOperationSttt);
			currentToken = lexer->getNextToken();
			currentProgram->m_flipOperations[currentStructure] = new flipOperation(currentStructure, createOperand(currentToken, lexer));
		}
		break;
		case Token::evaluateTok:
		{
			addProgramContent(statementType::evaluateOperationSttt);
			//the first operand is read after the evaluate token
			currentToken = lexer->getNextToken();
			//declare both operands assign the first
			auto lhs = createOperand(currentToken, lexer);
			operand rhs;


			currentToken = lexer->getNextToken();
			evalType eType = evalType::equal;
			if (!tokenIsOperand(currentToken)) {

				//the first operation is read if the token is not an operand(no the form: evaluate operand1 operand2; but the form: evaluate operand operation ... operand) after the first operand

				auto firstOperation = currentToken;
				Token secondOperation = Token::lambdaTok;

				currentToken = lexer->getNextToken();

				//if the next token is an operand the evaluation is parsed
				if (tokenIsOperand(currentToken)) {

					rhs = createOperand(currentToken, lexer);
				}
				//if the next token is not an operand the evaluation has a second operation
				else {
					//optional or token to separate operations
					if (currentToken == Token::orTok) {
						//we just skip it
						currentToken = lexer->getNextToken();
					}
					//if the optional token was not there, we read current one as the operation
					secondOperation = currentToken;
					currentToken = lexer->getNextToken();
					//next and final is the second operand
					rhs = createOperand(currentToken, lexer);
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
				rhs = createOperand(currentToken, lexer);
			}


			currentProgram->m_evaluateOperations[currentStructure] = new evaluateOperation(currentStructure, eType, lhs, rhs);
		}
		break;
		case Token::finallyTok:
		{
			addProgramContent(statementType::finallySttt);
			currentToken = lexer->getNextToken();

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
				currentToken = lexer->getNextToken();
				op = createOperand(currentToken, lexer);
			}


			currentProgram->m_finallySttts[currentStructure] = new finallySttt(currentStructure, fType, op);

			--currentScopeNesting;
			currentProgram = currentProgram->m_parent;
		}
		break;
		case Token::ifTok:
		{

			currentToken = lexer->getNextToken();

			addProgramBlock(new conditionalBlock(currentStructure, createOperand(currentToken, lexer)));

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
				currentToken = lexer->getNextToken();
				operands.push_back(createOperand(currentToken, lexer));
			}

			currentToken = lexer->getNextToken();

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

			currentToken = lexer->getNextToken();
			std::string varTag = getVariableName(currentToken, lexer);
			currentToken = lexer->getNextToken();


			switch (currentToken) {
				//setting to variable
			case Token::tagTok:
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag, operand(lexer->getTagStringTokPosMinus()));
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
				currentProgram->m_setOperations[currentStructure] = new setOperation(currentStructure, varTag, createLiteral(lexer->getLiteralTokPosMinus(), currentToken));
			}

			//setOperation setOp(currentStructure, varTag);
			break;
		}
		case Token::lambdaTok:
			currentToken = lexer->getNextToken();
			break;
		case Token::declareTok:
			currentToken = lexer->getNextToken();
			switch (currentToken)
			{
			case Token::variableTok:
			{
				addProgramContent(statementType::variableDeclarationSttt);
				currentToken = lexer->getNextToken();
				currentProgram->m_variables[currentStructure] = new variableDeclaration(currentStructure, lexer->getTagStringTokPosMinus(), currentScopeNesting);
				lastTag = lexer->getTagStringTokPosMinus();
			}
			break;
			case Token::functionTok:
			{
				currentToken = lexer->getNextToken();
				lastTag = lexer->getTagStringTokPosMinus();
				addProgramBlock(new functionBlock(currentStructure, lastTag));

			}
			default:
				break;
			}
			break;
		case Token::launchTok:
			addFunctionCallStatement(true);
			break;
		case Token::callFunctionTok:
			addFunctionCallStatement();
			break;
		default:
			//this gets called if the token is not a valid start of a new statement and also pretty much once after every statement
			//as the last token is kept as current.
			currentToken = lexer->getNextToken();
			break;
		}
		++currentStructure;
		LOG_ISHLENG((*m_logger), "Parser", std::to_string(currentStructure));
	}

}

//note that this will only end prematurely if the grammar is invalid
void Parser::m_makeProgram()
{
	m_makeProgram(m_lexer);
}

Parser::Parser(Lexer* lexer, Logger *logger):
	m_lexer(lexer),
	m_program(),
	m_logger(logger)
{
	m_makeProgram();
	//we include all the basic BIFs
	m_program.m_includedBuiltins["print-anything"] = new print_anything_BIF();
	m_program.m_includedBuiltins["print-new-line"] = new print_new_line_BIF();
	m_program.m_includedBuiltins["is-empty"] = new is_empty_BIF();
	m_program.m_includedBuiltins["get-input"] = new get_input_BIF();
	m_program.m_includedBuiltins["run-ishleng"] = new run_ishleng_BIF();
}

const Program* Parser::getProgram() const noexcept
{
	return &m_program;
}

Program* Parser::getProgram() noexcept
{
	return &m_program;
}

std::vector<functionBlock*> Parser::getFunctionsOnly(Lexer* lexer, Logger *logger, const Subprogram* parentProgram)
{
	std::vector<functionBlock*> retval;

	auto newParser =  Parser(lexer,logger);
	auto contents = newParser.m_program.getContent();
	for (auto x:contents) {
		if (!x.isStatement) {
			auto candidate = newParser.m_program.m_subprograms[x.orderedID];
			if (parentProgram != nullptr) {
				candidate->m_parent = const_cast<Subprogram*>(parentProgram);
			}

			if (candidate->getType() == subprogramType::functionBlock) {
				retval.push_back(new functionBlock(*dynamic_cast<functionBlock*>(candidate)));
				
			}
		}
	}
	return retval;
}
