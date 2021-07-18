#pragma once
#include <map>
#include <exception>

#include "EngineError.h"
#include "DataStructure.h"
#include "programs.h"


class builtinFunction
{
protected:
	//calls the given function, returns a pair of bool(success) and string(error message).
	virtual std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval) = 0;
public:



	//does the call with exception throwing
	void doCall(const std::map<std::string, DataStructure*>&args, DataStructure *retval, const programContent& errorOrigin, const std::vector<std::string>& stackTrace);

};


class print_anything_BIF : public builtinFunction {
private:
	std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class print_new_line_BIF : public builtinFunction {
private:
	std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class is_empty_BIF : public builtinFunction {
private:
	std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class get_input_BIF : public builtinFunction {
private:
	std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class run_ishleng_BIF : public builtinFunction {
private:
	std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};

class unstring_BIF : public builtinFunction {
private:
	std::pair<bool, std::string> call(const std::map<std::string, DataStructure*>& args, DataStructure* retval)override;
};
