#pragma once
#include <map>
#include <functional>
#include "Parser.h"
#include "DataStructure.h"
#include "builtinFunctions.h"



#define SUBPROGRAM_TYPE_ENUM(extra) extra baseBlock, functionBlock, ifBlock, classBlock
#define BLOCKIFY(x) {x}
#define STATEMENT_TYPE_ENUM variableDeclarationSttt, arithmeticOperationSttt, functionCallSttt
#define ADD_COMMA(x) x##,

enum class subprogramType BLOCKIFY(SUBPROGRAM_TYPE_ENUM());

enum class statementType BLOCKIFY(STATEMENT_TYPE_ENUM);

enum class statementAndSubprogramType BLOCKIFY(SUBPROGRAM_TYPE_ENUM(ADD_COMMA(STATEMENT_TYPE_ENUM)));

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

	virtual ~ProgramStructure(){

	}
};

class variableDeclaration : public ProgramStructure<statementType> {
private:
	std::string m_tag;
	
public:
	variableDeclaration(size_t orderedID)noexcept;
};

class functionCall : public ProgramStructure<statementType> {
private:
	std::string m_functionCalledTag;
	std::map<std::string, DataStructure> m_args;
public:
	functionCall(size_t orderedID, std::string functionCalledTag, std::map<std::string,DataStructure> args);
	functionCall();
	std::string getFunctionCalledTag()const noexcept;
	std::map<std::string, DataStructure> getArgs()const noexcept;
};

struct programContent {
	statementType type;
	bool isStatement;
	size_t orderedID;
};

//we define a structure for scoped blocks
class Subprogram: public ProgramStructure<subprogramType>{
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
	std::map<size_t, Subprogram*> m_subprograms;
public:
	Subprogram(size_t orderedID, subprogramType type)noexcept;
	void addSubprogram(const Subprogram &subprogram);

	std::string getTag()const noexcept;


	

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
public:
	Program()noexcept;


	builtinFunction* getIncludedBuiltin(std::string m_name)const;

	~Program();
};

//turns a stream of tokens into a program class
class Recognizer
{
private:

	Logger *m_logger;

	Program m_program;
	Parser* m_parser;


	size_t m_entryPoint;


	DataStructure m_addLiteral(std::string literalStr, Token token);

	void m_makeProgram();
public:
	Recognizer(Parser* parser, Logger *logger);

	const Program *getProgram()const noexcept;
};
