#include "ParserTest.h"
#include "Parser.h"
#include "Logger.h"


std::vector<ParserTest> parserTests = {
	ParserTest("basicTest", "   \n\n\n \t\t declare", {Token::declareTok, Token::endTok},{}),
	ParserTest("ManyTokens", "   \n\n\n \t\n\t\n declare \t as \n giving an to  \t add \n subtract   \n VARNAME \t 200", {Token::declareTok, Token::addTok, Token::subtractTok, Token::tagTok, Token::intLiteralTok, Token::endTok},{"VARNAME", "200"})
};


ParserTest::ParserTest(const std::string& testName, const std::string& testInput, const std::vector<Token>& expected, const std::vector<std::string>& expectedLiteralsAndTags):
	Test(testName, "Parser Test"),
	m_testInput(testInput),
	m_expected(expected),
	m_expectedLiteralsAndTags(expectedLiteralsAndTags)
{
}

bool ParserTest::m_doTest()
{
	Logger logger;
	logger.toggleLogging(false);
	Parser parser(m_testInput,&logger,false);
	Token currentToken;
	size_t i = 0;
	size_t litAndTagCurrent = 0;
	do {
		currentToken = parser.getNextToken();
		if (m_expected[i] != currentToken) {
			m_passTestType = passTestType::failure;
			return false;
		}

		if (tokenIsLiteralOrTag(currentToken)) {
			if (m_expectedLiteralsAndTags.empty()) {
				m_passTestType = passTestType::failure;
				return false;
			}
			if (tokenIsLiteral(currentToken)) {
				if (m_expectedLiteralsAndTags[litAndTagCurrent] != parser.getLiteralTokPosMinus()) {
					m_passTestType = passTestType::failure;
					return false;
				}
			}
			else {
				if (m_expectedLiteralsAndTags[litAndTagCurrent] != parser.getTagStringTokPosMinus()) {
					m_passTestType = passTestType::failure;
					return false;
				}
			}
			litAndTagCurrent++;

		}
		i++;
	} while (currentToken != Token::endTok);
	m_passTestType = passTestType::success;
	return true;
}

bool doParserTests()
{
	return Test::doTests(parserTests);
}
