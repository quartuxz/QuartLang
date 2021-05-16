#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>

#include "Token.h"


#include "Logger.h"



bool tokenIsLiteralOrTag(Token token);
bool tokenIsLiteral(Token token);



//Only parses the files, it does not verify a grammar.
class Parser
{
private:

	Logger* m_logger;

	//we define all token entries for parsing
	static const std::map<std::string, Token> m_matches;

	std::string m_text;
	std::vector<Token> m_tokens;
	std::map<size_t, std::string> m_tags;
	std::map<size_t, std::string> m_literals;

	mutable size_t m_currentToken = 0;


	size_t m_currentChar = 0;

	std::string m_readNextWord(char separatingToken = ' ');

	void m_addLiteral(const std::string& contents, Token type);

	//reads and saves a file to the text buffer for tokenizing.
	void m_bindFileOrCode(std::string fileNameOrCode, bool isFileTrueIsCodeFalse = true);

	//converts the whole text into individual tokens
	void m_tokenize();

public:

	Parser(std::string filenameOrCode, Logger *log,bool isFileTrueIsCodeFalse = true);


	//returns the current token for reading sequentially
	Token getNextToken()const noexcept;
	size_t getCurrentTokenPosition()const noexcept;


	std::string getLiteral(size_t position)const;

	std::string getLiteralTokPosMinus()const;
	std::string getTagStringTokPosMinus()const;

	std::string getTagString(size_t position)const;
	
};

