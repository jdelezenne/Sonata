/*=============================================================================
IMethodInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMETHODINFO_H_
#define _SE_IMETHODINFO_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "Core/Reflection/TypeInfo.h"
#include "Core/Reflection/IParameterInfo.h"

namespace SonataEngine
{

class TypeInfo;
class Object;

enum MethodAttributes
{
	MethodAttributes_Private = (1<<0),
	MethodAttributes_Public = (1<<1),
	MethodAttributes_Static = (1<<2),
	MethodAttributes_HasDefault = (1<<3)
};

typedef Array<IParameterInfo*> ParameterList;

/** Provides access to method metadata. */
class SE_CORE_EXPORT IMethodInfo
{
public:
	/** Gets the attributes associated with this method. */
	virtual MethodAttributes GetMethodAttributes() const = 0;

	/** Gets the return type of this method. */
	virtual TypeInfo* GetReturnType() const = 0;

	/** Gets the parameters of this method. */
	virtual const ParameterList& GetParameters() const = 0;

	/**
		Invokes the method.
		@param obj The object on which to invoke the method.
		@param parameters An argument list for the invoked method.
		@return An object containing the return value of the invoked method.
	*/
	virtual Variant* Invoke(Object* obj, Array<Variant>* parameters) = 0;
};

}

#endif 
