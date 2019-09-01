/*=============================================================================
Enum.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ENUM_H_
#define _SE_ENUM_H_

#include "Core/Common.h"
#include "Core/Reflection/FieldInfo.h"

namespace SonataEngine
{

/** Defines and creates an enumeration type. */
class SE_CORE_EXPORT EnumBuilder
{
public:
	EnumBuilder(const String& name);

	FieldInfo* DefineEnum(const String& enumName, const Variant& enumValue);
	FieldInfo* DefineEnum(const String& enumName);

protected:
	TypeInfo* _Type;
	int32 _Index;
};

}

#endif 
