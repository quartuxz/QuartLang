#pragma once
#include "Token.h"
#include "SyntaxValidator.h"
#include "tests.h"





class SyntaxValidatorTest : public Test
{
private:
	bool m_doTest()override;
	std::vector<Token> m_tokens;
	validateResult m_expected;
public:
	SyntaxValidatorTest(const std::string& testName, const std::vector<Token> &tokens, validateResult expected);
	~SyntaxValidatorTest();
};

bool doSyntaxValidatorTests();

