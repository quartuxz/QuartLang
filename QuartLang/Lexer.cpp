#include <fstream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <ctype.h>
#include <locale>

#include "Lexer.h"




Lexer::Lexer(std::string filenameOrCode, const DictionaryLexer* dict, Logger* logger, bool isFileTrueIsCodeFalse):
	m_logger(logger),
	m_dictionaryLexer(dict)
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

	for (auto x : m_literals) {
		logger->log("Lexer, literals",x.second);
	}
	for (auto x : m_tags) {
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

	std::stringstream nextLiteral;
	bool readingLiteral = false;
	char delimiter;

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '\"' || str[i] == '\"') {
			
			//we finish reading the literal
			if (readingLiteral) {
				if (str[i] == delimiter) {
					readingLiteral = false;
					m_preliterals.push_back(nextLiteral.str());
					nextLiteral.str("");
				}

			}
			else {
				readingLiteral = true;
				delimiter = str[i];
			}


		}
		else if(readingLiteral){
			nextLiteral << str[i];
		}
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

	size_t currentPreliteral = 0;

	std::string currentTokenString;
	size_t i = 0;
	while(true) {
		currentTokenString = m_readNextWord();
		//we encounter the end of file string which is an empty string
		if (currentTokenString == "") {
			break;
		}
		//we encounter a string start
		if (currentTokenString[0] == '\"' || currentTokenString[0] == '\'') {
			char delimiter = currentTokenString[0];

			if (currentTokenString.back() != delimiter) {
				std::string nextWord;
				do {
					nextWord = m_readNextWord();

				}while (nextWord.back() != delimiter);
			}

			std::string finalStr = m_preliterals[currentPreliteral++];

			if (finalStr.size() == 1) {
				m_addLiteral(finalStr, Token::charLiteralTok);
			}else {
				m_addLiteral(finalStr, Token::stringLiteralTok);
			}

		}
		//finding a integer or a floating point
		else if (isdigit(currentTokenString[0]) || currentTokenString[0] == '-') {
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
			//we find the token in the list if there is one that matches
			if (m_dictionaryLexer->checkIfStringHasToken(currentTokenString)) {
				Token tok = m_dictionaryLexer->getTokenForString(currentTokenString);
				if (tok != Token::lambdaTok) {
					m_tokens.push_back(m_dictionaryLexer->getTokenForString(currentTokenString));
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

std::vector<Token> Lexer::getTokens() const noexcept
{
	return m_tokens;
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

const DictionaryLexer* Lexer::getDictionaryLexer() const noexcept
{
	return m_dictionaryLexer;
}

bool tokenIsLiteralOrTag(Token token)
{
	return (token == Token::tagTok || tokenIsLiteral(token));
}

bool tokenIsLiteral(Token token)
{
	return (token == Token::floatLiteralTok || token == Token::intLiteralTok || token == Token::stringLiteralTok || token == Token::boolLiteralTok || token == Token::charLiteralTok);
}

bool tokenIsOperand(Token token)
{
	return (tokenIsLiteralOrTag(token) || token == Token::resultTok || token == Token::itTok);
}
