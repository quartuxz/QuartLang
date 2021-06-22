#pragma once
#include <map>
#include <vector>
#include <optional>
#include <string>
#include "Token.h"
#include "Logger.h"

enum class nonTerminal {
	startNT, blockNT, allNT,
	operandNT, numericalOperandNT, tagResultOrItNT, 
	arithmeticOpNT, evaluateOpNT, comparisonNT, functionCallOpNT, 
	argumentsNT, arguments2NT, declareNT, finallyOpNT,  ifNT,
	numericalComparisonNT, numericalComparison2NT, numericalComparisonOptNT,
	setOpNT, tagOrStringLiteralNT, openOpNT, referOpNT, tagNT,
	appendOpNT, appendOpSecondOperandNT, bindOpNT, literalNT, tagOrItNT,
	skip
};

extern std::map<nonTerminal, std::string> nonTerminalToStrings;

struct nonTerminalOrToken {

	nonTerminal nt;
	Token terminal;

	bool trueForTerminal;

	nonTerminalOrToken(nonTerminal p_nt);
	nonTerminalOrToken(Token p_terminal);
};



//to mean an empty word
#define lambda

//
extern std::map<nonTerminal, std::vector< std::vector< nonTerminalOrToken > > > defaultProductions;


struct validateResult {
	bool success;
	size_t line = 0;
	size_t tokenPos;
	std::string message;

	validateResult(bool p_success, std::string p_message = "", size_t p_tokenPos = 0, size_t p_line = 0);

	std::string getString()const;

	bool operator==(const validateResult& rhs);
};


class validationError : public std::exception{
private:
	validateResult m_valRes;
	std::string m_fileName;


	std::string m_message;
public:
	validationError(const validateResult &res, const std::string &fileName);

	const char* what()const override;
};

class SyntaxValidator
{
private:

	std::vector<Token> m_tokens;
	std::map<nonTerminal, std::vector< std::vector< nonTerminalOrToken > > > m_productions;
	std::vector<nonTerminal> m_nexts = {nonTerminal::startNT};

	Logger* m_logger;

public:
	//Matias Chara normal form.
	//productions must be in the form: (Token)*·(nonTerminal)*.
	//non terminal symbols must not have more than 1/ONE production rule with only non-terminals or lambda as their product.
	//this rule must be the first for the nonterminal(this can be fixed, but its not worthwhile)
	//no two rules may start with the same terminal
	SyntaxValidator(Logger *logger,const std::vector<Token> &tokens, const std::map<nonTerminal, std::vector< std::vector< nonTerminalOrToken > > > &prod);
	validateResult validate();
	~SyntaxValidator();
};

