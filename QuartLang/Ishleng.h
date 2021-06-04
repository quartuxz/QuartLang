#pragma once
#include "Lexer.h"
#include "Parser.h"
#include "Engine.h"
#include "DictionaryLexer.h"



class Ishleng
{
private:

	Logger *m_logger;

	Lexer *m_lexer;
	Parser *m_parser;
	Engine *m_engine;
	
	std::string m_codeOrFile;
	bool m_isFileTrueIsCodeFalse;
	const DictionaryLexer* m_dictionaryLexer;

public:

	Ishleng(Logger *logger, const DictionaryLexer *dict,const std::string& codeOrFile, bool isFileTrueIsCodeFalse = true);


	void lex();
	void parse();
	runType run();

	const Lexer* getLexer()const noexcept;
	const Parser* getParser()const noexcept;
	const Engine* getEngine()const noexcept;

	~Ishleng();
};

