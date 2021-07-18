#include "programs.h"
#include <stdexcept>
#include <sstream>



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


#define DEEP_COPY(y, type) for(auto x : copy.y){y[x.first] = new type(*x.second); }

Subprogram::Subprogram(const Subprogram& copy) :
	ProgramStructure<subprogramType>(copy.m_orderedID, copy.m_type),
	m_parent(copy.m_parent),
	m_contents(copy.m_contents)
{
	DEEP_COPY(m_variables, variableDeclaration);
	DEEP_COPY(m_functionCalls, functionCall);
	DEEP_COPY(m_setOperations, setOperation);
	DEEP_COPY(m_arithmeticOperations, arithmeticOperation);
	DEEP_COPY(m_finallySttts, finallySttt);
	DEEP_COPY(m_evaluateOperations, evaluateOperation);
	DEEP_COPY(m_flipOperations, flipOperation);
	DEEP_COPY(m_referOperations, referOperation);
	DEEP_COPY(m_appendOperations, appendOperation);
	DEEP_COPY(m_finishOperations, finishOperation);
	DEEP_COPY(m_importOperations, importOperation);




	for (auto x: copy.m_subprograms) {
		Subprogram* toAddSubProgram = nullptr;
		switch (x.second->m_type)
		{
		case subprogramType::functionBlock:
			toAddSubProgram = new functionBlock(dynamic_cast<functionBlock&>(*x.second));
			break;
		case subprogramType::conditionalBlock:
			toAddSubProgram = new conditionalBlock(dynamic_cast<conditionalBlock&>(*x.second));
			break;
		case subprogramType::baseBlock:
			toAddSubProgram = new Subprogram(*x.second);
			break;
		default:
			break;
		}

		m_addSubprogram(x.first,toAddSubProgram);
	}
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

const importOperation* Subprogram::getImportOperation(size_t orderedID) const
{
	return m_importOperations.at(orderedID);
}

std::vector<programContent> Subprogram::getContent() const noexcept
{
	return m_contents;
}


#define DELETE_STATEMENT_MAP(y) for(auto x : y){ delete x.second; }

Subprogram::~Subprogram()
{
	DELETE_STATEMENT_MAP(m_variables);
	DELETE_STATEMENT_MAP(m_functionCalls);
	DELETE_STATEMENT_MAP(m_setOperations);
	DELETE_STATEMENT_MAP(m_arithmeticOperations);
	DELETE_STATEMENT_MAP(m_evaluateOperations);
	DELETE_STATEMENT_MAP(m_flipOperations);
	DELETE_STATEMENT_MAP(m_referOperations);
	DELETE_STATEMENT_MAP(m_appendOperations);
	DELETE_STATEMENT_MAP(m_finishOperations);
	DELETE_STATEMENT_MAP(m_importOperations);
	DELETE_STATEMENT_MAP(m_subprograms);
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

functionBlock::functionBlock(const functionBlock& copy) :
	Subprogram(copy),
	m_tag(copy.m_tag)
{
}

