#include "wide_tests.h"
#include "Lexer.h"
#include "Parser.h"
#include "Engine.h"
#include "Ishleng.h"

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
WideTest("itTest","it_test.txt","10"),
WideTest("flipTest","flip_test.txt","1-20-3.5"),
WideTest("functionTest","function_test.txt","0th step, 1th step, 2th step, 3th step, total steps: 4"),
WideTest("multiprocessingFibonacciTest","multiprocessing_fibonacci_test.txt","...1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, "),
WideTest("includeAndBindingTest","include_and_bindings_test.txt","5xx2.5"),
WideTest("importTest","import_test.txt","8")
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



	DictionaryLexer dict("words_alpha.txt", m_matches);

	Ishleng ishleng(&logger, &dict, m_testFile);
	ishleng.lex();
	ishleng.parse();
	ishleng.run();

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
