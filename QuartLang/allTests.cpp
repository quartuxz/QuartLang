#include "allTests.h"
#include "wide_tests.h"
#include "LexerTest.h"

bool doAllTests()
{
	return doWideTests() && doLexerTests();
}
