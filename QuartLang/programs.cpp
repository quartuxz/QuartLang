#include "programs.h"
#include <stdexcept>


//IMPLEMENT THIS FULLY
const DataStructure& Program::m_addLiteral(std::string literalStr, Token token)
{

	DataStructure data;
	std::string type;
	//IMPLEMENT THE OTHER CASES
	switch (token)
	{
	case Token::stringLiteralTok:
		m_literals.push_back(DataStructure(literalStr));
		break;
	case Token::intLiteralTok:
		m_literals.push_back(DataStructure(std::atoi(literalStr.c_str())));
		break;
	case Token::floatLiteralTok:
		m_literals.push_back(DataStructure((float)std::atof(literalStr.c_str())));
		break;
	case Token::boolLiteralTok:
		m_literals.push_back(DataStructure((literalStr == "true" ? true : false)));
		break;
	default:
		throw std::invalid_argument("Token is not a literal!");
		break;
	}

	return m_literals.back();
}

Subprogram::Subprogram(size_t orderedID, subprogramType type) noexcept :
	ProgramStructure(orderedID, type)
{
}

void Subprogram::addSubprogram(const Subprogram& subprogram)
{
}

std::string Subprogram::getTag() const noexcept
{
	return m_tag;
}

const variableDeclaration* Subprogram::getVariable(size_t orderedID)const
{
	return m_variables.at(orderedID);
}

const setOperation* Subprogram::getSetOperation(size_t orderedID)const
{
	return m_setOperations.at(orderedID);
}

const arithmeticOperation* Subprogram::getArithmeticOperation(size_t orderedID) const
{
	return m_arithmeticOperations.at(orderedID);
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

	for (auto x : m_subprograms) {
		delete x.second;
	}
}

const functionCall* Subprogram::getFunctionCall(size_t orderedID)const
{
	return m_functionCalls.at(orderedID);
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

conditionalBlock::conditionalBlock(size_t orderedID, const operand& condition, bool repeats):
	Subprogram(orderedID, subprogramType::conditionalBlock),
	m_condition(condition),
	m_repeats(repeats)
{
}
