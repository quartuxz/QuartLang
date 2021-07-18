#pragma once
#include "Lexer.h"
#include "Parser.h"
#include "Engine.h"
#include "SyntaxValidator.h"
#include "DictionaryLexer.h"



class Ishleng
{
private:

	Logger *m_logger;

	Lexer *m_lexer;
	SyntaxValidator* m_syntaxValidator;
	Parser *m_parser;
	Engine *m_engine;
	std::map<std::string, builtinFunction*> m_extraBuiltins;

	
	std::string m_codeOrFile;
	bool m_isFileTrueIsCodeFalse;
	const DictionaryLexer* m_dictionaryLexer;

	static std::map<std::string, builtinFunction*> m_wideExtraBuiltins;

public:

	Ishleng(Logger* logger, const DictionaryLexer* dict, const std::string& codeOrFile, std::map<std::string, builtinFunction*> extraBuiltins = {}, bool isFileTrueIsCodeFalse = true);


	void lex();
	//throws exception when failed
	void validate();
	void parse();
	runType run();
	
	static void setWideExtraBuiltins(std::map<std::string, builtinFunction*> wideExtraBuiltins )noexcept;
		
	const Lexer* getLexer()const noexcept;
	const Parser* getParser()const noexcept;
	const Engine* getEngine()const noexcept;

	~Ishleng();
};

