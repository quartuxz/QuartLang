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



functionCall::functionCall(size_t orderedID, const std::string& functionCalledTag, const std::map<std::string, operand>& args, bool isMultithreaded, const std::string& threadInstanceName):
ProgramStructure<statementType>(orderedID, statementType::functionCallSttt),
m_functionCalledTag(functionCalledTag),
m_args(args),
m_isMultithreaded(isMultithreaded),
m_threadInstanceName(threadInstanceName)
{
}

functionCall::functionCall() :
	ProgramStructure<statementType>(0, statementType::functionCallSttt)
{
}

bool functionCall::getIsMultithreaded() const noexcept
{
	return m_isMultithreaded;
}

const std::string& functionCall::getThreadInstanceName() const noexcept
{
	return m_threadInstanceName;
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

finallySttt::finallySttt(size_t orderedID, finallyType fType, const std::optional<operand>& op) :
	ProgramStructure<statementType>(orderedID, statementType::finallySttt),
	m_finallyType(fType)
{
	if (op.has_value()) {
		m_optionalOperand = op.value();
	}
}

finallyType finallySttt::getFinallyType() const noexcept
{
	return m_finallyType;
}

const operand& finallySttt::getOptionalOperand() const noexcept
{
	return m_optionalOperand;
}

evaluateOperation::evaluateOperation(size_t orderedID, evalType eType, const operand& lhs, const operand& rhs):
	ProgramStructure<statementType>(orderedID,statementType::evaluateOperationSttt),
	m_evalType(eType),
	m_lhs(lhs),
	m_rhs(rhs)
{
}

const operand& evaluateOperation::getRhs() const noexcept
{
	return m_rhs;
}

const operand& evaluateOperation::getLhs() const noexcept
{
	return m_lhs;
}

evalType evaluateOperation::getEvalType() const noexcept
{
	return m_evalType;
}

flipOperation::flipOperation(size_t orderedId, const operand& op):
	ProgramStructure<statementType>(orderedId,statementType::flipOperationSttt),
	m_op(op)
{

}

const operand& flipOperation::getOperand() const noexcept
{
	return m_op;
}

appendOperation::appendOperation(size_t orderedId, appendType appType, const operand& place, const operand& content, const std::string& varName):
	ProgramStructure<statementType>(orderedId, statementType::appendOperationSttt),
	m_appendType(appType),
	m_place(place),
	m_content(content),
	m_varName(varName)
{
}

appendType appendOperation::getAppendType() const noexcept
{
	return m_appendType;
}

const operand& appendOperation::getContent() const noexcept
{
	return m_content;
}

std::string appendOperation::getVarName() const noexcept
{
	return m_varName;
}

const operand& appendOperation::getPlace() const noexcept
{
	return m_place;
}

referOperation::referOperation(size_t orderedId, const std::string& varName, const operand& whereOp, const std::string& referantName):
	ProgramStructure<statementType>(orderedId, statementType::referOperationSttt),
	m_where(whereOp),
	m_varName(varName),
	m_referantName(referantName)
{
}

const operand& referOperation::getWhere() const
{
	// TODO: insert return statement here
	return m_where;
}

std::string referOperation::getVarName() const
{
	return m_varName;
}

std::string referOperation::getReferantName() const
{
	return m_referantName;
}

finishOperation::finishOperation(size_t orderedId, const std::string &threadInstanceName):
	ProgramStructure<statementType>(orderedId, statementType::finishSttt),
	m_threadInstanceName(threadInstanceName)
{
}

const std::string& finishOperation::getThreadInstanceName() const noexcept
{
	return m_threadInstanceName;
}
