#pragma once
#include <vector>
#include "tests.h"
#include "Token.h"



class LexerTest: public Test
{
private:
	std::string m_testInput;
	std::vector<Token> m_expected;
	std::vector<std::string> m_expectedLiteralsAndTags;
	bool m_doTest()override;
public:
	LexerTest(const std::string& testName, const std::string& testInput, const std::vector<Token>& expected, const std::vector<std::string>& expectedLiteralsAndTags);


};

bool doLexerTests();