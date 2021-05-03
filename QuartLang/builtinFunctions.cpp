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
    if (args.at("words").getTypeOrPrimitiveTag() == "string"){
        std::cout << *((std::string*)args.at("words").getData());
    }
    else if (args.at("words").getTypeOrPrimitiveTag() == "float") {
        std::cout << *(float*)args.at("words").getData();
    }
    else if (args.at("words").getTypeOrPrimitiveTag() == "int") {
        std::cout << *(int*)args.at("words").getData();
    }

    return std::map<std::string, DataStructure>();
}
