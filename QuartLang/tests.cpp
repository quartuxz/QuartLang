#include <iostream>
#include "tests.h"

std::stringstream testOutput = std::stringstream();


Test::Test(const std::string& testName, const std::string &testTypeName):
	m_testName(testName),
	m_testTypeName(testTypeName)
{
}

bool Test::doTest()
{
	bool testSuccesful;
	try {
		 testSuccesful = m_doTest();
	}
	catch (...) {
		m_passTestType = passTestType::exceptionThrown;
		throw;
	}

	return testSuccesful;
}

void Test::displayResult() const noexcept
{
	std::cout << "|" << m_testTypeName <<": \"" << m_testName << "\" -> " << makePassTestString(m_passTestType) << "!|" << std::endl;
}

Test::~Test()
{
}

std::string makePassTestString(passTestType psstt)
{
	switch (psstt)
	{
	case passTestType::success:
		return "success";
		break;
	case passTestType::exceptionThrown:
		return "exception thrown";
		break;
	case passTestType::failure:
		return "failure";
		break;
	default:
		break;
	}
}
