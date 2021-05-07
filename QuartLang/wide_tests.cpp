#include "wide_tests.h"
#include "Parser.h"
#include "Recognizer.h"
#include "Engine.h"


std::vector<WideTest> wideTests = {
WideTest("helloWorldTest","hello_world_test.txt","hello world"),
WideTest("variableTest","variables_test.txt","7.6")

};

bool doWideTests()
{
	bool retVal = true;
	for (size_t i = 0; i < wideTests.size(); i++)
	{
		if (!wideTests[i].doTest()) {
			retVal = false;
		}
		wideTests[i].displayResult();
	}
    return retVal;
}

WideTest::WideTest(const std::string& testName, const std::string& testFile, const std::string& required):
    Test(testName, "Wide test"),
    m_testFile(testFile),
	m_required(required)
{
}

bool WideTest::doTest()
{
	bool retval = true;
	try{
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
	}
	catch (...) {
		m_passTestType = passTestType::exceptionThrown;
		throw;
	}
    return retval;
}
