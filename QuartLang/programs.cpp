#include "programs.h"
#include <stdexcept>


//IMPLEMENT THIS FULLY
DataStructure* Program::m_addLiteral(std::string literalStr, Token token)
{

	void* data = nullptr;
	std::string type;
	//IMPLEMENT THE OTHER CASES
	switch (token)
	{
	case Token::stringLiteralTok:
		type = "string";
		m_stringLiterals.push_back(literalStr);
		data = &m_stringLiterals.back();
		break;
	case Token::intLiteralTok:
		type = "int";
		m_intLiterals.push_back(std::atoi(literalStr.c_str()));
		data = &m_intLiterals.back();
		break;
	case Token::floatLiteralTok:
		type = "float";
		m_floatLiterals.push_back(std::atof(literalStr.c_str()));
		data = &m_floatLiterals.back();
		break;
	case Token::boolLiteralTok:
		type = "bool";
		m_boolLiterals.push_back((literalStr == "true" ? true : false));
		data = &m_boolLiterals.back();;
		break;
	default:
		throw std::invalid_argument("Token is not a literal!");
		break;
	}
	m_stackDataStructures.push_back(new DataStructure(type, std::map<std::string, DataStructure*>(), data));
	return m_stackDataStructures.back();
}

DataStructure* Program::m_addTag(std::string tagStr)
{
	m_tags.push_back(tagStr);
	m_stackDataStructures.push_back(new DataStructure("tagString", std::map<std::string, DataStructure*>(), &m_tags.back()));
	return m_stackDataStructures.back();
}


void Subprogram::m_addVariableDeclaration(variableDeclaration* varDecl)
{
	m_variables[varDecl->getOrderedID()] = varDecl;
	m_variableTags[varDecl->getTag()] = varDecl->getOrderedID();
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

variableDeclaration* Subprogram::getVariableUpwards(const std::string& tag, size_t upwardsFrom)
{
	if (checkVariableTagExists(tag)) {
		variableDeclaration* var = getVariable(tag);
		if (upwardsFrom > var->getOrderedID()) {
			return var;
		}
	}

	if (m_parent != nullptr) {
		return m_parent->getVariableUpwards(tag, upwardsFrom);
	}

	return nullptr;
}

bool Subprogram::checkVariableTagExists(const std::string& tag)const noexcept
{
	return m_variableTags.find(tag) != m_variableTags.end();
}



variableDeclaration* Subprogram::getVariable(const std::string& tag)
{
	return m_variables[m_variableTags[tag]];
}

variableDeclaration* Subprogram::getVariable(size_t orderedID)
{
	return m_variables[orderedID];
}

setOperation* Subprogram::getSetOperation(size_t orderedID)
{
	return m_setOperations[orderedID];
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