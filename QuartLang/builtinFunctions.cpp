#include "builtinFunctions.h"
#include <sstream>
#include <iostream>
#include "tests.h"
#include "Ishleng.h"


#ifdef MUST_TEST_ISHLENG
#define OUTPUT_STREAM_TEST_OR_REAL_ISHLENG testOutput
#else
#define OUTPUT_STREAM_TEST_OR_REAL_ISHLENG std::cout
#endif

#define WRONG_ARGS "wrong argument size"
#define WRONG_TYPE "wrong type"

std::pair<bool, std::string> print_anything_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    if (args.size() != 1) {
        return { false, WRONG_ARGS };
    }
    if (args.begin()->second->getTypeOrPrimitiveTag() == "string") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.begin()->second->getString();
    }
    else if (args.begin()->second->getTypeOrPrimitiveTag() == "float") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.begin()->second->getFloatData();
    }
    else if (args.begin()->second->getTypeOrPrimitiveTag() == "int") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.begin()->second->getIntData();
    }
    else if (args.begin()->second->getTypeOrPrimitiveTag() == "bool") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.begin()->second->getBoolData();
    }
    else if (args.begin()->second->getTypeOrPrimitiveTag() == "char") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.begin()->second->getCharData();
    }
    else {
        return { false, WRONG_TYPE };
    }

    return {true, ""};

}

std::pair<bool, std::string> print_new_line_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << std::endl;
    return { true, "" };
}

std::pair<bool, std::string> is_empty_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    *retval = DataStructure(args.begin()->second->isEmpty());
    return {true, ""};
}

std::pair<bool, std::string> get_input_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    std::string inString;
    std::cin >> inString;
    *retval = DataStructure(inString);
    return { true, "" };
}

std::pair<bool, std::string> run_ishleng_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{

    Logger logger;
    logger.toggleLogging(false);

    DictionaryLexer dict("words_alpha.txt",m_matches);


    Ishleng ishleng(&logger,&dict, args.begin()->second->getString());
    ishleng.lex();
    ishleng.parse();
    *retval = DataStructure((int)ishleng.run());
    return { true, "" };

}

void builtinFunction::doCall(const std::map<std::string, DataStructure*>& args, DataStructure* retval, const programContent& errorOrigin, const std::vector<std::string>& stackTrace)
{
    
    auto callRes = call(args,retval);
    if (!callRes.first) {
        throw EngineError(engineErrorType::builtinError, errorOrigin, callRes.second, stackTrace);
    }

}

#include "LexUtils.h"

std::pair<bool, std::string> unstring_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    if (args.size() != 1) {
        return {false, WRONG_ARGS};
    }

    if (args.begin()->second->getTypeOrPrimitiveTag() != STRING_TYPE_STR) {
        return { false, WRONG_TYPE };
    }

    auto m_addLiteral = [retval](const std::string & str, Token type) {
        switch (type)
        {
        case Token::boolLiteralTok:
            if (str == "true") {
                *retval = DataStructure(true);
            }else{
                *retval = DataStructure(false);
            }
            break;
        case Token::floatLiteralTok:
            *retval = DataStructure((float)std::atof(str.c_str()));
            break;
        case Token::intLiteralTok:
            *retval = DataStructure((int)std::atoi(str.c_str()));
            break;
        }
        
    };

    auto currentTokenString = args.begin()->second->getString();

    LEX_NUMBER_OR_BOOL;

    return {true, ""};
}
