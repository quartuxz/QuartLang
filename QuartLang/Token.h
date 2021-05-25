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
	DECL_ENUM_ELEMENT(charLiteralTok),
	DECL_ENUM_ELEMENT(callFunctionTok),
	DECL_ENUM_ELEMENT(passArgTok),
	DECL_ENUM_ELEMENT(setTok),
	DECL_ENUM_ELEMENT(resultTok),
	DECL_ENUM_ELEMENT(addTok),
	DECL_ENUM_ELEMENT(subtractTok),
	DECL_ENUM_ELEMENT(multiplyTok),
	DECL_ENUM_ELEMENT(divideTok),
	DECL_ENUM_ELEMENT(ifTok),
	DECL_ENUM_ELEMENT(finallyTok),
	DECL_ENUM_ELEMENT(endBlockTok),
	DECL_ENUM_ELEMENT(repeatTok),
	DECL_ENUM_ELEMENT(giveTok),
	DECL_ENUM_ELEMENT(evaluateTok),
	DECL_ENUM_ELEMENT(lessThanTok),
	DECL_ENUM_ELEMENT(moreThanTok),
	DECL_ENUM_ELEMENT(equalToTok),
	DECL_ENUM_ELEMENT(orTok),
	DECL_ENUM_ELEMENT(andTok),
	DECL_ENUM_ELEMENT(flipTok),
	DECL_ENUM_ELEMENT(referTok),
	DECL_ENUM_ELEMENT(appendTok),
	DECL_ENUM_ELEMENT(backTok),
	DECL_ENUM_ELEMENT(frontTok),
	DECL_ENUM_ELEMENT(itTok),
	DECL_ENUM_ELEMENT(launchTok),
	DECL_ENUM_ELEMENT(finishTok),
	DECL_ENUM_ELEMENT(commentTok),
	DECL_ENUM_ELEMENT(endTok)
}; END_ENUM(Token)

#endif