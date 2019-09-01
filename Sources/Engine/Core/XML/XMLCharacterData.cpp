/*=============================================================================
XMLCharacterData.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "XMLCharacterData.h"

namespace SonataEngine
{

XMLCharacterData::XMLCharacterData() :
	XMLNode()
{
}

XMLCharacterData::~XMLCharacterData()
{
}

String XMLCharacterData::GetValue() const
{
	return _Value;
}

void XMLCharacterData::SetValue(const String& value)
{
	_Value = value;
}

int32 XMLCharacterData::GetLength() const
{
	return _Value.Length();
}

}
