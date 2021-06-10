#pragma once
#include<string>
#include <map>
#include <vector>

#define INT_TYPE_STR "int"
#define FLOAT_TYPE_STR "float"
#define BOOL_TYPE_STR "bool"
#define CHAR_TYPE_STR "char"
#define STRING_TYPE_STR "string"

#define FUNCTION_TYPE_STR "function"
#define BUILTIN_FUNCTION_TYPE_STR "builtinFunction"

class functionBlock;
class builtinFunction;


class DataStructure{
private:
	std::string m_typeOrPrimitiveTag = "";
	bool m_isComposite = false;
	bool m_isArray = false;
	union {
		int m_intData;
		float m_floatData;
		bool m_boolData;
		char m_charData;

	};
	//subobjects addressed by names
	std::vector<DataStructure> m_subobjects = std::vector<DataStructure>();
	std::map<std::string, size_t> m_subobjectNames = std::map<std::string, size_t>();


	const functionBlock *m_function = nullptr;

	builtinFunction* m_builtinFunction = nullptr;


	mutable bool strChanged = true;
	mutable std::string m_strBuffer = "";
public:

	DataStructure(const std::string &typeOrPrimitiveTag, const std::vector<DataStructure>& listElements);
	DataStructure(const std::string &typeOrPrimitiveTag, const std::vector<std::pair<std::string, DataStructure>>& subobjects);

	//constructor for functions
	DataStructure(const functionBlock *function);
	DataStructure(builtinFunction *builtin);

	//construct a string data structure(x subobjects with chars for each string char)
	DataStructure(const std::string &str);
	//constructors for primitive types
	DataStructure(int intData);
	DataStructure(float floatData);
	DataStructure(bool boolData);
	DataStructure(char charData);

	DataStructure();

	bool isEmpty();


	const functionBlock* getFunction()const noexcept;
	const builtinFunction* getBuiltinFunction()const noexcept;
	builtinFunction* getBuiltinFunction() noexcept;



	int getIntData()const noexcept;
	float getFloatData()const noexcept;
	bool getBoolData()const noexcept;
	char getCharData()const noexcept;
	std::string getString()const noexcept;


	bool getIsComposite()const noexcept;
	bool getIsArray()const noexcept;

	std::string getTypeOrPrimitiveTag()const noexcept;

	const DataStructure& getNamedSubobject(std::string name)const;
	const DataStructure& getSubobject(size_t id)const;
	DataStructure& getNamedSubobject(std::string name);
	DataStructure& getSubobject(int id);


	void pushBackSubobject(const DataStructure & subobject);
	void addNamedSubobject(const DataStructure & subobject, const std::string &name );





	~DataStructure();
};


enum class operandType{
	tag, result, literal
};

//a variable a result or a literal
struct operand {
	//constructs a result operand
	operand();
	operand(const std::string &p_tagStr);
	operand(const DataStructure &p_literalData);
	operandType type = operandType::result;
	std::string tagStr;
	DataStructure literalData;
};