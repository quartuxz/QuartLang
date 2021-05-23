#include "DictionaryLexer.h"
#include <fstream>
#include <iostream>


#include "tests.h"

std::map<std::string, std::map<std::string, Token>*> DictionaryLexer::m_finalTokenDicts;

std::map<std::string, Token>* DictionaryLexer::m_makeTokenDict(const std::string& file, const std::map<std::string, Token>& reservedKeyWords)
{
	std::map<std::string, Token>* retval = new std::map<std::string, Token>();
	std::string str;
	std::fstream t(file);
	str = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	std::string currentString;

	size_t printIndex = 0;
#ifndef MUST_TEST_ISHLENG
	std::cout << "LOADING DICTIONARY..." ;
#endif
	for (size_t i = 0; i < str.size(); i++)
	{

		if (str[i] == '\n') {
			auto reserved = reservedKeyWords.find(currentString);
			if (reserved != reservedKeyWords.end()) {
				retval->insert(*reserved);
			}
			else {
				retval->insert({ currentString, Token::lambdaTok });
			}
			currentString = "";
		}else{
			currentString.push_back(str[i]);
		}
	}
#ifndef MUST_TEST_ISHLENG
	std::cout << "DONE" << std::endl;
	std::cout << "--- --- ---" << std::endl;
#endif
	return retval;
}

DictionaryLexer::DictionaryLexer(const std::string& file, const std::map<std::string, Token>& reservedKeyWords) :
	m_file(file)

{
	if (m_finalTokenDicts.find(file) == m_finalTokenDicts.end()) {
		m_finalTokenDicts[file] = m_makeTokenDict(file, reservedKeyWords);
	}

}

const std::map<std::string, Token>* DictionaryLexer::getFinalTokenDict() const noexcept
{
	return m_finalTokenDicts[m_file];
}

Token DictionaryLexer::getTokenForString(const std::string& str) const
{
	return m_finalTokenDicts[m_file]->at(str);
}

bool DictionaryLexer::checkIfStringHasToken(const std::string& str) const noexcept
{
	return (m_finalTokenDicts[m_file]->find(str) != m_finalTokenDicts[m_file]->end());
}

void DictionaryLexer::cleanupDictionaries()
{
	for (auto x : m_finalTokenDicts) {
		delete x.second;
	}
}

DictionaryLexer::~DictionaryLexer()
{
	
}
