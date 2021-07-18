#pragma once
#include <map>
#include <string>

#define SUBPROGRAM_TYPE_ENUM(extra) extra baseBlock, functionBlock, conditionalBlock
#define BLOCKIFY(x) {x}
#define STATEMENT_TYPE_ENUM notASttt, variableDeclarationSttt, arithmeticOperationSttt, functionCallSttt, setOperationSttt, finallySttt, evaluateOperationSttt, flipOperationSttt, referOperationSttt, appendOperationSttt, finishSttt, importSttt
#define ADD_COMMA(x) x##,

enum class subprogramType BLOCKIFY(SUBPROGRAM_TYPE_ENUM( ));

enum class statementType BLOCKIFY(STATEMENT_TYPE_ENUM);

enum class statementAndSubprogramType BLOCKIFY(SUBPROGRAM_TYPE_ENUM(ADD_COMMA(STATEMENT_TYPE_ENUM)));


extern std::map<statementType, std::string> statementTypeToString;

template<class T>
class ProgramStructure {
protected:
	const size_t m_orderedID;
	T m_type;
public:
	ProgramStructure(size_t orderedID, T type)noexcept :
		m_orderedID(orderedID),
		m_type(type)
	{}
	size_t getOrderedID()const noexcept {
		return m_orderedID;
	}
	T getType()const noexcept {
		return m_type;
	}

	virtual ~ProgramStructure() {

	}
};