#pragma once
#include <map>
#include <exception>
#include "DataStructure.h"

//observation: could make a type checker and make it so it throws this exception after parsing
//and not during execution.
class WrongArgsNumberException : public std::exception {
private:
	size_t m_expected;
	size_t m_given;
	std::string m_whatString;
public:
	WrongArgsNumberException(size_t expected, size_t given);
	const char* what()const override;
};


class builtinFunction
{
public:


	virtual std::map<std::string, DataStructure> call(const std::map<std::string, DataStructure>& args) = 0;

};


class print_BIF : public builtinFunction {
public:
	std::map<std::string, DataStructure> call(const std::map<std::string, DataStructure>& args)override;
};
