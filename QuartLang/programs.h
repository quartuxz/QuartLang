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
	std::map<std::string, size_t> m_variableTags;
	std::map<size_t, functionCall*> m_functionCalls;
	std::map<size_t, setOperation*> m_setOperations;
	std::map<size_t, Subprogram*> m_subprograms;

	void m_addVariableDeclaration(variableDeclaration *varDecl);
public:
	Subprogram(size_t orderedID, subprogramType type)noexcept;
	void addSubprogram(const Subprogram& subprogram);

	std::string getTag()const noexcept;



	variableDeclaration* getVariableUpwards(const std::string& tag, size_t upwardsFrom);


	bool checkVariableTagExists(const std::string& tag)const noexcept;
	variableDeclaration* getVariable(const std::string& tag);
	variableDeclaration* getVariable(size_t orderedID);
	setOperation* getSetOperation(size_t orderedID);


	//all the accessors for statements
	const functionCall* getFunctionCall(size_t orderedID)const;


	std::vector<programContent> getContent()const noexcept;
	virtual ~Subprogram();
};


class Program : public Subprogram {
	friend class Recognizer;
private:
	//NOTE: the contents of the map are "mutable"
	std::map<std::string, builtinFunction*> m_includedBuiltins;
	//END NOTE.
	std::vector<int> m_intLiterals;
	std::vector<std::string> m_stringLiterals;
	std::vector<float> m_floatLiterals;
	std::vector<char> m_boolLiterals;
	std::vector<std::string> m_tags;
	std::vector<DataStructure*> m_stackDataStructures;

	DataStructure* m_addLiteral(std::string literalStr, Token token);
	DataStructure* m_addTag(std::string tagStr);
public:
	Program()noexcept;


	builtinFunction* getIncludedBuiltin(std::string m_name)const;

	~Program();
};
