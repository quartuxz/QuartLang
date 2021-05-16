#include "wide_tests.h"
#include "Parser.h"
#include "Recognizer.h"
#include "Engine.h"


std::vector<WideTest> wideTests = {
WideTest("helloWorldTest","hello_world_test.txt","hello world"),
WideTest("variableTest","variables_test.txt","7.6"),
WideTest("doubleSetTest","double_set_test.txt","hello"),
WideTest("arithmeticTest","arithmetic_test.txt","3"),
WideTest("resultTest","result_test.txt","8")

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
	Parser parser(m_testFile, &logger);
	Recognizer recognizer(&parser, &logger);
	Engine engine(recognizer.getProgram(), &logger);
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
