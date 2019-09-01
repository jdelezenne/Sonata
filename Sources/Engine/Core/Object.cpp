/*=============================================================================
Object.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Object.h"

namespace SonataEngine
{

SE_IMPLEMENT_OBJECT(Object);

SE_IMPLEMENT_ABSTRACT(NamedObject);
//SE_IMPLEMENT_REFLECTION(NamedObject);

SE_IMPLEMENT_ABSTRACT(StructObject);

uint32 NamedObject::HashString(const String& str)
{
	uint32 result = 0;        
	const SEchar* pStr = str.Data();
	const SEchar* pChar;

	for (pChar = pStr; *pChar; pChar++)
	{
		result += *pChar * 193951;
		result *= 399283;
	}
	return result;
}

}
