#pragma once
#include "ProgramStructure.h"
#include "DataStructure.h"


class variableDeclaration : public ProgramStructure<statementType> {
private:
	std::string m_tag;
	size_t m_scopeNesting;
	DataStructure m_data;
public:
	variableDeclaration(size_t orderedID, const std::string& tag, size_t scopeNesting = 0)noexcept;
	std::string getTag()const noexcept;
	const DataStructure& getData()const noexcept;
	size_t getScopeNesting()const noexcept;
	void setData(const DataStructure& data);
};

class functionCall : public ProgramStructure<statementType> {
private:
	std::string m_functionCalledTag;
	std::map<std::string, operand> m_args;
	size_t m_variableArgsNum = 0;
public:
	functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, operand>& args, size_t variableArgsNum = 0);
	functionCall();
	size_t getVariableArgsNum()const noexcept;
	std::string getFunctionCalledTag()const noexcept;
	const std::map<std::string, operand>& getArgs()const noexcept;
};

class setOperation : public ProgramStructure<statementType> {
private:
	operand m_toSet;
	std::string m_settedName;
public:

	setOperation(size_t orderedID, const std::string & settedName, const operand& op);
	std::string getSettedName()const noexcept;
	
	const operand& getToSet()const noexcept;

};


enum class arithmeticOperationType {
	add, subtract, multiply, divide
};

class arithmeticOperation : public ProgramStructure<statementType> {
private:
	arithmeticOperationType m_opType;

	operand m_lhs;
	operand m_rhs;
public:

	arithmeticOperation(size_t orderedID, arithmeticOperationType opType, const operand& lhs,const operand& rhs);

	const operand& getLhs()const noexcept;
	const operand& getRhs()const noexcept;
	arithmeticOperationType getOperationType()const noexcept;

};