#include "DictionaryLexer.h"
#include <fstream>
#include <iostream>


#include "tests.h"



const std::map<std::string, Token> m_matches = {
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
	{"it", Token::itTok},
	{"function", Token::functionTok},
	{"launch", Token::launchTok},
	{"finish", Token::finishTok},
	{"comment", Token::commentTok},
	{"include", Token::includeTok},
	{"bind", Token::bindTok},
	{"open", Token::openTok},
	{"import", Token::importTok}

};

std::string makeLower(const std::string &str) {
	
	std::string retval;


	static const std::map<char, char> upperToLower = {
		{'A','a'},
		{'B','b'},
		{'C','c'},
		{'D','d'},
		{'E','e'},
		{'F','f'},
		{'G','g'},
		{'H','h'},
		{'I','i'},
		{'J','j'},
		{'K','k'},
		{'L','l'},
		{'M','m'},
		{'N','n'},
		{'O','o'},
		{'P','p'},
		{'Q','q'},
		{'R','r'},
		{'S','s'},
		{'T','t'},
		{'U','u'},
		{'V','v'},
		{'W','w'},
		{'X','x'},
		{'Y','y'},
		{'Z','z'}
	};

	
	for (size_t i = 0; i < str.size(); i++)
	{
		char currentChar = str[i];
		if (upperToLower.find(currentChar) != upperToLower.end()) {
			retval.push_back(upperToLower.at(currentChar));
		}
		else {
			retval.push_back(currentChar);
		}
	}
	return retval;
}


std::map<std::string, std::map<std::string, Token>*> DictionaryLexer::m_finalTokenDicts;

std::map<std::string, Token>* DictionaryLexer::m_makeTokenDict(const std::string& file, const std::map<std::string, Token>& reservedKeyWords)
{
	std::map<std::string, Token>* retval = new std::map<std::string, Token>();
	std::string str;
	std::fstream t(file);
	str = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	std::string currentString;

	size_t printIndex = 0;

	//we first insert the reserved keywords in their lowercase form with their tokens
	for (auto x : reservedKeyWords) {
		auto inserted = std::make_pair(makeLower(x.first), x.second);
		retval->insert(inserted);
	}

	//we parse the dictionary for lambda convertible english words
	for (size_t i = 0; i < str.size(); i++)
	{

		if (str[i] == '\n') {

			//we detect a word that is was already inserted by virtue of being reserved
			if (retval->find(currentString) == retval->end()) {
				//we make the words lowercase
				retval->insert({ makeLower(currentString), Token::lambdaTok });
			}
			currentString = "";
		}else{
			currentString.push_back(str[i]);
		}
	}
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
	
	return m_finalTokenDicts[m_file]->at(makeLower(str));
}

bool DictionaryLexer::checkIfStringHasToken(const std::string& str) const noexcept
{
	return (m_finalTokenDicts[m_file]->find(makeLower(str)) != m_finalTokenDicts[m_file]->end());
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
