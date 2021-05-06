#pragma once
#include <vector>
#include <string>
#include "tests.h"




class WideTest : public Test {
private:
	std::string m_testFile;
	std::string m_required;
public:
	WideTest(const std::string &testName, const std::string &testFile, const std::string &required);
	bool doTest()override;
};


bool doWideTests();