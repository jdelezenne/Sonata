/*=============================================================================
XMLCharacterData.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_XMLCHARACTERDATA_H_
#define _SE_XMLCHARACTERDATA_H_

#include "Core/Common.h"
#include "Core/XML/XMLNode.h"
#include "Core/String.h"

namespace SonataEngine
{

/** Provides text manipulation methods that are used by several classes. */
class SE_CORE_EXPORT XMLCharacterData : public XMLNode
{
public:
	virtual ~XMLCharacterData();

	virtual String GetValue() const;

	virtual void SetValue(const String& value);

	/// Gets the length of the data, in characters.
	virtual int32 GetLength() const;

protected:
	XMLCharacterData();

protected:
	String _Value;
};

}

#endif 
