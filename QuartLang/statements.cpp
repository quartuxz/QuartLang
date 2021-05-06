#include "statements.h"


variableDeclaration::variableDeclaration(size_t orderedID, const std::string& tag, DataStructure* data) noexcept :
	ProgramStructure<statementType>(orderedID, statementType::variableDeclarationSttt),
	m_tag(tag),
	m_data(data)
{

}

std::string variableDeclaration::getTag()
{
	return m_tag;
}

DataStructure* variableDeclaration::getData()
{
	return m_data;
}

void variableDeclaration::setData(DataStructure* data)noexcept
{
	m_data = data;
}



functionCall::functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, DataStructure*>& args, size_t variableArgsNum) :
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


const std::map<std::string, DataStructure*>& functionCall::getArgs() const noexcept
{
	return m_args;
}

setOperation::setOperation(size_t orderedID, const std::string& settedName, const std::string& toVarName) :
	ProgramStructure<statementType>(orderedID, statementType::setOperationSttt),
	m_settedName(settedName),
	m_toVarName(toVarName),
	m_setType(setType::toVariable)
{
}

setOperation::setOperation(size_t orderedID, const std::string& settedName, DataStructure* toLiteralData) :
	ProgramStructure<statementType>(orderedID, statementType::setOperationSttt),
	m_settedName(settedName),
	m_toLiteralData(toLiteralData),
	m_setType(setType::toLiteral)
{
}

setOperation::setOperation(size_t orderedID, const std::string& settedName) :
	ProgramStructure<statementType>(orderedID, statementType::setOperationSttt),
	m_setType(setType::toResult)

{
}

setType setOperation::getSetType() const noexcept
{
	return m_setType;
}

std::string setOperation::getSettedName() const noexcept
{
	return m_settedName;
}

std::string setOperation::getToVarName() const noexcept
{
	return m_toVarName;
}

DataStructure* setOperation::getToLiteralData()
{
	return m_toLiteralData;
}

