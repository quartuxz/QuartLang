#include "builtinFunctions.h"
#include <sstream>
#include <iostream>
#include "tests.h"

WrongArgsNumberException::WrongArgsNumberException(size_t expected, size_t given) :
    m_expected(expected),
    m_given(given)
{
    std::stringstream ss;
    ss << "the function expected " << m_expected << ", arguments but received " << m_given << ".";
    m_whatString = ss.str();
}

const char* WrongArgsNumberException::what() const
{

    return m_whatString.c_str();
}

#ifdef MUST_TEST_ISHLENG
#define OUTPUT_STREAM_TEST_OR_REAL_ISHLENG testOutput
#else
#define OUTPUT_STREAM_TEST_OR_REAL_ISHLENG std::cout
#endif

DataStructure print_anything_BIF::call(const std::map<std::string, DataStructure>& args)
{
    if (args.size() > 1) {
        throw WrongArgsNumberException(1, args.size());
    }
    if (args.at("input-words").getTypeOrPrimitiveTag() == "string") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.at("input-words").getString();
    }
    else if (args.at("input-words").getTypeOrPrimitiveTag() == "float") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.at("input-words").getFloatData();
    }
    else if (args.at("input-words").getTypeOrPrimitiveTag() == "int") {
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.at("input-words").getIntData();
    }
    else if(args.at("input-words").getTypeOrPrimitiveTag() == "bool"){
        OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << args.at("input-words").getBoolData();
    }
    return DataStructure();
}

DataStructure print_new_line_BIF::call(const std::map<std::string, DataStructure>& args)
{
    OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << "\n";
    return DataStructure();
}
