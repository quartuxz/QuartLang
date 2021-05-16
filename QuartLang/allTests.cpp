#include "allTests.h"
#include "wide_tests.h"
#include "ParserTest.h"

bool doAllTests()
{
	return doWideTests() && doParserTests();
}
