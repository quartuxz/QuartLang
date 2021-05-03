#include <fstream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <ctype.h>
#include <locale>

#include "Parser.h"



const std::map<std::string, Token> Parser::m_matches = {
	//we first define all lambda tokens, those that are not parsed and also arent tags
	{"a", Token::lambdaTok},
	{"named", Token::lambdaTok},
	{"as", Token::lambdaTok},
	{"an", Token::lambdaTok},
	{"to", Token::lambdaTok},
	{"the", Token::lambdaTok},
	{"of", Token::lambdaTok},
	{"for", Token::lambdaTok},
	{"execution", Token::lambdaTok},
	{"giving", Token::lambdaTok},


	//then we define all other tokens
	{"declare", Token::declareTok},
	{"call", Token::callFunctionTok}




};



Parser::Parser(std::string filename, Logger *logger,bool isFileTrueIsCodeFalse):
	m_logger(logger)
{

	if (isFileTrueIsCodeFalse) {
		m_bindFile(filename);
	}
	else {
		m_text = filename;
	}
	m_tokenize();
#ifdef MUST_LOG
	
	logger->log("Parser",m_text);

	
	std::stringstream sstream;

	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		sstream << GetStringToken(m_tokens[i]) << " ";
	}
	sstream << "\n" << "\n";
	logger->log("Parser",sstream.str());

	for (auto x:m_literals) {
		logger->log("Parser, literals",x.second);
	}
	for (auto x:m_tags) {
		logger->log("Parser, tags",x.second);
	}

#endif
}

void Parser::m_bindFile(std::string fileName)
{
	std::ifstream t(fileName);
	std::string str((std::istreambuf_iterator<char>(t)),
	std::istreambuf_iterator<char>());
	//we remove newlines and excess whitespaces
	bool wasWhiteSpace = false;
	for (size_t i = 0; i < str.size(); i++) {

		if (str[i] != '\n' && !(str[i] == ' ' && wasWhiteSpace)) {
			m_text.push_back(str[i]);
		}

		if (str[i] != ' ') {
			wasWhiteSpace = false;
		}
	}
	m_text.push_back(' ');
}

void Parser::m_tokenize()
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
		if (currentTokenString[0] == '\"') {
			std::stringstream ss;
			ss << currentTokenString;

			if (currentTokenString.back() != '\"') {
				std::string nextChunk;
				do {
					nextChunk = m_readNextWord();

					ss << nextChunk;
				}while (nextChunk.back() != '\"');
			}
			std::string tempStr = ss.str();
			std::string finalStr(++tempStr.begin(), --tempStr.end());
			m_addLiteral(finalStr, Token::stringLiteralTok);
		}
		//finding a integer or a floating point
		else if (isdigit(currentTokenString[0])) {
			m_logger->log("Parser",currentTokenString);
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
		else {
			if (m_matches.find(currentTokenString) != m_matches.end()) {
				Token tok = m_matches.at(currentTokenString);
				if (tok != Token::lambdaTok) {
					m_tokens.push_back(m_matches.at(currentTokenString));
				}
				
			}
			//if the token is not in the list, it is a tag
			else {
				m_tags[m_tokens.size()] = currentTokenString;
				m_tokens.push_back(Token::tagTok);
			}
		}

		m_logger->log("Parser internals", currentTokenString);
	}
	


	//we insert a lambda token to signify an empty word for the whole program.
	if (m_tokens.empty()) {
		m_tokens.push_back(Token::lambdaTok);
	}
	m_tokens.push_back(Token::endTok);
}

std::string Parser::m_readNextWord(char separatingToken)
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

Token Parser::getNextToken() const noexcept
{
	return m_tokens[m_currentToken++];
}

void Parser::m_addLiteral(const std::string &contents, Token type)
{
	m_literals[m_tokens.size()] = contents;
	m_tokens.push_back(type);
}

std::string Parser::getLiteral(size_t position) const noexcept
{
	return m_literals.at(position);
}

size_t Parser::getCurrentTokenPosition() const noexcept
{
	return m_currentToken;
}

std::string Parser::getTagString(size_t position)const noexcept
{
	return m_tags.at(position);
}

bool tokenIsLiteralOrTag(Token token)
{
	return (token == Token::tagTok || token == Token::floatLiteralTok || token == Token::intLiteralTok || token == Token::stringLiteralTok);
}
