#include "statements.h"


variableDeclaration::variableDeclaration(size_t orderedID, const std::string& tag, size_t scopedNesting) noexcept :
	ProgramStructure<statementType>(orderedID, statementType::variableDeclarationSttt),
	m_tag(tag),
	m_scopeNesting(scopedNesting)
	
{

}

std::string variableDeclaration::getTag()const noexcept
{
	return m_tag;
}

const DataStructure& variableDeclaration::getData() const noexcept
{
	return m_data;
}

size_t variableDeclaration::getScopeNesting() const noexcept
{
	return m_scopeNesting;
}

void variableDeclaration::setData(const DataStructure& data)
{
	m_data = data;
}



functionCall::functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, operand>& args, size_t variableArgsNum) :
	ProgramStructure<statementType>(orderedID, statementType::functionCallSttt),
	m_functionCalledTag(functionCalledTag),
	m_args(args),
	m_variableArgsNum(variableArgsNum)
{
}

functionCall::functionCall() :
	ProgramStructure<statementType>(0, statementType::functionCallSttt)
{
}

size_t functionCall::getVariableArgsNum() const noexcept
{
	return m_variableArgsNum;
}

std::string functionCall::getFunctionCalledTag() const noexcept
{
	return m_functionCalledTag;
}


const std::map<std::string, operand>& functionCall::getArgs() const noexcept
{
	return m_args;
}

setOperation::setOperation(size_t orderedID, const std::string& settedName, const operand& op):
	ProgramStructure<statementType>(orderedID, statementType::setOperationSttt),
	m_settedName(settedName),
	m_toSet(op)
{
}

std::string setOperation::getSettedName() const noexcept
{
	return m_settedName;
}

const operand& setOperation::getToSet() const noexcept
{
	return m_toSet;
	// TODO: insert return statement here
}

arithmeticOperation::arithmeticOperation(size_t orderedID, arithmeticOperationType opType, const operand& lhs, const operand& rhs):
	ProgramStructure<statementType>(orderedID, statementType::arithmeticOperationSttt),
	m_opType(opType),
	m_lhs(lhs),
	m_rhs(rhs)
{
}

const operand& arithmeticOperation::getLhs() const noexcept
{
	return m_lhs;
}

const operand& arithmeticOperation::getRhs() const noexcept
{
	return m_rhs;
}

arithmeticOperationType arithmeticOperation::getOperationType() const noexcept
{
	return m_opType;
}
