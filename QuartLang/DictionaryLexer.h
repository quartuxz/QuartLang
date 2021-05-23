#pragma once
#include <string>
#include <map>
#include "Token.h"

class DictionaryLexer {
private:
	std::string m_file;
	//creates dynamic memory as return value
	static std::map<std::string, Token> *m_makeTokenDict(const std::string& file, const std::map<std::string, Token> &reservedKeyWords);
	//we cache everything
	static std::map<std::string, std::map<std::string, Token> *> m_finalTokenDicts;
public:
	DictionaryLexer(const std::string& file, const std::map<std::string,Token> &reservedKeyWords);
	
	const std::map<std::string, Token>* getFinalTokenDict()const noexcept;

	Token getTokenForString(const std::string& str)const;
	bool checkIfStringHasToken(const std::string &str)const noexcept;

	static void cleanupDictionaries();

	~DictionaryLexer();
};