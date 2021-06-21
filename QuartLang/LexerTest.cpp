#include "LexerTest.h"
#include "Lexer.h"
#include "Logger.h"


std::vector<LexerTest> LexerTests = {
	LexerTest("lambdaTest", "\n\n",{Token::lambdaTok, Token::endTok} ,{}),
	LexerTest("basicTest", "   \n\n\n \t\t declare", {Token::declareTok, Token::endTok},{}),
	LexerTest("ManyTokens", "   \n\n\n \t\n\t\n deCLarE \t as \n giving an to  \t aDd \n Subtract   \n VARNAME \t -200", {Token::declareTok, Token::addTok, Token::subtractTok, Token::tagTok, Token::intLiteralTok, Token::endTok},{"VARNAME", "-200"}),
	LexerTest("LineTest", " declare variable var1 \n declare \n function ", {Token::declareTok, Token::variableTok, Token::tagTok, Token::declareTok,Token::functionTok, Token::endTok},{"var1"},std::make_optional(std::map<size_t,size_t>({ {0,1},{1,1},{2,1},{3,2},{4,3}, {5,4}})))
};


LexerTest::LexerTest(const std::string& testName, const std::string& testInput, const std::vector<Token>& expected, const std::vector<std::string>& expectedLiteralsAndTags, const std::optional<std::map<size_t,size_t>> & tokensForLines):
	Test(testName, "Lexer Test"),
	m_testInput(testInput),
	m_expected(expected),
	m_expectedLiteralsAndTags(expectedLiteralsAndTags)
{
	if (tokensForLines.has_value()) {
		m_expectedTokensForLines = tokensForLines.value();
		m_doesLineTest = true;
	}
	else {
		m_doesLineTest = false;
	}
}

bool LexerTest::m_doTest()
{
	Logger logger;
	logger.toggleLogging(false);

	DictionaryLexer dict("words_alpha.txt",m_matches);

	Lexer lexer(m_testInput,&dict,&logger,false);
	Token currentToken;
	size_t i = 0;
	size_t litAndTagCurrent = 0;
	do {
		currentToken = lexer.getNextToken();
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
				if (m_expectedLiteralsAndTags[litAndTagCurrent] != lexer.getLiteralTokPosMinus()) {
					m_passTestType = passTestType::failure;
					return false;
				}
			}
			else {
				if (m_expectedLiteralsAndTags[litAndTagCurrent] != lexer.getTagStringTokPosMinus()) {
					m_passTestType = passTestType::failure;
					return false;
				}
			}
			litAndTagCurrent++;

		}
		i++;
	} while (currentToken != Token::endTok);

	if (m_doesLineTest) {
		if (m_expectedTokensForLines != lexer.getTokenPosToLine() ) {
			m_passTestType = passTestType::failure;
			return false;
		}
	}


	m_passTestType = passTestType::success;
	return true;
}

bool doLexerTests()
{
	return Test::doTests(LexerTests);
}
