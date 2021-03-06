// File name: "EnumToString.h"
#undef DECL_ENUM_ELEMENT
#undef BEGIN_ENUM
#undef END_ENUM

#ifndef GENERATE_ENUM_STRINGS
#define DECL_ENUM_ELEMENT( element ) element
#define BEGIN_ENUM( ENUM_NAME ) enum class ENUM_NAME
#define END_ENUM( ENUM_NAME ) \
            const char* GetString##ENUM_NAME(ENUM_NAME index);
#else
#define DECL_ENUM_ELEMENT( element ) #element
#define BEGIN_ENUM( ENUM_NAME ) const char* gs_##ENUM_NAME [] =
#define END_ENUM( ENUM_NAME ) const char* GetString##ENUM_NAME( \
            ENUM_NAME index){ return gs_##ENUM_NAME [(size_t)index]; }
#endif

