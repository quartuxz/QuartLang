#include "DataStructure.h"


DataStructure::DataStructure(const std::string &typeOrPrimitiveTag, const std::map<std::string, DataStructure*>& subobjects, void* data):
	m_typeOrPrimitiveTag(typeOrPrimitiveTag),
	m_subobjects(subobjects),
	m_data(data)
{
}

void* DataStructure::getData() const noexcept
{
	return m_data;
}

DataStructure* DataStructure::getSubobject(std::string name)
{
	return m_subobjects.at(name);
}

std::string DataStructure::getTypeOrPrimitiveTag() const noexcept
{
	return m_typeOrPrimitiveTag;
}
