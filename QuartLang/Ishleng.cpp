#include "Ishleng.h"
#include <sstream>

#define DELETE_IF_NOT_NULL(x) if(x != nullptr){ delete x; }



Ishleng::Ishleng(Logger* logger, const DictionaryLexer* dict, const std::string& codeOrFile, bool isFileTrueIsCodeFalse):
	m_logger(logger),
	m_dictionaryLexer(dict),
	m_lexer(nullptr),
	m_parser(nullptr),
	m_engine(nullptr),
	m_codeOrFile(codeOrFile),
	m_isFileTrueIsCodeFalse(isFileTrueIsCodeFalse)
{
}

void Ishleng::lex()
{
	DELETE_IF_NOT_NULL(m_lexer);
	m_lexer = new Lexer(m_codeOrFile,m_dictionaryLexer,m_logger, m_isFileTrueIsCodeFalse);
}

void Ishleng::validate()
{
	DELETE_IF_NOT_NULL(m_syntaxValidator);
	m_syntaxValidator = new SyntaxValidator(m_logger,m_lexer->getTokens(),defaultProductions);


	validateResult valRes = m_syntaxValidator->validate();

#ifdef MUST_LOG_ISHLENG
	m_logger->log("ishleng",std::to_string(m_lexer->getTokenPosToLine().size()));

	m_logger->log("ishleng", std::to_string(valRes.tokenPos));


	for (auto x : m_lexer->getTokenPosToLine()) {
		m_logger->log("ishleng, first", std::to_string(x.first));
		m_logger->log("ishleng, second", std::to_string(x.second));
		m_logger->log("ishleng", "");
	}
#endif

	valRes.line = m_lexer->getTokenPosToLine().at(valRes.tokenPos);

	std::stringstream ss;

	if (m_isFileTrueIsCodeFalse) {
		ss << m_codeOrFile;
	}
	else {
		ss << std::endl << "/\\/\\/\\" << std::endl << m_codeOrFile << std::endl << "/\\/\\/\\";
	}


	if (!valRes.success) {
		throw validationError(valRes, ss.str());
	}

}

void Ishleng::parse()
{
	DELETE_IF_NOT_NULL(m_parser);
	m_parser = new Parser(m_lexer,m_logger);
}

runType Ishleng::run()
{
	DELETE_IF_NOT_NULL(m_engine);
	m_engine = new Engine(m_parser->getProgram(),m_dictionaryLexer, m_logger);
	return m_engine->run();
}

const Lexer* Ishleng::getLexer() const noexcept
{
	return m_lexer;
}

const Parser* Ishleng::getParser() const noexcept
{
	return m_parser;
}

const Engine* Ishleng::getEngine() const noexcept
{
	return m_engine;
}

Ishleng::~Ishleng()
{
	DELETE_IF_NOT_NULL(m_parser);
	DELETE_IF_NOT_NULL(m_syntaxValidator);
	DELETE_IF_NOT_NULL(m_lexer);
	DELETE_IF_NOT_NULL(m_engine);
}
