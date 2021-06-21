#include "allTests.h"
#include "wide_tests.h"
#include "LexerTest.h"
#include "SyntaxValidatorTest.h"

bool doAllTests()
{
	return  doLexerTests() && doSyntaxValidatorTests() && doWideTests();
}
