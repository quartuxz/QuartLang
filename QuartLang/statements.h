#pragma once
#include "ProgramStructure.h"
#include "DataStructure.h"


class variableDeclaration : public ProgramStructure<statementType> {
private:
	std::string m_tag;
	DataStructure* m_data;
public:
	variableDeclaration(size_t orderedID, const std::string& tag, DataStructure* data)noexcept;
	std::string getTag();
	DataStructure* getData();
	void setData(DataStructure* data)noexcept;
};

class functionCall : public ProgramStructure<statementType> {
private:
	std::string m_functionCalledTag;
	std::map<std::string, DataStructure*> m_args;
	size_t m_variableArgsNum = 0;
public:
	functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, DataStructure*>& args, size_t variableArgsNum = 0);
	functionCall();
	size_t getVariableArgsNum()const noexcept;
	std::string getFunctionCalledTag()const noexcept;
	const std::map<std::string, DataStructure*>& getArgs()const noexcept;
};

enum class setType {
	toLiteral,
	toResult,
	toVariable
};

class setOperation : public ProgramStructure<statementType> {
private:
	setType m_setType;
	std::string m_settedName;
	std::string m_toVarName;
	DataStructure* m_toLiteralData;
public:
	setOperation(size_t orderedID, const std::string& settedName, const std::string& toVarName);
	setOperation(size_t orderedID, const std::string& settedName, DataStructure* toLiteralData);
	setOperation(size_t orderedID, const std::string& settedName);

	setType getSetType()const noexcept;
	std::string getSettedName()const noexcept;
	std::string getToVarName()const noexcept;
	DataStructure* getToLiteralData();

};