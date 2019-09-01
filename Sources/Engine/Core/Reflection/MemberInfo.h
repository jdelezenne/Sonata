/*=============================================================================
MemberInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MEMBERINFO_H_
#define _SE_MEMBERINFO_H_

#include "Core/Common.h"
#include "Core/Reflection/IMemberInfo.h"

namespace SonataEngine
{

/** Provides access to member metadata. */
class SE_CORE_EXPORT MemberInfo : public IMemberInfo
{
public:
	/** Destructor. */
	virtual ~MemberInfo()
	{
		const Attribute* attribute;
		foreach (attribute, _Attributes, AttributeList)
		{
			delete attribute;
		}
	}

	virtual TypeInfo* GetDeclaringType() const { return _DeclaringType; }

	virtual const AttributeList& GetAttributes() const { return _Attributes; }

protected:
	/** Constructor. */
	MemberInfo() :
		_DeclaringType(NULL)
	{
	}

protected:
	TypeInfo* _DeclaringType;
	AttributeList _Attributes;
};

}

#endif 
