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
	bool m_isMultithreaded;
	std::string m_threadInstanceName;
public:
	functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, operand>& args, bool isMultithreaded = false, const std::string& threadInstanceName = "");
	functionCall();
	bool getIsMultithreaded()const noexcept;
	const std::string& getThreadInstanceName()const noexcept;

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

enum class evalType {
	equal, lessThan, moreThan, orEval, andEval, lessThanOrEqual, moreThanOrEqual, moreThanOrLessThan
};

class evaluateOperation : public ProgramStructure<statementType> {
private:
	evalType m_evalType;
	operand m_lhs;
	operand m_rhs;
public:
	evaluateOperation(size_t orderedID, evalType eType, const operand &lhs, const operand &rhs);
	const operand& getRhs()const noexcept;
	const operand& getLhs()const noexcept;
	evalType getEvalType()const noexcept;
};


class flipOperation : public ProgramStructure<statementType> {
private:
	operand m_op;
public:
	flipOperation(size_t orderedId, const operand &op);

	const operand& getOperand()const noexcept;
};


class referOperation : public ProgramStructure<statementType> {
private:
	operand m_where;
	std::string m_varName;
	std::string m_referantName;
public:
	referOperation(size_t orderedId, const std::string& varName, const operand& whereOp, const std::string &referantName);
	const operand& getWhere()const;
	std::string getVarName()const;
	std::string getReferantName()const;
};


enum class appendType {
	push_back, push_front, map_to
};

class appendOperation: public ProgramStructure<statementType> {
private:
	appendType m_appendType;
	operand m_content;
	operand m_place;
	std::string m_varName;
public:
	appendOperation(size_t orderedId, appendType appType,const operand& place,const operand& content, const std::string& varName);
	appendType getAppendType()const noexcept;
	const operand& getContent()const noexcept;
	std::string getVarName()const noexcept;
	const operand& getPlace()const noexcept;

};


class finishOperation : public ProgramStructure<statementType> {
private:
	std::string m_threadInstanceName;
public:
	finishOperation(size_t orderedId, const std::string& threadInstanceName);

	const std::string& getThreadInstanceName()const noexcept;

};


class importOperation : public ProgramStructure<statementType> {
private:
	operand m_code;
public:
	importOperation(size_t orderedId, const operand &code);


	const operand& getCode()const noexcept;

};