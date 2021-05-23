#include <fstream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <ctype.h>
#include <locale>

#include "Lexer.h"



const std::map<std::string, Token> Lexer::m_matches = {
	/*we first define all lambda tokens, those that are not parsed and also arent tags DEPRECATED
	{"a", Token::lambdaTok},
	{"named", Token::lambdaTok},
	{"as", Token::lambdaTok},
	{"an", Token::lambdaTok},
	{"by", Token::lambdaTok},
	{"to", Token::lambdaTok},
	{"from", Token::lambdaTok},
	{"with", Token::lambdaTok},
	{"the", Token::lambdaTok},
	{"of", Token::lambdaTok},
	{"for", Token::lambdaTok},
	{"execution", Token::lambdaTok},
	{"giving", Token::lambdaTok},
	{"then", Token::lambdaTok},
	{"for", Token::lambdaTok},
	{"while", Token::lambdaTok},
	{"than", Token::lambdaTok},
	{"is", Token::lambdaTok},
	{"than", Token::lambdaTok},
	*/

	//then we define all other tokens
	{"declare", Token::declareTok},
	{"call", Token::callFunctionTok},
	{"variable", Token::variableTok},
	{"set", Token::setTok},
	{"result", Token::resultTok},
	{"add", Token::addTok},
	{"subtract", Token::subtractTok},
	{"multiply", Token::multiplyTok},
	{"divide", Token::divideTok},
	{"finally", Token::finallyTok},
	{"repeat", Token::repeatTok},
	{"give", Token::giveTok},
	{"if", Token::ifTok},
	{"end", Token::endBlockTok},
	{"evaluate", Token::evaluateTok},
	{"less", Token::lessThanTok},
	{"lower", Token::lessThanTok},
	{"more", Token::moreThanTok},
	{"greater", Token::moreThanTok},
	{"equal", Token::equalToTok},
	{"or", Token::orTok},
	{"and", Token::andTok},
	{"flip", Token::flipTok},
	{"refer", Token::referTok},
	{"append", Token::appendTok},
	{"back", Token::backTok},
	{"front", Token::frontTok},
	{"it", Token::itTok}
};



Lexer::Lexer(std::string filenameOrCode, Logger *logger,bool isFileTrueIsCodeFalse):
	m_logger(logger),
	m_dictionaryLexer("words_alpha.txt",m_matches)
{

	m_bindFileOrCode(filenameOrCode, isFileTrueIsCodeFalse);
	m_tokenize();
#ifdef MUST_LOG_ISHLENG
	
	logger->log("Lexer",m_text);

	
	std::stringstream sstream;

	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		sstream << GetStringToken(m_tokens[i]) << " ";
	}
	sstream << "\n" << "\n";
	logger->log("Lexer",sstream.str());

	for (auto x:m_literals) {
		logger->log("Lexer, literals",x.second);
	}
	for (auto x:m_tags) {
		logger->log("Lexer, tags",x.second);
	}

#endif
}

void Lexer::m_bindFileOrCode(std::string fileName, bool isFileTrueIsCodeFalse)
{
	std::string str;
	if (isFileTrueIsCodeFalse) {
		std::ifstream t(fileName);
		str = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	}
	else {
		str = fileName;
	}


	//we remove all special characters and excess whitespaces
	std::string noSpecialCharacters;
	for (size_t i = 0; i < str.size();i++) {
		if (str[i] == '\n' || str[i] == '\t') {
			noSpecialCharacters.push_back(' ');
		}
		else {
			noSpecialCharacters.push_back(str[i]);
		}
	}
	bool wasWhiteSpace = false;
	for (size_t i = 0; i < noSpecialCharacters.size(); i++) {


		if (!(noSpecialCharacters[i] == ' ' && wasWhiteSpace)) {

			m_text.push_back(noSpecialCharacters[i]);
		}

		if (wasWhiteSpace) {
			wasWhiteSpace = false;
		}

		if (noSpecialCharacters[i] == ' ') {
			wasWhiteSpace = true;
		}
	}
	if (m_text[0] == ' ') {
		m_text = std::string(++m_text.begin(),m_text.end());
	}
	m_text.push_back(' ');
}

void Lexer::m_tokenize()
{
	//we reset the previous tokens.
	m_currentToken = 0;
	m_tokens.clear();
	std::string currentTokenString;
	size_t i = 0;
	while(true) {
		currentTokenString = m_readNextWord();
		if (currentTokenString == "") {
			break;
		}
		//we encounter a string start
		if (currentTokenString[0] == '\"') {
			std::stringstream ss;
			ss << currentTokenString;

			if (currentTokenString.back() != '\"') {
				do {
					ss << " ";
					ss << m_readNextWord();
				}while (ss.str().back() != '\"');
			}
			std::string tempStr = ss.str();
			std::string finalStr(++tempStr.begin(), --tempStr.end());
			m_addLiteral(finalStr, Token::stringLiteralTok);
		}
		//finding a integer or a floating point
		else if (isdigit(currentTokenString[0])) {
			bool isInteger = true;
			for (auto x: currentTokenString) {
				if (x == '.') {
					isInteger = false;
					m_addLiteral(currentTokenString, Token::floatLiteralTok);
					break;
				}
			}
			if (isInteger) {
				m_addLiteral(currentTokenString, Token::intLiteralTok);
			}
		}
		//we encounter a boolean
		else if (currentTokenString == "true" || currentTokenString == "false") {
			m_addLiteral(currentTokenString, Token::boolLiteralTok);
		}
		else {
			if (m_dictionaryLexer.checkIfStringHasToken(currentTokenString)) {
				Token tok = m_dictionaryLexer.getTokenForString(currentTokenString);
				if (tok != Token::lambdaTok) {
					m_tokens.push_back(m_dictionaryLexer.getTokenForString(currentTokenString));
				}
			}
			//if the token is not in the list, it is a tag
			else {
				m_tags[m_tokens.size()] = currentTokenString;
				m_tokens.push_back(Token::tagTok);
			}
		}

		m_logger->log("Lexer internals", currentTokenString);
	}
	


	//we insert a lambda token to signify an empty word for the whole program.
	if (m_tokens.empty()) {
		m_tokens.push_back(Token::lambdaTok);
	}
	m_tokens.push_back(Token::endTok);
}

std::string Lexer::m_readNextWord(char separatingToken)
{
	size_t currentCharminusOne = m_currentChar == 0 ? 0:m_currentChar-1;
	if (currentCharminusOne >= m_text.size()) {
		return "";
	}
	std::string retval;
	if (m_currentChar == 0) {
		
		while (m_text[m_currentChar] != separatingToken && m_currentChar < m_text.size()) {
			retval.push_back(m_text[m_currentChar]);
			m_currentChar++;
		}

	}
	else {
		m_currentChar++;
		while (m_text[m_currentChar] != separatingToken && m_currentChar < m_text.size()) {
			retval.push_back(m_text[m_currentChar]);
			m_currentChar++;
		}
	}
	return retval;
}

Token Lexer::getNextToken() const noexcept
{
	return m_tokens[m_currentToken++];
}

void Lexer::m_addLiteral(const std::string &contents, Token type)
{
	m_literals[m_tokens.size()] = contents;
	m_tokens.push_back(type);
}

std::string Lexer::getLiteral(size_t position) const
{
	return m_literals.at(position);
}

std::string Lexer::getLiteralTokPosMinus() const
{
	return m_literals.at(m_currentToken-1);
}

std::string Lexer::getTagStringTokPosMinus() const
{
	return m_tags.at(m_currentToken-1);
}

size_t Lexer::getCurrentTokenPosition() const noexcept
{
	return m_currentToken;
}

std::string Lexer::getTagString(size_t position)const
{
	return m_tags.at(position);
}

bool tokenIsLiteralOrTag(Token token)
{
	return (token == Token::tagTok || tokenIsLiteral(token));
}

bool tokenIsLiteral(Token token)
{
	return (token == Token::floatLiteralTok || token == Token::intLiteralTok || token == Token::stringLiteralTok || token == Token::boolLiteralTok);
}

bool tokenIsOperand(Token token)
{
	return (tokenIsLiteralOrTag(token) || token == Token::resultTok || token == Token::itTok);
}
