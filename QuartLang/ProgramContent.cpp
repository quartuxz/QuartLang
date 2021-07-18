#include "ProgramContent.h"
#include <sstream>

std::string programContent::getString() const
{
	std::stringstream ss;
	ss << "ordered id: " << orderedID << ", is statement?: " << (isStatement ? "true" : "false") << ", statement type: " << statementTypeToString[type];

	return ss.str();
}