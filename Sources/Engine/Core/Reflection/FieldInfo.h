/*=============================================================================
FieldInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FIELDINFO_H_
#define _SE_FIELDINFO_H_

#include "Core/Common.h"
#include "Core/Reflection/MemberInfo.h"
#include "Core/Reflection/IFieldInfo.h"

namespace SonataEngine
{

/** Provides access to field metadata. */
class SE_CORE_EXPORT FieldInfo : public MemberInfo, public IFieldInfo
{
public:
	/** Destructor. */
	virtual ~FieldInfo();

	virtual MemberTypes GetMemberType() const;
	virtual String GetName() const;

	virtual FieldAttributes GetFieldAttributes() const;

	virtual TypeInfo* GetFieldType() const;

	virtual Variant GetValue(Object* obj) const;

	virtual void SetValue(Object* obj, const Variant& value) const;

protected:
	/** Constructor. */
	FieldInfo();

	/** Constructor. */
	FieldInfo(const String& fieldName, const String& fieldTypeName, FieldAttributes attributes, int offset);

protected:
	String _FieldName;
	FieldAttributes _FieldAttributes;
	String _FieldTypeName;
	mutable TypeInfo* _FieldType;
	int _Offset;
	mutable Variant _Value;

	friend class TypeBuilder;
	friend class EnumBuilder;
};

#include "FieldInfo.inl"

}

#endif 
