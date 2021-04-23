#include "Recognizer.h"

void Subprogram::addSubprogram()
{
}


template<class T>
T ProgramStructure<T>::getType() const noexcept
{
	return T();
}

Recognizer::Recognizer(Parser* parser):
	m_parser(parser),
	m_program(0,subprogramType::baseBlock)
{

}

Subprogram::Subprogram(size_t orderedID, subprogramType type) noexcept:
	ProgramStructure(orderedID, type)
{
}

void Subprogram::addSubprogram(const Subprogram& subprogram)
{
}

Program::Program() noexcept:
	Subprogram(0,subprogramType::baseBlock)
{
}
