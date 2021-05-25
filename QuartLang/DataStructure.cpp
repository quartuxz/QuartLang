#include "DataStructure.h"

int DataStructure::getIntData() const noexcept
{
	return m_intData;
}

float DataStructure::getFloatData() const noexcept
{
	return m_floatData;
}

bool DataStructure::getBoolData() const noexcept
{
	return m_boolData;
}

char DataStructure::getCharData() const noexcept
{
	return m_charData;
}

std::string DataStructure::getString() const noexcept
{
	if (strChanged == true) {
		m_strBuffer = "";
		for (auto x : m_subobjects) {
			m_strBuffer.push_back(x.getCharData());
		}
		strChanged = false;
	}
	return m_strBuffer;
}

bool DataStructure::getIsComposite() const noexcept
{
	return m_isComposite;
}

bool DataStructure::getIsArray() const noexcept
{
	return m_isArray;
}


std::string DataStructure::getTypeOrPrimitiveTag() const noexcept
{
	return m_typeOrPrimitiveTag;
}

const DataStructure& DataStructure::getNamedSubobject(std::string name) const
{
	return m_subobjects[m_subobjectNames.at(name)];
}

const DataStructure& DataStructure::getSubobject(size_t id) const
{
	return m_subobjects[id];
}

DataStructure& DataStructure::getNamedSubobject(std::string name)
{
	return m_subobjects[m_subobjectNames[name]];
}

DataStructure& DataStructure::getSubobject(int id)
{
	if (id >= 0) {
		return m_subobjects[id];
	}
	else {
		return m_subobjects[m_subobjects.size() + id];
	}
}

void DataStructure::pushBackSubobject(const DataStructure& subobject)
{
	m_subobjects.push_back(subobject);
	m_isArray = true;
}

void DataStructure::addNamedSubobject(const DataStructure& subobject, const std::string& name)
{
	m_subobjectNames[name] = m_subobjects.size();
	m_subobjects.push_back(subobject);
	m_isComposite = true;
}

DataStructure::DataStructure(int intData):
	m_typeOrPrimitiveTag(INT_TYPE_STR),
	m_intData(intData)
{
}

DataStructure::DataStructure(float floatData):
	m_typeOrPrimitiveTag(FLOAT_TYPE_STR),
	m_floatData(floatData)
{
}

DataStructure::DataStructure(bool boolData):
	m_typeOrPrimitiveTag(BOOL_TYPE_STR),
	m_boolData(boolData)
{

}

DataStructure::DataStructure(char charData):
	m_typeOrPrimitiveTag(CHAR_TYPE_STR),
	m_charData(charData)
{
}

DataStructure::DataStructure(const std::string& typeOrPrimitiveTag, const std::vector<DataStructure>& listElements):
	m_typeOrPrimitiveTag(typeOrPrimitiveTag),
	m_subobjects(listElements),
	m_isArray(true)
{
}

DataStructure::DataStructure()
{
}

bool DataStructure::isEmpty()
{
	return !m_isArray && !m_isComposite && m_typeOrPrimitiveTag.empty();
}

DataStructure::DataStructure(const std::string& typeOrPrimitiveTag, const std::vector<std::pair<std::string, DataStructure>>& subobjects) :
	m_typeOrPrimitiveTag(typeOrPrimitiveTag),
	m_isComposite(true)
{
	for (auto x:subobjects) {
		m_subobjectNames[x.first] = m_subobjects.size();
		m_subobjects.push_back(x.second);
	}
}

DataStructure::DataStructure(const std::string& str):
	m_typeOrPrimitiveTag(STRING_TYPE_STR),
	m_isArray(true)
{
	for (auto x : str) {
		m_subobjects.push_back(DataStructure(x));
	}

}

operand::operand()
{
}

operand::operand(const std::string& p_varName):
	type(operandType::variable),
	varName(p_varName)
{
}

operand::operand(const DataStructure& p_literalData):
	type(operandType::literal),
	literalData(p_literalData)
{
}
