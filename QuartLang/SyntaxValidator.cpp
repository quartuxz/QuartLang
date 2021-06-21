#include "SyntaxValidator.h"
#include <sstream>
#include <stack>


std::map<nonTerminal, std::string> nonTerminalToStrings = {

    {nonTerminal::startNT, "start"},
    {nonTerminal::blockNT, "block start"},
    {nonTerminal::allNT, "code body"},
    {nonTerminal::operandNT, "operand"},
    {nonTerminal::numericalOperandNT, "numerical operand"},
    {nonTerminal::tagResultOrItNT, "tag result or it"},
    {nonTerminal::arithmeticOpNT, "arithmetic operation"},
    {nonTerminal::evaluateOpNT, "evaluate operation"},
    {nonTerminal::comparisonNT, "comparison"},
    {nonTerminal::functionCallOpNT, "function call operation"},
    {nonTerminal::argumentsNT, "argument"},
    {nonTerminal::arguments2NT, "argument name" },
    {nonTerminal::declareNT, "declare"},
    {nonTerminal::finallyOpNT, "finally operation"},
    {nonTerminal::ifNT, "if"},
    {nonTerminal::numericalComparisonNT, "numerical comparison first part"},
    {nonTerminal::numericalComparison2NT, "numerical comparison second part"},
    {nonTerminal::numericalComparisonOptNT, "numerical comparison optional parameter"},
    {nonTerminal::setOpNT , "set operation"},
    {nonTerminal::tagOrStringLiteralNT, "tag or string literal"},
    {nonTerminal::openOpNT, "open operation"},
    {nonTerminal::referOpNT, "refer operation"},
    {nonTerminal::tagNT, "tag"},
    {nonTerminal::appendOpNT,"append operation"},
    {nonTerminal::appendOpSecondOperandNT, "append operation second operand"},
    {nonTerminal::bindOpNT, "bind operation"},
    {nonTerminal::literalNT, "literal"},
    {nonTerminal::skip, "skip this"}

};


bool blockStart(Token tok) {
    if (tok == Token::ifTok || tok == Token::functionTok) {
        return true;
    }
    return false;
}

bool blockEnd(Token tok) {
    if (tok == Token::finallyTok) {
        return true;
    }
    return false;
}


std::vector<std::pair<std::string,  std::vector< bool (*)(Token)> > > defaultMatchAmounts = {
    //std::make_pair("block starts and ends" ,std::vector<bool (*)(Token)>{blockStart,blockEnd})
};

//like lambda but skips a token
#define skip nonTerminal::skip
//when validating a string, if no terminal string matches and there is an alternative lambda production,
//then no symbol is read, the current input string read symbol is not changed, and the next non-terminal is read
std::map<nonTerminal, std::vector< std::vector< nonTerminalOrToken > > > defaultProductions  =
{
    {nonTerminal::allNT, {
        { skip },
        { Token::addTok, nonTerminal::arithmeticOpNT},
        { Token::subtractTok, nonTerminal::arithmeticOpNT},
        { Token::multiplyTok, nonTerminal::arithmeticOpNT},
        { Token::divideTok , nonTerminal::arithmeticOpNT},
        { Token::evaluateTok, nonTerminal::evaluateOpNT},
        { Token::callFunctionTok, nonTerminal::functionCallOpNT},
        { Token::declareTok, nonTerminal::declareNT},
        //{ Token::finallyTok, nonTerminal::finallyOpNT},
        { Token::ifTok, nonTerminal::ifNT, nonTerminal::blockNT},
        { Token::setTok, nonTerminal::setOpNT},
        { Token::finishTok, nonTerminal::operandNT },
        { Token::importTok, nonTerminal::tagOrStringLiteralNT },
        { Token::openTok, nonTerminal::openOpNT },
        { Token::includeTok, nonTerminal::tagOrStringLiteralNT },
        { Token::flipTok, nonTerminal::operandNT},
        { Token::launchTok, nonTerminal::functionCallOpNT },
        { Token::commentTok, nonTerminal::tagOrStringLiteralNT },
        { Token::referTok, nonTerminal::referOpNT },
        { Token::appendTok, nonTerminal::appendOpNT },
        { Token::bindTok, nonTerminal::bindOpNT }
    }},
    {nonTerminal::blockNT, {
        { nonTerminal::allNT, nonTerminal::blockNT},
        { Token::finallyTok, nonTerminal::finallyOpNT}
    }},
    {nonTerminal::startNT, {
        { nonTerminal::allNT, nonTerminal::startNT},
        //we include lambda to allow for all not enumerated tokens to just fall through and not throw errors, this allows us 
        
        { Token::endTok}
    }},
    {nonTerminal::arithmeticOpNT , { { nonTerminal::numericalOperandNT, nonTerminal::numericalOperandNT} } },
    {nonTerminal::evaluateOpNT , { { nonTerminal::operandNT, nonTerminal::comparisonNT,nonTerminal::operandNT } } },
    {nonTerminal::numericalOperandNT, {
        {nonTerminal::tagResultOrItNT},
        {Token::floatLiteralTok},
        {Token::intLiteralTok}
    }},
    {nonTerminal::tagResultOrItNT, {
        {Token::tagTok},
        {Token::resultTok},
        {Token::itTok}
    }},
    {nonTerminal::operandNT, {
        { nonTerminal::numericalOperandNT },
        {Token::boolLiteralTok},
        {Token::stringLiteralTok},
        {Token::charLiteralTok}
    }},
    {nonTerminal::functionCallOpNT, {
        {Token::tagTok,nonTerminal::argumentsNT},
    }},
    {nonTerminal::argumentsNT,{
        { lambda },
        {Token::boolLiteralTok, nonTerminal::arguments2NT},
        {Token::stringLiteralTok, nonTerminal::arguments2NT},
        {Token::charLiteralTok, nonTerminal::arguments2NT},
        {Token::floatLiteralTok, nonTerminal::arguments2NT},
        {Token::intLiteralTok, nonTerminal::arguments2NT},
        {Token::tagTok, nonTerminal::arguments2NT},
        {Token::resultTok, nonTerminal::arguments2NT},
        {Token::itTok, nonTerminal::arguments2NT}
    }},
    {nonTerminal::arguments2NT, {
        {Token::tagTok, nonTerminal::argumentsNT}
    }},
    {nonTerminal::declareNT, {
        {Token::functionTok, Token::tagTok, nonTerminal::blockNT},
        {Token::variableTok, Token::tagTok}
        
    }},
    {nonTerminal::finallyOpNT, {
        {Token::endBlockTok},
        {Token::repeatTok, nonTerminal::operandNT},
        {Token::giveTok, nonTerminal::operandNT}
    }},
    {nonTerminal::ifNT,{
        {nonTerminal::tagResultOrItNT},
        {Token::boolLiteralTok}
    }},
    {nonTerminal::comparisonNT,{
        {nonTerminal::numericalComparisonNT,nonTerminal::numericalComparison2NT},
        {Token::andTok},
        {Token::orTok},
    }},
    {nonTerminal::numericalComparisonNT,{
        {Token::lessThanTok},
        {Token::moreThanTok},
        {Token::equalToTok}
    }},
    {nonTerminal::numericalComparison2NT,{
        { nonTerminal::numericalComparisonOptNT },
        {Token::orTok, nonTerminal::numericalComparisonNT}
    }},
    {nonTerminal::numericalComparisonOptNT,{
        { lambda },
        {Token::lessThanTok},
        {Token::moreThanTok},
        {Token::equalToTok},
    }},
    {nonTerminal::setOpNT, {
        {Token::tagTok, nonTerminal::operandNT},
        {Token::itTok, nonTerminal::operandNT}
    }},
    {nonTerminal::tagOrStringLiteralNT, {
        {Token::tagTok},
        {Token::stringLiteralTok}
    }},
    { nonTerminal::openOpNT, {
        { nonTerminal::tagOrStringLiteralNT, nonTerminal::tagNT }
    } },
    {nonTerminal::tagNT, {
        { Token::tagTok }
    } },
    { nonTerminal::referOpNT,{
        {nonTerminal::operandNT, nonTerminal::tagNT, nonTerminal::tagNT }
    } },
    { nonTerminal::appendOpNT, {
        { nonTerminal::operandNT, nonTerminal::appendOpSecondOperandNT, nonTerminal::tagNT }
    } },
    { nonTerminal::appendOpSecondOperandNT, {
        {nonTerminal::operandNT},
        { Token::frontTok },
        { Token::backTok }
    } },
    { nonTerminal::bindOpNT, {
        { nonTerminal::literalNT, nonTerminal::tagNT}
    } },
    { nonTerminal::literalNT, {
        {Token::boolLiteralTok},
        {Token::intLiteralTok},
        {Token::floatLiteralTok},
        {Token::stringLiteralTok},
        {Token::charLiteralTok}
    } }

};


SyntaxValidator::SyntaxValidator(Logger* logger, const std::vector<Token> &tokens, const std::map<nonTerminal, std::vector<std::vector<nonTerminalOrToken>>>& prod):
    m_logger(logger),
    m_tokens(tokens),
    m_productions(prod)
{
    
}


std::string makeValidatorMessage(const std::vector<nonTerminal> &nts) {
    std::stringstream ss;
    ss << "(expected: ";
    for (size_t i = 0; i < nts.size(); i++)
    {
        ss << nonTerminalToStrings[nts[i]];
        if (i != nts.size()-1) {
            ss << "->";
        }
    }
    ss << ")";

    return ss.str();
}


//each rule is an OR operation. Its either this one OR the next one, so we evaluate each one until we find a match.
validateResult SyntaxValidator::validate()
{
    //we validate structure of expressions
    size_t lastToken = 0;


    std::stack<nonTerminal> nexts;
    nexts.push(nonTerminal::startNT);

    std::vector<nonTerminal> allProccessedNTs;


    while(!nexts.empty()) {

        //NEXTS LOOP(THE REMAINING NON-TERMINALS TO PROCCESS)
        allProccessedNTs.push_back(nexts.top());

        if (lastToken >= m_tokens.size()) {
            return validateResult(false, makeValidatorMessage(allProccessedNTs), lastToken-1, 0);
        }


        //we increment the last token to skip the current token
        if (nexts.top() == skip) {
            lastToken++;

            nexts.pop();
            continue;
        }


        auto rulesForNT = m_productions[nexts.top()];

        nexts.pop();





        bool foundRuleMatch = false;
        std::vector<nonTerminal> candidateStack;
        bool foundNonTerminalOrLambdaOnly = false;

        //if the first rule is empty, we found a lambda rule
        if (rulesForNT[0].empty()) {
            foundNonTerminalOrLambdaOnly = true;
        }
        //if the first symbol is a nonTerminal then the whole chain is non-terminals, it is the only non-terminal chain for the production rules of a non-terminal.
        else if (!rulesForNT[0][0].trueForTerminal) {


            m_logger->log("SyntaxValidator", "found a non terminal only");

            for (auto x : rulesForNT[0]) { candidateStack.push_back(x.nt); }
            foundNonTerminalOrLambdaOnly = true;
        }


        


        
        //we iterate through each rule for a given non-terminal
        for (size_t i = 0; i < rulesForNT.size(); i++) {
            //RULE LOOP

            if (foundNonTerminalOrLambdaOnly && i == 0) {
                continue;
            }


            bool foundMatch = true;


            //we iterate through each symbol produced by the rule, checking if they are terminals.
            for (size_t o = 0; o < rulesForNT[i].size(); o++) {
                
                //SYMBOL LOOP

                //we match the terminals, so we only check for terminals
                if (rulesForNT[i][o].trueForTerminal) {
                    //if the terminal sub-string exceeds the size of the tokens, we break
                    if ((lastToken+o) > (m_tokens.size()-1)) {
                        foundMatch = false;
                        break;
                    }
                    //if the terminal does not match we break
                    if (rulesForNT[i][o].terminal != m_tokens[lastToken + o]) {
                        foundMatch = false;
                        break;
                    }
                }

                //if all terminals match and the token string is the same size or larger than the to be matched terminal sub-string, then a match was found.

                //SYMBOL LOOP
            }


            //this rule matches
            if (foundMatch) {
                candidateStack.clear();
                m_logger->log("SyntaxValidator","found match!");
                //so we add the non-terminals to the candidate non-terminal stack
                for (auto x : rulesForNT[i]) {
                    if (!x.trueForTerminal) {
                        candidateStack.push_back(x.nt);
                    }else {
                    //we increase the lastToken counter for every matched terminal, so we can keep matching the terminals.
                        lastToken++;
                    }
                }
                foundRuleMatch = true;
                //and we break
                break;
            }

            //RULE LOOP
        }

        if (!foundNonTerminalOrLambdaOnly && !foundRuleMatch) {

            return validateResult(false, makeValidatorMessage(allProccessedNTs),lastToken,0);
        }

        for (int i = candidateStack.size()-1; i >= 0; i--) {
            m_logger->log("SyntaxValidator", std::to_string(int(candidateStack[i])));
            nexts.push(candidateStack[i]);
        }




        m_logger->log("SyntaxValidator", std::to_string(lastToken));
        m_logger->log("SyntaxValidator", "--");


        //NEXTS LOOP(THE REMAINING NON_TERMINALS TO PROCCESS)
    }



    return validateResult(true);
}

SyntaxValidator::~SyntaxValidator()
{
}

nonTerminalOrToken::nonTerminalOrToken(nonTerminal p_nt):
    nt(p_nt),
    trueForTerminal(false)
{
   
}

nonTerminalOrToken::nonTerminalOrToken(Token p_terminal):
    terminal(p_terminal),
    trueForTerminal(true)
{
}

validateResult::validateResult(bool p_success, std::string p_message, size_t p_tokenPos, size_t p_line):
    success(p_success),
    message(p_message),
    tokenPos(p_tokenPos),
    line(p_line)
{
}

std::string validateResult::getString() const
{
    std::stringstream ss;

    ss << "|validation was successful?: " << (success?"yes":"no") << ", line number: " << line << ", error token position: " << tokenPos << ", message: " << message << "|";

    return ss.str();
}

bool validateResult::operator==(const validateResult& rhs)
{
    return success == rhs.success && line == rhs.line && tokenPos == rhs.tokenPos;
}

validationError::validationError(const validateResult& res, const std::string& fileName):
    m_valRes(res),
    m_fileName(fileName)
{
    std::stringstream ss;

    ss << "VALIDATION ERROR!:\n\t" << m_valRes.getString() << "\nwith file/source: " << m_fileName;

    m_message = ss.str();
}

#include <iostream>

const char* validationError::what() const
{
    return m_message.c_str();
}
