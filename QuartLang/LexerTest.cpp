#include "LexerTest.h"
#include "Lexer.h"
#include "Logger.h"


std::vector<LexerTest> LexerTests = {
	LexerTest("basicTest", "   \n\n\n \t\t declare", {Token::declareTok, Token::endTok},{}),
	LexerTest("ManyTokens", "   \n\n\n \t\n\t\n deCLarE \t as \n giving an to  \t aDd \n Subtract   \n VARNAME \t -200", {Token::declareTok, Token::addTok, Token::subtractTok, Token::tagTok, Token::intLiteralTok, Token::endTok},{"VARNAME", "-200"})
};


LexerTest::LexerTest(const std::string& testName, const std::string& testInput, const std::vector<Token>& expected, const std::vector<std::string>& expectedLiteralsAndTags):
	Test(testName, "Lexer Test"),
	m_testInput(testInput),
	m_expected(expected),
	m_expectedLiteralsAndTags(expectedLiteralsAndTags)
{
}

bool LexerTest::m_doTest()
{
	Logger logger;
	logger.toggleLogging(false);

	DictionaryLexer dict("words_alpha.txt",m_matches);

	Lexer Lexer(m_testInput,&dict,&logger,false);
	Token currentToken;
	size_t i = 0;
	size_t litAndTagCurrent = 0;
	do {
		currentToken = Lexer.getNextToken();
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
				if (m_expectedLiteralsAndTags[litAndTagCurrent] != Lexer.getLiteralTokPosMinus()) {
					m_passTestType = passTestType::failure;
					return false;
				}
			}
			else {
				if (m_expectedLiteralsAndTags[litAndTagCurrent] != Lexer.getTagStringTokPosMinus()) {
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

bool doLexerTests()
{
	return Test::doTests(LexerTests);
}
