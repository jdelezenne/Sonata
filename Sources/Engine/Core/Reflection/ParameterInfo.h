/*=============================================================================
ParameterInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PARAMETERINFO_H_
#define _SE_PARAMETERINFO_H_

#include "Core/Common.h"
#include "Core/Reflection/IParameterInfo.h"

namespace SonataEngine
{

/** Provides access to parameter metadata. */
class SE_CORE_EXPORT ParameterInfo : public IParameterInfo
{
public:
	/** Constructor. */
	ParameterInfo();

	virtual String GetName() const;

	virtual ParameterAttributes GetParameterAttributes() const;

	virtual IMemberInfo* GetMember() const;

	virtual TypeInfo* GetParameterType() const;

	virtual int32 GetPosition() const;

protected:
	/** Constructor. */
	ParameterInfo(const String& paramName, const String& paramTypeName, ParameterAttributes attributes, int position);

protected:
	const String _ParameterName;
	ParameterAttributes _ParameterAttributes;
	IMemberInfo* _Member;
	String _ParameterTypeName;
	mutable TypeInfo* _ParameterType;
	int _Position;

	friend class TypeBuilder;
	friend class MethodBuilder;
};

#include "ParameterInfo.inl"

}

#endif 
