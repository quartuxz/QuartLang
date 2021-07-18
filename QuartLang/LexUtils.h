#pragma once
#include <string>

#define LEX_NUMBER_OR_BOOL  \
	if (isdigit(currentTokenString[0]) || currentTokenString[0] == '-') { \
		bool isInteger = true; \
		for (auto x : currentTokenString) { \
			if (x == '.') { \
				isInteger = false; \
				m_addLiteral(currentTokenString, Token::floatLiteralTok); \
				break; \
			} \
		} \
			if (isInteger) { \
				m_addLiteral(currentTokenString, Token::intLiteralTok); \
			} \
	} \
	else if (currentTokenString == "true" || currentTokenString == "false") {\
			m_addLiteral(currentTokenString, Token::boolLiteralTok);\
	}