#pragma once
#include <string>
#include <vector>
#include <map>


//called tokens, but hey conform an alphabet of symbols that can be used to make
//the language for the program. 
enum class Token {
	lambdaTok,
	declareTok,
	tagTok,
	functionTok,
	variableTok,
	floatLiteralTok,
	intLiteralTok,
	stringLiteralTok,
	printTok  
};



//Only parses the files, it does not verify a grammar.
class Parser
{
private:

	//we define all token entries for parsing
	static const std::map<std::string, Token> m_matches;

	std::string m_text;
	std::vector<Token> m_tokens;
	std::map<size_t, std::string> m_tags;
	std::map<size_t, std::string> m_literals;

	mutable size_t m_currentToken = 0;


	//reads and saves a file to the text buffer for tokenizing.
	void m_bindFile(std::string fileName);

	//converts the whole text into individual tokens
	void m_tokenize();

public:

	Parser(std::string filenameOrCode, bool isFileTrueIsCodeFalse = true);




	//returns the current token for reading sequentially
	Token getNextToken()const noexcept;
	size_t getCurrentTokenPosition()const noexcept;

	std::string getTagString(size_t position)const noexcept;
	

};
