#pragma once
#include<string>
#include <map>
#include <vector>


class DataStructure{
private:
	std::string m_typeOrPrimitiveTag = "";
	void* m_data = nullptr;
	//subobjects addressed by names
	std::map<std::string, DataStructure*> m_subobjects = std::map<std::string, DataStructure*>();
public:
	DataStructure(const std::string &typeOrPrimitiveTag, const std::map<std::string, DataStructure*>& subobjects, void*data = nullptr);

	DataStructure();

	void* getData()const noexcept;

	std::string getTypeOrPrimitiveTag()const noexcept;

	DataStructure* getSubobject(std::string name);
};
