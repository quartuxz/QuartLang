#include "builtinFunctions.h"
#include <sstream>
#include <iostream>

WrongArgsNumberException::WrongArgsNumberException(size_t expected, size_t given):
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

std::map<std::string, DataStructure> print_BIF::call(const std::map<std::string, DataStructure>& args)
{
    if (args.size() > 1) {
        throw WrongArgsNumberException(1, args.size());
    }
    std::cout << "asdasd";
    std::cout << (const char*)args.at("words").getData();
    return std::map<std::string, DataStructure>();
}
