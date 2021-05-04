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
				variableDeclaration var(currentStructure,);

				currentProgram->m_contents.push_back(stt);
				currentProgram->m_variables[currentStructure] = ;
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
			std::map<std::string, DataStructure> args;
			DataStructure dat;
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
						dat = m_program.m_addLiteral(m_parser->getLiteral(m_parser->getCurrentTokenPosition()-1), currentToken);
						m_logger->log("Recognizer, Literal", m_parser->getLiteral(m_parser->getCurrentTokenPosition()-1));
						
					}
				}
				//then we parse the place where it is put
				else {
					//we populate the args
					args[m_parser->getTagString(m_parser->getCurrentTokenPosition()-1)] = dat;
					m_logger->log("Recognizer, Tag", m_parser->getTagString(m_parser->getCurrentTokenPosition()-1));
				}
				it++;
				currentToken = m_parser->getNextToken();
				
			}
			m_logger->log("Recognizer", m_parser->getTagString(functionTagTokenPos));
			//we insert a function call at the specified orderer place
			currentProgram->m_functionCalls[currentStructure] = new functionCall(currentStructure, m_parser->getTagString(functionTagTokenPos), args);
			m_logger->log("Recognizer, FunctionTag", currentProgram->m_functionCalls[currentStructure]->getFunctionCalledTag());
		}
			break;
		default:
			currentToken = m_parser->getNextToken();
			break;
		}
		++currentStructure;
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

//IMPLEMENT THIS FULLY
DataStructure Program::m_addLiteral(std::string literalStr, Token token)
{
	
	void* data = nullptr;
	std::string type;
	//IMPLEMENT THE OTHER CASES
	switch (token)
	{
	case Token::stringLiteralTok:
		type = "string";
		m_stringLiterals.push_back(literalStr);
		data = &m_stringLiterals.back();
		break;
	case Token::intLiteralTok:
		type = "int";
		m_intLiterals.push_back(std::atoi(literalStr.c_str()));
		data = &m_intLiterals.back();
		break;
	case Token::floatLiteralTok:
		type = "float";
		m_floatLiterals.push_back(std::atof(literalStr.c_str()));
		data = &m_floatLiterals.back();
		break;
	case Token::boolLiteralTok:
		type = "bool";
		m_boolLiterals.push_back((literalStr == "true"?true:false));
		data = &m_boolLiterals.back();;
		break;
	default:
		throw std::invalid_argument("Token is not a literal!");
		break;
	}
	return DataStructure(type,std::map<std::string, DataStructure*>(), data);
}

DataStructure Program::m_addTag(std::string tagStr)
{
	m_tags.push_back(tagStr);
	return DataStructure("tagString", std::map<std::string,DataStructure*>(),&m_tags.back());
}

Subprogram::Subprogram(size_t orderedID, subprogramType type) noexcept:
	ProgramStructure(orderedID, type)
{
}

void Subprogram::addSubprogram(const Subprogram& subprogram)
{
}

std::string Subprogram::getTag() const noexcept
{
	return m_tag;
}

std::vector<programContent> Subprogram::getContent() const noexcept
{
	return m_contents;
}

Subprogram::~Subprogram()
{
	for (auto x:m_variables)
	{
		delete x.second;
	}

	for (auto x:m_functionCalls) {
		delete x.second;
	}
	
	for (auto x:m_subprograms) {
		delete x.second;
	}
}

const functionCall* Subprogram::getFunctionCall(size_t orderedID)const
{
	return m_functionCalls.at(orderedID);
}

builtinFunction* Program::getIncludedBuiltin(std::string m_name)const
{
	builtinFunction *retVal = nullptr;
	if (m_includedBuiltins.find(m_name) != m_includedBuiltins.end()) {
		retVal = m_includedBuiltins.at(m_name);
	}
	return retVal;
}


Program::~Program()
{
	for (auto x : m_includedBuiltins) {
		delete x.second;
	}
}

Program::Program() noexcept:
	Subprogram(0,subprogramType::baseBlock)
{

}

variableDeclaration::variableDeclaration(size_t orderedID, const std::string& tag, const DataStructure& data) noexcept:
	ProgramStructure<statementType>(orderedID, statementType::variableDeclarationSttt), 
	m_tag(tag), 
	m_data(data)
{

}

functionCall::functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, DataStructure>& args, size_t variableArgsNum):
	ProgramStructure<statementType>(orderedID, statementType::functionCallSttt), 
	m_functionCalledTag(functionCalledTag),
	m_args(args),
	m_variableArgsNum(variableArgsNum)
{
}

functionCall::functionCall():
	ProgramStructure<statementType>(0, statementType::functionCallSttt)
{
}

std::string functionCall::getFunctionCalledTag() const noexcept
{
	return m_functionCalledTag;
}


std::map<std::string, DataStructure> functionCall::getArgs() const noexcept
{
	return m_args;
}

