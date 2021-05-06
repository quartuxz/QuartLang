#include <iostream>
#include "tests.h"

std::stringstream testOutput = std::stringstream();


Test::Test(const std::string& testName, const std::string &testTypeName):
	m_testName(testName),
	m_testTypeName(testTypeName)
{
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
		return "exceptionn thrown";
		break;
	case passTestType::failure:
		return "failure";
		break;
	default:
		break;
	}
}
