#pragma once
#include <optional>
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
public:
	functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, operand>& args);
	functionCall();
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


enum class finallyType {
	repeat, end, give
};

class finallySttt : public ProgramStructure<statementType>{
private:
	finallyType m_finallyType;
	operand m_optionalOperand;
public:
	finallySttt(size_t orderedID, finallyType fType, const std::optional<operand> &op = std::nullopt);

	finallyType getFinallyType()const noexcept;
	const operand& getOptionalOperand()const noexcept;
};