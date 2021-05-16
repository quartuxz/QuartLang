#pragma once
#include "ProgramStructure.h"
#include "statements.h"
#include "builtinFunctions.h"
#include "Token.h"

struct programContent {
	statementType type;
	bool isStatement;
	size_t orderedID;
};

//we define a structure for scoped blocks
class Subprogram : public ProgramStructure<subprogramType> {
	friend class Recognizer;
protected:
	std::string m_tag;
	Subprogram* m_parent = nullptr;
	//the table of contents, sorted by order of appearance
	std::vector<programContent> m_contents;
	//we create the maps for every statement type, allowing access with an orderedID
	//all of these are allocated dynamically
	std::map<size_t, variableDeclaration*> m_variables;
	std::map<size_t, functionCall*> m_functionCalls;
	std::map<size_t, setOperation*> m_setOperations;
	std::map<size_t, arithmeticOperation*> m_arithmeticOperations;

	std::map<size_t, Subprogram*> m_subprograms;

public:
	Subprogram(size_t orderedID, subprogramType type)noexcept;
	void addSubprogram(const Subprogram& subprogram);

	std::string getTag()const noexcept;

	const variableDeclaration* getVariable(size_t orderedID)const;
	const setOperation* getSetOperation(size_t orderedID)const;

	//all the accessors for statements
	const functionCall* getFunctionCall(size_t orderedID)const;


	const arithmeticOperation* getArithmeticOperation(size_t orderedID)const;

	std::vector<programContent> getContent()const noexcept;
	virtual ~Subprogram();
};

class conditionalBlock : public Subprogram {
private:
	operand m_condition;
	bool m_repeats;
public:
	conditionalBlock(size_t orderedID, const operand& condition, bool repeats);

};

class functionBlock : public Subprogram {

};


class Program : public Subprogram {
	friend class Recognizer;
private:
	//NOTE: the contents of the map are "mutable"
	std::map<std::string, builtinFunction*> m_includedBuiltins;
	//END NOTE.
	std::vector<DataStructure> m_literals;

	const DataStructure& m_addLiteral(std::string literalStr, Token token);
public:
	Program()noexcept;


	builtinFunction* getIncludedBuiltin(std::string m_name)const;

	~Program();
};
