#include "ProgramStructure.h"

std::map<statementType, std::string> statementTypeToString = {
	{statementType::appendOperationSttt, "append operation statement"},
	{statementType::arithmeticOperationSttt, "arithmetic operation statement"},
	{statementType::evaluateOperationSttt, "evaluate operation statement"},
	{statementType::finallySttt, "finally statement"},
	{statementType::finishSttt, "finish statement"},
	{statementType::flipOperationSttt, "flip operation statement"},
	{statementType::functionCallSttt, "function call statement"},
	{statementType::importSttt, "import statement"},
	{statementType::notASttt, "not a statement"},
	{statementType::referOperationSttt, "refer operation statement"},
	{statementType::setOperationSttt, "set operation statement"},
	{statementType::variableDeclarationSttt, "variable declaration statement"}
	
};
