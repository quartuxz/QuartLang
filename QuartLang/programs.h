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
	friend class Parser;
protected:
	Subprogram* m_parent = nullptr;
	//the table of contents, sorted by order of appearance
	std::vector<programContent> m_contents;
	//we create the maps for every statement type, allowing access with an orderedID
	//all of these are allocated dynamically
	std::map<size_t, variableDeclaration*> m_variables;
	std::map<size_t, functionCall*> m_functionCalls;
	std::map<size_t, setOperation*> m_setOperations;
	std::map<size_t, arithmeticOperation*> m_arithmeticOperations;
	std::map<size_t, finallySttt*> m_finallySttts;
	std::map<size_t, evaluateOperation*> m_evaluateOperations;
	std::map<size_t, flipOperation*> m_flipOperations;
	std::map<size_t, referOperation*> m_referOperations;
	std::map<size_t, appendOperation*> m_appendOperations;
	std::map<size_t, finishOperation*> m_finishOperations;

	std::map<size_t, Subprogram*> m_subprograms;

	void m_addSubprogram(size_t orderedID, Subprogram *subprogram);

public:
	Subprogram(size_t orderedID, subprogramType type)noexcept;

	std::string getTag()const noexcept;

	const Subprogram* getSubprogram(size_t orderedID)const;



	//all the accessors for statements
	const variableDeclaration* getVariable(size_t orderedID)const;
	const setOperation* getSetOperation(size_t orderedID)const;

	const evaluateOperation* getEvaluateOperation(size_t orderedID)const;
	const flipOperation* getFlipOperation(size_t orderedID)const;

	const referOperation* getReferOperation(size_t orderedID)const;
	const appendOperation* getAppendOperation(size_t orderedID)const;

	const functionCall* getFunctionCall(size_t orderedID)const;

	const finallySttt* getFinallySttt(size_t orderedID)const;

	const arithmeticOperation* getArithmeticOperation(size_t orderedID)const;

	const finishOperation* getFinishOperation(size_t orderedID)const;


	std::vector<programContent> getContent()const noexcept;
	virtual ~Subprogram();
};



class conditionalBlock : public Subprogram {
	friend class Parser;
private:
	operand m_condition;
public:
	conditionalBlock(size_t orderedID, const operand& condition);
	const operand& getCondition()const noexcept;
};

class functionBlock : public Subprogram {
private:
	std::string m_tag;
public:
	functionBlock(size_t orderedID, const std::string& tag);
	const std::string& getTag()const;
};


class Program : public Subprogram {
	friend class Parser;
private:
	//NOTE: the contents of the map are "mutable"
	std::map<std::string, builtinFunction*> m_includedBuiltins;
	//END NOTE.
	std::vector<DataStructure> m_literals;
public:
	Program()noexcept;



	builtinFunction* getIncludedBuiltin(std::string m_name)const;

	~Program();
};

