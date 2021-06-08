#pragma once
#include <map>
#include <functional>
#include "Lexer.h"
#include "DataStructure.h"
#include "builtinFunctions.h"
#include "programs.h"


//turns a stream of tokens into a program class
class Parser
{
protected:


	Logger *m_logger;

	Program m_program;
	Lexer* m_lexer;
	

	size_t m_entryPoint;


	std::string lastTag;
	Token currentToken;
	Subprogram* currentProgram = &m_program;
	size_t currentStructure = 0;
	size_t currentScopeNesting = 0;


	std::map<std::string, std::pair<Token,std::string>> m_bindings;

	void m_makeProgram(Lexer *lexer);
	void m_makeProgram();
public:
	Parser(Lexer* lexer, Logger *logger);
	
	static std::vector<functionBlock*> getFunctionsOnly(Lexer *lexer, Logger *logger, const Subprogram *parentProgram = nullptr);


	const Program *getProgram()const noexcept;
	Program *getProgram()noexcept;
};

