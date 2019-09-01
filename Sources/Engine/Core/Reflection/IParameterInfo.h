/*=============================================================================
IParameterInfo.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IPARAMETERINFO_H_
#define _SE_IPARAMETERINFO_H_

#include "Core/Common.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "Core/Reflection/TypeInfo.h"
#include "Core/Reflection/IMemberInfo.h"

namespace SonataEngine
{

class TypeInfo;
class Object;
class MemberInfo;

enum ParameterAttributes
{
	ParameterAttributes_None = (1<<0),
	ParameterAttributes_In = (1<<1),
	ParameterAttributes_Out = (1<<2)
};

/** Provides access to parameter metadata. */
class SE_CORE_EXPORT IParameterInfo
{
public:
	/** Gets the name of the parameter. */
	virtual String GetName() const = 0;

	/** Gets the attributes associated with this parameter. */
	virtual ParameterAttributes GetParameterAttributes() const = 0;

	/** Gets the member in which the parameter is implemented. */
	virtual IMemberInfo* GetMember() const = 0;

	/** Gets the Type of this parameter. */
	virtual TypeInfo* GetParameterType() const = 0;

	/** Gets the signature position for the parameter. */
	virtual int32 GetPosition() const = 0;
};

}

#endif 
