#include <fstream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

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



Parser::Parser(std::string filename, Logger *log,bool isFileTrueIsCodeFalse) {

	if (isFileTrueIsCodeFalse) {
		m_bindFile(filename);
	}
	else {
		m_text = filename;
	}
	m_tokenize();
#ifdef MUST_LOG
	
	log->log("Parser",m_text);

	
	std::stringstream sstream;

	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		sstream << GetStringToken(m_tokens[i]) << " ";
	}
	sstream << "\n" << "\n";
	log->log("Parser",sstream.str());

	for (auto x:m_literals) {
		log->log("Parser, literals",x.second);
	}
	for (auto x:m_tags) {
		log->log("Parser, tags",x.second);
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

			m_literals[m_tokens.size()] = ss.str();
			m_tokens.push_back(Token::stringLiteralTok);
		}
		else {
			if (m_matches.find(currentTokenString) != m_matches.end()) {
				m_tokens.push_back(m_matches.at(currentTokenString));
			}
			//if the token is not in the list, it is a tag
			else {
				m_tags[m_tokens.size()] = currentTokenString;
				m_tokens.push_back(Token::tagTok);
			}
		}

		m_logger->log("Parser internals", currentTokenString);
	}
	
	m_tokens.push_back(Token::endTok);
	return;


	//DEPRECATED CODE TO REMOVE LAMBDAS(NEED TO FIX INDICES FOR LITERALS AND TAGS)
	//CLEARLY THE LOOP IS NOT WORKO
	//this loop breaks every structured programming principle.
	while (!m_tokens.empty()) {
		bool shallContinue = false;
		//we clean the tokens from any lambdaTokens if there are any other tokens other than lambda
		for (auto iter = m_tokens.begin(); iter != m_tokens.end(); ++iter) {
			//we delete the lambda token, checking also if all are lambda tokens, so that we can
			//compress/combine them into a single one.
			if ((*iter) == Token::lambdaTok) {
				m_tokens.erase(iter);
				shallContinue = true;
				break;
			}
		}
		if (shallContinue) {
			continue;
		}
		break;
	}

	//we insert a lambda token to signify an empty word for the whole program.
	if (m_tokens.empty()) {
		m_tokens.push_back(Token::lambdaTok);
	}

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
