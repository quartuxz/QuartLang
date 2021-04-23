#include <fstream>
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


	//then we define all other tokens
	{"declare", Token::declareTok},
	{"print", Token::printTok},
	{},
	{}




};



Parser::Parser(std::string filename, bool isFileTrueIsCodeFalse) {
	if (isFileTrueIsCodeFalse) {
		m_bindFile(filename);
	}
	else {
		m_text = filename;
	}
	m_tokenize();
}

void Parser::m_bindFile(std::string fileName)
{

}

void Parser::m_tokenize()
{
	//we reset the previous tokens.
	m_currentToken = 0;
	m_tokens.clear();
	bool readingString = false;
	std::string currentTokenString;
	for (size_t i = 0; i < m_text.size();++i) {

		//we detect a string literal
		if (m_text[i] == '\"' || readingString) {
			currentTokenString.push_back(m_text[i]);
			//we detect the end of a string literal and proceed to save it.
			if (m_text[i] == '\"' && !readingString) {
				readingString = false;
				m_literals[m_tokens.size()] = currentTokenString;
				m_tokens.push_back(Token::stringLiteralTok);
			}
			else {
				readingString = true;
			}

		}
		//we detect a new token, so the token is recognized and then the string is reset.
		else if (m_text[i] == ' ') {
			//if the token is in the list, parse it
			if (m_matches.find(currentTokenString)!=m_matches.end()) {
				m_tokens.push_back(m_matches.at(currentTokenString));
				
			}
			//if the token is not in the list, it is a tag
			else {
				m_tags[m_tokens.size()] = currentTokenString;
				m_tokens.push_back(Token::tagTok);
			}
			//clear the string for the next token
			currentTokenString.clear();
		}
		//the white space is not detected, the token is not final, we continue reading.
		else{
			currentTokenString.push_back(m_text[i]);
		}
	}
	bool onlyLambda = true;
	//we clean the tokens from any lambdaTokens if there are any other tokens other than lambda
	for (auto iter = m_tokens.begin(); iter != m_tokens.end(); ++iter) {
		//we delete the lambda token, checking also if all are lambda tokens, so that we can
		//compress/combine them into a single one.
		if ((*iter) == Token::lambdaTok) {
			m_tokens.erase(iter);
		}
	}
	//we insert a lambda token to signify an empty word for the whole program.
	if (m_tokens.empty()) {
		m_tokens.push_back(Token::lambdaTok);
	}

}

Token Parser::getNextToken() const noexcept
{
	return m_tokens[m_currentToken++];
}

size_t Parser::getCurrentTokenPosition() const noexcept
{
	return m_currentToken;
}

std::string Parser::getTagString(size_t position)const noexcept
{
	return m_tags.at(position);
}
