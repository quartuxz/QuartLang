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


	virtual void call(const std::map<std::string, DataStructure*>& args, DataStructure *retval) = 0;

};


class print_anything_BIF : public builtinFunction {
public:
	void call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class print_new_line_BIF : public builtinFunction {
public:
	void call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class is_empty_BIF : public builtinFunction {
public:
	void call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};
