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



void print_anything_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    if (args.size() > 1) {
        throw WrongArgsNumberException(1, args.size());
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
}

void print_new_line_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    OUTPUT_STREAM_TEST_OR_REAL_ISHLENG << "\n";
}

void is_empty_BIF::call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)
{
    *retval = DataStructure(args.begin()->second->isEmpty());
}
