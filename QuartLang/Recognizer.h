#pragma once
#include "Parser.h"
#include <map>


enum class subprogramType{
	baseBlock,
	functionBlock,
	ifBlock,
	classBlock
};


enum class statementType {
	variableDeclarationSttt,
	arithmeticOperationSttt,
};

template<class T>
class ProgramStructure {
protected:
	const size_t m_orderedID;
	T m_type;
public:
	ProgramStructure(size_t orderedID, T type)noexcept:
	m_orderedID(orderedID),
	m_type(type)
	{}
	size_t getOrderedID()const noexcept {
		return m_orderedID;
	}
	T getType()const noexcept {
		return m_type;

	}
};

class variableDeclaration : public ProgramStructure<statementType> {
private:
	std::string m_tag;
public:

	
};

struct statement {
	statementType type;
	size_t orderedID;
};

//we define a structure for blocks
class Subprogram: public ProgramStructure<subprogramType>{
	friend class Recognizer;
protected:
	std::string m_tag;
	Subprogram* m_parent;
	std::vector<statement> m_contents;
	//we create the maps for every statement type, allowing access with an orderedID
	std::map<size_t, variableDeclaration> m_variables;
	std::map<size_t, ar>
	std::vector<Subprogram> m_subprograms;
public:
	Subprogram(size_t orderedID, subprogramType type)noexcept;
	void addSubprogram(const Subprogram &subprogram);
};


class Program : public Subprogram {
public:
	Program()noexcept;
};


class Recognizer
{
private:
	Program m_program;
	Parser *m_parser;

	void m_recognizeStatements();
	void m_recognizeBlocks();
public:
	Recognizer(Parser* parser);
};

