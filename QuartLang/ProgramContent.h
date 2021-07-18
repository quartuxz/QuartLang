#pragma once
#include <string>
#include "statements.h"

struct programContent {
	statementType type;
	bool isStatement;
	size_t orderedID;


	std::string getString()const;
};
