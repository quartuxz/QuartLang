#if ( !defined(DAYS_H) || defined(GENERATE_ENUM_STRINGS) )

#if (!defined(GENERATE_ENUM_STRINGS))
#define DAYS_H
#endif


#include "EnumToString.h"

//they conform an alphabet of symbols that can be used to make
//the language for the program.


BEGIN_ENUM(Token) {
	DECL_ENUM_ELEMENT(lambdaTok),
		DECL_ENUM_ELEMENT(declareTok),
		DECL_ENUM_ELEMENT(functionTok),
		DECL_ENUM_ELEMENT(variableTok),
		DECL_ENUM_ELEMENT(tagTok),
		DECL_ENUM_ELEMENT(floatLiteralTok),
		DECL_ENUM_ELEMENT(intLiteralTok),
		DECL_ENUM_ELEMENT(stringLiteralTok),
		DECL_ENUM_ELEMENT(boolLiteralTok),
		DECL_ENUM_ELEMENT(callFunctionTok),
		DECL_ENUM_ELEMENT(passArgTok),
		DECL_ENUM_ELEMENT(variableTok),
		DECL_ENUM_ELEMENT(thenTok),
		DECL_ENUM_ELEMENT(setTok),
		DECL_ENUM_ELEMENT(endTok)
}; END_ENUM(Token)

#endif