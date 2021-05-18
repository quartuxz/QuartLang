#pragma once
#include <vector>
#include <sstream>
#include <string>

#define MUST_TEST_ISHLENG

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
	virtual bool m_doTest() = 0;
public:
	Test(const std::string& testName, const std::string &testTypeName = "Test");
	bool doTest();
	void displayResult()const noexcept;
	template<typename T>
	static bool doTests(std::vector<T>& tests) {
		bool retVal = true;
		for (size_t i = 0; i < tests.size(); i++)
		{
			try{
				if (!tests[i].doTest()) {
					retVal = false;
				}
			}
			catch(...){
				tests[i].displayResult();
				throw;
			}
			tests[i].displayResult();
		}
		return retVal;
	}
	virtual ~Test();
};
