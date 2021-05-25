#include "programs.h"
#include <stdexcept>




void Subprogram::m_addSubprogram(size_t orderedID, Subprogram* subprogram)
{
	m_subprograms[orderedID] = subprogram;
	subprogram->m_parent = this;
}

Subprogram::Subprogram(size_t orderedID, subprogramType type) noexcept :
	ProgramStructure<subprogramType>(orderedID, type)
{
}

const Subprogram* Subprogram::getSubprogram(size_t orderedID) const
{
	return m_subprograms.at(orderedID);
}

const flipOperation* Subprogram::getFlipOperation(size_t orderedID) const
{
	return m_flipOperations.at(orderedID);
}


const referOperation* Subprogram::getReferOperation(size_t orderedID) const
{
	return m_referOperations.at(orderedID);
}

const appendOperation* Subprogram::getAppendOperation(size_t orderedID) const
{
	return m_appendOperations.at(orderedID);
}

const variableDeclaration* Subprogram::getVariable(size_t orderedID)const
{
	return m_variables.at(orderedID);
}

const setOperation* Subprogram::getSetOperation(size_t orderedID)const
{
	return m_setOperations.at(orderedID);
}

const evaluateOperation* Subprogram::getEvaluateOperation(size_t orderedID) const
{
	return m_evaluateOperations.at(orderedID);
}

const arithmeticOperation* Subprogram::getArithmeticOperation(size_t orderedID) const
{
	return m_arithmeticOperations.at(orderedID);
}

const finishOperation* Subprogram::getFinishOperation(size_t orderedID) const 
{
	return m_finishOperations.at(orderedID);
}

std::vector<programContent> Subprogram::getContent() const noexcept
{
	return m_contents;
}

Subprogram::~Subprogram()
{
	for (auto x : m_variables)
	{
		delete x.second;
	}
	for (auto x : m_functionCalls) {
		delete x.second;
	}
	for (auto x : m_setOperations) {
		delete x.second;
	}
	for (auto x : m_arithmeticOperations)
	{
		delete x.second;
	}

	for (auto x : m_evaluateOperations) {
		delete x.second;
	}
	
	for (auto x : m_flipOperations) {
		delete x.second;
	}
	
	for (auto x : m_referOperations) {
		delete x.second;
	}
	
	for (auto x : m_appendOperations) {
		delete x.second;
	}

	for (auto x : m_subprograms) {
		delete x.second;
	}

}

const functionCall* Subprogram::getFunctionCall(size_t orderedID)const
{
	return m_functionCalls.at(orderedID);
}

const finallySttt* Subprogram::getFinallySttt(size_t orderedID) const
{
	return m_finallySttts.at(orderedID);
}

builtinFunction* Program::getIncludedBuiltin(std::string m_name)const
{
	builtinFunction* retVal = nullptr;
	if (m_includedBuiltins.find(m_name) != m_includedBuiltins.end()) {
		retVal = m_includedBuiltins.at(m_name);
	}
	return retVal;
}


Program::~Program()
{
	for (auto x : m_includedBuiltins) {
		delete x.second;
	}
}

Program::Program() noexcept :
	Subprogram(0, subprogramType::baseBlock)
{

}

conditionalBlock::conditionalBlock(size_t orderedID, const operand& condition):
	Subprogram(orderedID, subprogramType::conditionalBlock),
	m_condition(condition)
{
}

const operand& conditionalBlock::getCondition() const noexcept
{
	return m_condition;
}

functionBlock::functionBlock(size_t orderedID, const std::string& tag) :
	Subprogram(orderedID, subprogramType::functionBlock),
	m_tag(tag)
{
}

const std::string& functionBlock::getTag() const
{
	return m_tag;
}
