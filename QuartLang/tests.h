#pragma once
#include <vector>
#include <sstream>
#include <string>

//#define MUST_TEST_ISHLENG

extern std::stringstream testOutput;

enum class passTestType {
	notRun,
	success,
	exceptionThrown,
	failure
};

std::string makePassTestString(passTestType psstt);

class Test {
protected:
	std::string m_testName;
	std::string m_testTypeName = "Test"; 
	passTestType m_passTestType = passTestType::notRun;
public:
	Test(const std::string& testName, const std::string &testTypeName = "Test");
	virtual bool doTest() = 0;
	void displayResult()const noexcept;
	virtual ~Test();
};
