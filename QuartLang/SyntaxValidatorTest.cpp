#include "SyntaxValidatorTest.h"
#include <iostream>

std::vector<SyntaxValidatorTest> syntaxValudatorTests = {
	SyntaxValidatorTest("ArithmeticExpressionTest1",{Token::addTok, Token::tagTok, Token::intLiteralTok, Token::endTok},validateResult(true)),
	SyntaxValidatorTest("ArithmeticExpressionTest2",{Token::addTok, Token::tagTok, Token::addTok, Token::endTok},validateResult(false,"",2)),
	SyntaxValidatorTest("callFunctionTest1",{Token::callFunctionTok, Token::tagTok, Token::intLiteralTok, Token::tagTok, Token::endTok},validateResult(true)),
	SyntaxValidatorTest("callFunctionTest2",{Token::callFunctionTok, Token::tagTok, Token::intLiteralTok, Token::tagTok, Token::resultTok, Token::tagTok, Token::endTok},validateResult(true)),
	SyntaxValidatorTest("callFunctionTest3",{Token::callFunctionTok, Token::tagTok, Token::intLiteralTok, Token::boolLiteralTok, Token::endTok},validateResult(false, "", 3)),
	SyntaxValidatorTest("callFunctionTest4",{Token::callFunctionTok, Token::endBlockTok, Token::intLiteralTok, Token::charLiteralTok, Token::endTok},validateResult(false,"",1)),
	SyntaxValidatorTest("declareFunctionTest1",{Token::declareTok, Token::functionTok, Token::tagTok,Token::finallyTok,Token::endBlockTok,Token::endTok},validateResult(true)),
	SyntaxValidatorTest("declareFunctionTest2",{Token::declareTok, Token::functionTok, Token::tagTok,Token::endTok},validateResult(false,"",3)),
	SyntaxValidatorTest("declareFunctionTest3",{Token::declareTok, Token::functionTok, Token::tagTok, Token::declareTok, Token::functionTok, Token::tagTok, Token::endTok},validateResult(false,"",6)),
	SyntaxValidatorTest("declareFunctionTest4",{Token::declareTok, Token::functionTok, Token::tagTok, Token::declareTok, Token::functionTok, Token::tagTok, Token::finallyTok, Token::endBlockTok, Token::endTok},validateResult(false,"",8)),
	SyntaxValidatorTest("declareFunctionTest5",{Token::declareTok, Token::functionTok, Token::tagTok, Token::declareTok, Token::functionTok, Token::tagTok, Token::finallyTok, Token::endBlockTok, Token::finallyTok, Token::endBlockTok, Token::endTok},validateResult(true)),
	SyntaxValidatorTest("ifTest",{Token::ifTok, Token::boolLiteralTok,Token::finallyTok,Token::endBlockTok, Token::endTok},validateResult(true)),
	SyntaxValidatorTest("emptyFunctionTest",{Token::declareTok,Token::functionTok,Token::tagTok,Token::finallyTok,Token::endBlockTok,Token::endTok},validateResult(true))
};


bool SyntaxValidatorTest::m_doTest()
{


	Logger logger;
	logger.toggleLogging(false);

	SyntaxValidator synVal(&logger,m_tokens,defaultProductions);
	validateResult res = synVal.validate();


	bool retVal = (m_expected == res);

	if (retVal) {
		m_passTestType = passTestType::success;
	}
	else {
		m_passTestType = passTestType::failure;
	}

	//std::cout << res.getString();

	return retVal;
}

SyntaxValidatorTest::SyntaxValidatorTest(const std::string& testName, const std::vector<Token>& tokens, validateResult expected):
	Test(testName,"SyntaxValidatorTest"),
	m_tokens(tokens),
	m_expected(expected)
{
}

SyntaxValidatorTest::~SyntaxValidatorTest()
{
}

bool doSyntaxValidatorTests()
{
	return Test::doTests(syntaxValudatorTests);
}
