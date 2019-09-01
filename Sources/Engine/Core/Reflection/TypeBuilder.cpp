/*=============================================================================
TypeBuilder.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TypeBuilder.h"
#include "Core/Reflection/ParameterInfo.h"

namespace SonataEngine
{

TypeBuilder::TypeBuilder(const String& name) :
	_Type()
{
	_Type = (TypeInfo*)TypeFactory::Instance()->GetType(name);
	if (_Type == NULL)
	{
		_Type = new TypeInfo(name);
	}
}

FieldInfo* TypeBuilder::DefineField(const String& fieldName, const String& typeName, FieldAttributes attributes, int offset)
{
	FieldInfo* fi = new FieldInfo(fieldName, typeName, attributes, offset);
	fi->_Attributes = _Attributes;
	fi->_DeclaringType = _Type;
	_Type->_Fields.Add(fi);
	_Attributes.Clear();

	return fi;
}

MethodInfo* TypeBuilder::DefineMethod(const String& methodName, const String& returnTypeName, const ParameterList& parameters, MethodAttributes attributes, int offset)
{
	MethodInfo* mi = new MethodInfo(methodName, returnTypeName, parameters, attributes, offset);
	mi->_Attributes = _Attributes;
	mi->_DeclaringType = _Type;
	_Type->_Methods.Add(mi);
	_Attributes.Clear();

	return mi;
}

MethodInfo* TypeBuilder::DefineMethod(const String& methodName, const String& returnTypeName, const String& parameters, MethodAttributes attributes, int offset)
{
	ParameterList params;
	Array<String> paramsStrings = parameters.Split(_T(','));
	int count = paramsStrings.Count();
	for (int i=0; i<count; i++)
	{
		String paramName;
		String paramType;

		Array<String> paramStrings = paramsStrings[i].Split(_T(' '));
		if (paramStrings.Count() == 1)
		{
			paramType = paramStrings[0];
		}
		else if (paramStrings.Count() == 2)
		{
			paramType = paramStrings[0];
			paramName = paramStrings[1];
		}

		if (!paramType.IsEmpty())
		{
			paramType.Replace(_T("const "), String::Empty);
			paramType.Replace(_T("&"), String::Empty);
			paramType.Replace(_T("*"), String::Empty);
		}

		params.Add(new ParameterInfo(paramName, paramType, ParameterAttributes_In, i));
	}

	MethodInfo* mi = new MethodInfo(methodName, returnTypeName, params, attributes, offset);
	mi->_Attributes = _Attributes;
	mi->_DeclaringType = _Type;
	_Type->_Methods.Add(mi);
	_Attributes.Clear();

	return mi;
}

Attribute* TypeBuilder::SetAttribute(Attribute* attribute)
{
	_Attributes.Add(attribute);

	return attribute;
}

}
