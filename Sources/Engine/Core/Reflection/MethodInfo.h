/*=============================================================================
MethodInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_METHODINFO_H_
#define _SE_METHODINFO_H_

#include "Core/Common.h"
#include "Core/Reflection/MemberInfo.h"
#include "Core/Reflection/IMethodInfo.h"

namespace SonataEngine
{

/** Provides access to method metadata. */
class SE_CORE_EXPORT MethodInfo : public MemberInfo, public IMethodInfo
{
public:
	/** Destructor. */
	virtual ~MethodInfo();

	virtual MemberTypes GetMemberType() const;
	virtual String GetName() const;

	virtual MethodAttributes GetMethodAttributes() const;

	virtual TypeInfo* GetReturnType() const;

	virtual const ParameterList& GetParameters() const;

	virtual Variant* Invoke(Object* obj, Array<Variant>* parameters);

protected:
	/** Constructor. */
	MethodInfo();

	/** Constructor. */
	MethodInfo(const String& methodName, const String& returnTypeName, const ParameterList& parameters, MethodAttributes attributes, int offset);

protected:
	String _MethodName;
	MethodAttributes _MethodAttributes;
	String _ReturnTypeName;
	mutable TypeInfo* _ReturnType;
	int _Offset;
	ParameterList _Parameters;

	friend class TypeBuilder;
};

#include "MethodInfo.inl"

}

#endif 
