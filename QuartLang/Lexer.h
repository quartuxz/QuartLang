#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "DictionaryLexer.h"

#include "Token.h"


#include "Logger.h"



bool tokenIsLiteralOrTag(Token token);
bool tokenIsLiteral(Token token);
bool tokenIsOperand(Token token);


//tokenizes code
class Lexer
{
private:

	const DictionaryLexer *m_dictionaryLexer;

	Logger* m_logger;

	//we define all token entries for parsing
	static const std::map<std::string, Token> m_matches;

	std::string m_text;
	std::vector<Token> m_tokens;
	std::map<size_t, std::string> m_tags;
	std::map<size_t, std::string> m_literals;

	std::vector<std::string> m_preliterals;

	mutable size_t m_currentToken = 0;


	size_t m_currentChar = 0;

	std::string m_readNextWord(char separatingToken = ' ');

	void m_addLiteral(const std::string& contents, Token type);

	//reads and saves a file to the text buffer for tokenizing.
	void m_bindFileOrCode(std::string fileNameOrCode, bool isFileTrueIsCodeFalse = true);

	//converts the whole text into individual tokens
	void m_tokenize();

public:

	Lexer(std::string filenameOrCode, const DictionaryLexer *dict,Logger *logger,bool isFileTrueIsCodeFalse = true);


	std::vector<Token> getTokens()const noexcept;

	//returns the current token for reading sequentially
	Token getNextToken()const noexcept;
	size_t getCurrentTokenPosition()const noexcept;


	std::string getLiteral(size_t position)const;

	std::string getLiteralTokPosMinus()const;
	std::string getTagStringTokPosMinus()const;

	std::string getTagString(size_t position)const;
	

	const DictionaryLexer* getDictionaryLexer()const noexcept;

};

