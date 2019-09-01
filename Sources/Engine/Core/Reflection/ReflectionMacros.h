/*=============================================================================
Macros.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_REFLECTION_MACROS_H_
#define _SE_REFLECTION_MACROS_H_

#include "Core/Common.h"

namespace SonataEngine
{

#if SE_USE_REFLECTION

#define typeof(classname) TypeFactory::Instance()->GetType(#classname)

#define is(obj, classname) (obj->GetType() == TypeFactory::Instance()->GetType(#classname))

#define SE_StaticCast(obj, classname) \
	((classname*)obj)

#define SE_DynamicCast(obj, classname) \
	((classname*) (obj && obj->GetType()->IsSubclassOf(classname::GetTypeStatic()) ? obj : NULL))


#define SE_DECLARE_COMMON(classname) \
private: \
	static TypeInfo* Type; \
public: \
	typedef classname object; \
	static String GetTypeName(); \
	static TypeInfo* GetTypeStatic(); \
	static TypeInfo& GetTypeStaticRef(); \
	virtual TypeInfo* GetType() const; \
	static void RegisterType(); \
	static Object* CreateInstance(); \
	static struct classname##AutoRegister \
	{ \
		classname##AutoRegister() \
		{ \
			classname::RegisterType(); \
		} \
	} AutoRegisterInstance;

#define SE_DECLARE_OBJECT(classname) \
	SE_DECLARE_COMMON(classname)

#define SE_DECLARE_BASE(classname, supername) \
	SE_DECLARE_COMMON(classname) \
	typedef supername super;

#define SE_DECLARE_ABSTRACT(classname, supername) \
	SE_DECLARE_BASE(classname, supername)

#define SE_DECLARE_CLASS(classname, supername) \
	SE_DECLARE_BASE(classname, supername)

#define SE_IMPLEMENT_COMMON(classname) \
	TypeInfo* classname::Type = NULL; \
	String classname::GetTypeName() \
	{ \
		return #classname; \
	} \
	TypeInfo* classname::GetTypeStatic() \
	{ \
		return classname::Type; \
	} \
	TypeInfo& classname::GetTypeStaticRef() \
	{ \
		return *classname::Type; \
	} \
	TypeInfo* classname::GetType() const \
	{ \
		return classname::Type; \
	} \

#define SE_IMPLEMENT_OBJECT(classname) \
	SE_IMPLEMENT_COMMON(classname) \
	struct classname::classname##AutoRegister classname::AutoRegisterInstance; \
	void classname::RegisterType() \
	{ \
		static TypeInfo type(#classname, "", classname::CreateInstance); \
		classname::Type = &type; \
		TypeFactory::Instance()->RegisterType(&type); \
	} \
	Object* classname::CreateInstance() \
	{ \
		return NULL; \
	}

#define SE_IMPLEMENT_BASE(classname) \
	SE_IMPLEMENT_COMMON(classname) \
	struct classname::classname##AutoRegister classname::AutoRegisterInstance; \
	void classname::RegisterType() \
	{ \
		static TypeInfo type(#classname, super::GetTypeName(), classname::CreateInstance); \
		classname::Type = &type; \
		TypeFactory::Instance()->RegisterType(&type); \
	}

#define SE_IMPLEMENT_ABSTRACT(classname) \
	SE_IMPLEMENT_BASE(classname) \
	Object* classname::CreateInstance() \
	{ \
		return NULL; \
	}

#define SE_IMPLEMENT_CLASS(classname) \
	SE_IMPLEMENT_BASE(classname) \
	Object* classname::CreateInstance() \
	{ \
		classname* ptr = new classname(); \
		return ptr; \
	}

#define SE_DECLARE_STRUCT(structname) \
	class _##structname : public StructObject \
	{ \
		SE_DECLARE_CLASS(_##structname, StructObject) \
	public: \
		virtual void Get(void* value) \
		{ \
			Memory::Copy(value, &_value, sizeof(structname)); \
		} \
		virtual void Set(void* value) \
		{ \
			Memory::Copy(&_value, value, sizeof(structname)); \
		} \
		structname _value; \
	};

#define SE_IMPLEMENT_STRUCT(structname) \
	SE_IMPLEMENT_COMMON(_##structname) \
	struct _##structname::_##structname##AutoRegister _##structname::AutoRegisterInstance; \
	void _##structname::RegisterType() \
	{ \
		static TypeInfo type(#structname, super::GetTypeName(), _##structname::CreateInstance); \
		_##structname::Type = &type; \
		TypeFactory::Instance()->RegisterType(&type); \
	} \
	Object* _##structname::CreateInstance() \
	{ \
		_##structname* ptr = new _##structname(); \
		return ptr; \
	}


#define SE_BEGIN_REFLECTION(classname) \
private: \
	static struct classname##AutoReflection \
	{ \
		typedef classname object; \
		classname##AutoReflection() \
		{ \
			TypeBuilder builder(#classname);

#define SE_END_REFLECTION(classname) \
		} \
	} AutoReflectionInstance;

#define SE_IMPLEMENT_REFLECTION(classname) \
	struct classname::classname##AutoReflection classname::AutoReflectionInstance;

#define SE_BEGIN_STRUCT(structname) \
	static struct _##structname##AutoReflection \
	{ \
		typedef _##structname object; \
		_##structname##AutoReflection() \
		{ \
			TypeBuilder builder(#structname);

#define SE_END_STRUCT(structname) \
		} \
	} _##structname##AutoReflectionInstance;


#define SE_BEGIN_ENUM(enumname) \
namespace \
{ \
	static struct enumname##AutoReflection \
	{ \
		enumname##AutoReflection() \
		{ \
			EnumBuilder builder(#enumname);

#define SE_END_ENUM(enumname) \
		} \
	} ##enumname##AutoReflectionInstance; \
}

#define SE_EnumValue(enumName, enumValue) \
	builder.DefineEnum(#enumName, Variant(enumValue));

#define SE_Enum(enumName) \
	builder.DefineEnum(#enumName);


#define SE_Field(fieldname, fieldtype, attributes) \
	builder.DefineField(#fieldname, #fieldtype, FieldAttributes_##attributes, SE_OffsetOf(object, fieldname));

#define SE_StructField(fieldname, fieldtype) \
	builder.DefineField(#fieldname, #fieldtype, FieldAttributes_Public, SE_OffsetOf(object, _value.fieldname));


#define SE_Method(methodname, returnType, parameters, attributes) \
	builder.DefineMethod(#methodname, typeof(returnType), parameters, MethodAttributes_##attributes, SE_OffsetOf(object, methodname));

#define SE_StaticMethod(methodname, returnType, parameters, attributes) \
	builder.DefineMethod(#methodname, typeof(returnType), parameters, (MethodAttributes)(MethodAttributes_##attributes | MethodAttributes_Static), SE_OffsetOf(object, methodname));

#else

#define typeof(classname)
#define is(obj, classname)
#define SE_StaticCast(obj, classname)
#define SE_DynamicCast(obj, classname)

#define SE_DECLARE_COMMON(classname)
#define SE_DECLARE_OBJECT(classname)
#define SE_DECLARE_BASE(classname, supername)
#define SE_DECLARE_ABSTRACT(classname, supername)
#define SE_DECLARE_CLASS(classname, supername)

#define SE_IMPLEMENT_COMMON(classname)
#define SE_IMPLEMENT_OBJECT(classname)
#define SE_IMPLEMENT_BASE(classname)
#define SE_IMPLEMENT_ABSTRACT(classname)
#define SE_IMPLEMENT_CLASS(classname)

#define SE_DECLARE_STRUCT(structname)
#define SE_IMPLEMENT_STRUCT(structname)
#define SE_BEGIN_REFLECTION(classname)
#define SE_END_REFLECTION(classname)
#define SE_IMPLEMENT_REFLECTION(classname)
#define SE_BEGIN_STRUCT(structname)
#define SE_END_STRUCT(structname)
#define SE_BEGIN_ENUM(enumname)
#define SE_END_ENUM(enumname)

#define SE_EnumValue(enumName, enumValue)
#define SE_Enum(enumName)
#define SE_Field(fieldname, fieldtype, attributes)
#define SE_StructField(fieldname, fieldtype)
#define SE_Method(methodname, returnType, parameters, attributes)
#define SE_StaticMethod(methodname, returnType, parameters, attributes)

#endif

}

#endif 
