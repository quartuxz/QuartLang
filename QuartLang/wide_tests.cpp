#include "wide_tests.h"
#include "Lexer.h"
#include "Parser.h"
#include "Engine.h"


std::vector<WideTest> wideTests = {
WideTest("helloWorldTest","hello_world_test.txt","hello world"),
WideTest("variableTest","variables_test.txt","7.6"),
WideTest("doubleSetTest","double_set_test.txt","hello"),
WideTest("arithmeticTest","arithmetic_test.txt","3"),
WideTest("resultTest","result_test.txt","8"),
WideTest("conditionalTest","conditional_test.txt","012345678910"),
WideTest("evaluationTest","evaluation_test.txt","011"),
WideTest("verboseVariableTest","verbose_variable_test.txt","5"),
WideTest("appendAndReferTest","append_and_refer_test.txt","second"),
WideTest("itTest","it_test.txt","10")

};

bool doWideTests()
{
	return Test::doTests(wideTests);
}

WideTest::WideTest(const std::string& testName, const std::string& testFile, const std::string& required):
    Test(testName, "Wide test"),
    m_testFile(testFile),
	m_required(required)
{
}

bool WideTest::m_doTest()
{
	bool retval = true;
	Logger logger;
	logger.toggleLogging(false);
	Lexer Lexer(m_testFile, &logger);
	Parser parser(&Lexer, &logger);
	Engine engine(parser.getProgram(), &logger);
	engine.run();

	if (testOutput.str() == m_required) {
		m_passTestType = passTestType::success;
	}
	else {
		retval = false;
		m_passTestType = passTestType::failure;
	}

	testOutput.str(std::string());
	testOutput.clear();
    return retval;
}
