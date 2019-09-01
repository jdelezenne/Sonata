/*=============================================================================
SampleScript.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}

#include <EntryPoint.h>
#include "SampleScript.h"

class TestObject : public Object
{
	SE_DECLARE_CLASS(TestObject, Object);

	SE_BEGIN_REFLECTION(TestObject);
		SE_Field(name, String, Public);
		SE_Field(health, int32, Public);
		SE_Field(size, real32, Public);
		SE_Field(color, Color32, Public);
	SE_END_REFLECTION(TestObject);

public:
	TestObject() :
		Object(),
		name("name"),
		health(100),
		size(1.0f),
		color(Color32::Red)
	{
	}

	void Print()
	{
		Console::WriteLine("TestObject!");
	}

protected:
	String name;
	int32 health;
	real32 size;
	Color32 color;
};

SE_IMPLEMENT_CLASS(TestObject);
SE_IMPLEMENT_REFLECTION(TestObject);


class Parameter
{
public:
	Parameter(const TypeInfo* type) : Type(type) {}
	String Name;
	const TypeInfo* Type;
};


template<class T>
SE_INLINE const TypeInfo* GetType()
{
	SE_ASSERTFAIL("No handler found.");
	return NULL;
}

template<> SE_INLINE const TypeInfo* GetType<bool>() { return typeof(bool); }
template<> SE_INLINE const TypeInfo* GetType<int8>() { return typeof(int8); }
template<> SE_INLINE const TypeInfo* GetType<uint8>() { return typeof(uint8); }
template<> SE_INLINE const TypeInfo* GetType<int16>() { return typeof(int16); }
template<> SE_INLINE const TypeInfo* GetType<uint16>() { return typeof(uint16); }
template<> SE_INLINE const TypeInfo* GetType<int32>() { return typeof(int32); }
template<> SE_INLINE const TypeInfo* GetType<uint32>() { return typeof(uint32); }
template<> SE_INLINE const TypeInfo* GetType<int64>() { return typeof(int64); }
template<> SE_INLINE const TypeInfo* GetType<uint64>() { return typeof(uint64); }
template<> SE_INLINE const TypeInfo* GetType<real32>() { return typeof(real32); }
template<> SE_INLINE const TypeInfo* GetType<real64>() { return typeof(real64); }
template<> SE_INLINE const TypeInfo* GetType<String>() { return typeof(String); }

template<class T>
SE_INLINE T GetValue(const Variant& v)
{
	SE_ASSERTFAIL("No handler found.");
	return T();
}

template<> SE_INLINE bool GetValue<bool>(const Variant& v) { return v.ToBoolean(); }
template<> SE_INLINE int8 GetValue<int8>(const Variant& v) { return v.ToInt8(); }
template<> SE_INLINE int16 GetValue<int16>(const Variant& v) { return v.ToInt16(); }
template<> SE_INLINE int32 GetValue<int32>(const Variant& v) { return v.ToInt32(); }
template<> SE_INLINE int64 GetValue<int64>(const Variant& v) { return v.ToInt64(); }
template<> SE_INLINE real32 GetValue<real32>(const Variant& v) { return v.ToReal32(); }
template<> SE_INLINE real64 GetValue<real64>(const Variant& v) { return v.ToReal64(); }
template<> SE_INLINE String GetValue<String>(const Variant& v) { return v.ToString(); }
template<> SE_INLINE Object* GetValue<Object*>(const Variant& v) { return v.ToObject(); }
template<> SE_INLINE const String GetValue<const String>(const Variant& v) { return v.ToString(); }

template<class RT>
struct ReturnSpecialization
{
	static const TypeInfo* ReturnType(RT (*func)())
	{
		return GetType<RT>();
	}

	template<class Callee>
	static const TypeInfo* ReturnType(RT (Callee::*func)())
	{
		return GetType<RT>();
	}

	static BaseArray<Parameter> Parameters(RT (*func)())
	{
		return BaseArray<Parameter>();
	}

	template <typename P1>
	static const TypeInfo* ReturnType(RT (*func)(P1))
	{
		return GetType<RT>();
	}

	template <typename P1, typename P2>
	static const TypeInfo* ReturnType(RT (*func)(P1, P2))
	{
		return GetType<RT>();
	}

	template <typename P1>
	static BaseArray<Parameter> Parameters(RT (*func)(P1))
	{
		BaseArray<Parameter> parameters;
		parameters.Add(Parameter(GetType<P1>()));
		return parameters;
	}

	template <typename P1, typename P2>
	static BaseArray<Parameter> Parameters(RT (*func)(P1, P2))
	{
		BaseArray<Parameter> parameters;
		parameters.Add(Parameter(GetType<P1>()));
		parameters.Add(Parameter(GetType<P2>()));
		return parameters;
	}

	static Variant Call(RT (*func)())
	{
		RT ret = func();
		return ret;
	}

	template <typename P1>
	static Variant Call(RT (*func)(P1), const Array<Variant>& values)
	{
		return func(
			GetValue<P1>(values[0])
		);
	}

	template <typename P1, typename P2>
	static Variant Call(RT (*func)(P1, P2), const Array<Variant>& values)
	{
		return func(
			GetValue<P1>(values[0]),
			GetValue<P2>(values[1])
		);
	}

	template<class Callee>
	static Variant Call(Callee& callee, RT (Callee::*func)())
	{
		RT ret = (callee.*func)();
		return ret;
	}
};

template<>
struct ReturnSpecialization<void>
{
	static const TypeInfo* ReturnType(void (*func)())
	{
		return NULL;
	}

	template<class Callee>
	static const TypeInfo* ReturnType(void (Callee::*func)())
	{
		return NULL;
	}

	template <typename P1>
	static const TypeInfo* ReturnType(void (*func)(P1))
	{
		return NULL;
	}

	template <typename P1, typename P2>
	static const TypeInfo* ReturnType(void (*func)(P1, P2))
	{
		return NULL;
	}

	template <typename P1>
	static BaseArray<Parameter> Parameters(void (*func)(P1))
	{
		BaseArray<Parameter> parameters;
		parameters.Add(Parameter(GetType<P1>()));
		return parameters;
	}

	template <typename P1, typename P2>
	static BaseArray<Parameter> Parameters(void (*func)(P1, P2))
	{
		BaseArray<Parameter> parameters;
		parameters.Add(Parameter(GetType<P1>()));
		parameters.Add(Parameter(GetType<P2>()));
		return parameters;
	}

	static Variant Call(void (*func)())
	{
		func();
		return Variant::Invalid;
	}

	template <typename P1>
	static Variant Call(void (*func)(P1), const Array<Variant>& values)
	{
		func(
			GetValue<P1>(values[0])
		);
		return Variant::Invalid;
	}

	template <typename P1, typename P2>
	static Variant Call(void (*func)(P1, P2), const Array<Variant>& values)
	{
		func(
			GetValue<P1>(values[0]),
			GetValue<P2>(values[1])
		);
		return Variant::Invalid;
	}

	template<class Callee>
	static Variant Call(Callee& callee, void (Callee::*func)())
	{
		(callee.*func)();
		return Variant::Invalid;
	}
};

template <typename RT>
const TypeInfo* GetReturnType(RT (*func)())
{
	return ReturnSpecialization<RT>::ReturnType(func);
}

template <typename RT, typename P1>
const TypeInfo* GetReturnType(RT (*func)(P1))
{
	return ReturnSpecialization<RT>::ReturnType(func);
}

template <typename RT, typename P1, typename P2>
const TypeInfo* GetReturnType(RT (*func)(P1, P2))
{
	return ReturnSpecialization<RT>::ReturnType(func);
}

template <typename Callee, typename RT>
const TypeInfo* GetReturnType(RT (Callee::*func)())
{
	return ReturnSpecialization<RT>::ReturnType(func);
}

template <typename Callee, typename RT, typename P1>
const TypeInfo* GetReturnType(RT (Callee::*func)(P1))
{
	return ReturnSpecialization<RT>::ReturnType(func);
}

template <typename Callee, typename RT, typename P1, typename P2>
const TypeInfo* GetReturnType(RT (Callee::*func)(P1, P2))
{
	return ReturnSpecialization<RT>::ReturnType(func);
}

template <typename RT>
BaseArray<Parameter> GetParameters(RT (*func)())
{
	return BaseArray<Parameter>();
}

template <typename RT, typename P1>
BaseArray<Parameter> GetParameters(RT (*func)(P1))
{
	return ReturnSpecialization<RT>::Parameters(func);
}

template <typename RT, typename P1, typename P2>
BaseArray<Parameter> GetParameters(RT (*func)(P1, P2))
{
	return ReturnSpecialization<RT>::Parameters(func);
}

template <typename Callee, typename RT>
BaseArray<Parameter> GetParameters(RT (Callee::*func)())
{
	return BaseArray<Parameter>();
}

template <typename Callee, typename RT, typename P1>
BaseArray<Parameter> GetParameters(RT (Callee::*func)(P1))
{
	return ReturnSpecialization<RT>::Parameters(func);
}

template <typename Callee, typename RT, typename P1, typename P2>
BaseArray<Parameter> GetParameters(RT (Callee::*func)(P1, P2))
{
	return ReturnSpecialization<RT>::Parameters(func);
}

template <typename RT>
Variant Call(RT (*func)(), const Array<Variant>& values)
{
	return ReturnSpecialization<RT>::Call(func);
}

template <typename RT, typename P1>
Variant Call(RT (*func)(P1), const Array<Variant>& values)
{
	return ReturnSpecialization<RT>::Call(func, values);
}

template <typename RT, typename P1, typename P2>
Variant Call(RT (*func)(P1, P2), const Array<Variant>& values)
{
	return ReturnSpecialization<RT>::Call(func, values);
}

template <typename Callee, typename RT>
Variant Call(Callee& callee, RT (Callee::*func)(), const Array<Variant>& values)
{
	return ReturnSpecialization<RT>::Call(callee, func);
}

template <typename Callee, typename RT, typename P1>
Variant Call(Callee& callee, RT (Callee::*func)(P1), const Array<Variant>& values)
{
	return ReturnSpecialization<RT>::Call(callee, func, values);
}

template <typename Callee, typename RT, typename P1, typename P2>
Variant Call(Callee& callee, RT (Callee::*func)(P1, P2), const Array<Variant>& values)
{
	return ReturnSpecialization<RT>::Call(callee, func, values);
}


class MethodBase
{
public:
	String Name;
	const TypeInfo* ReturnType;
	BaseArray<Parameter> Parameters;

	MethodBase()
	{
	}

	virtual Variant Invoke(Object* obj) = 0;
	virtual Variant Invoke(Object* obj, const Array<Variant>& values) = 0;
};

template <typename Func>
class Method : public MethodBase
{
public:
	Func func;

	Method() :
		MethodBase()
	{
		ReturnType = GetReturnType(func);
		Parameters = GetParameters(func);
	}

	virtual Variant Invoke(Object* obj)
	{
		return Call(func, Array<Variant>());
	}

	/*virtual Variant Invoke(int32 count, ...)
	{
		Array<Variant> values;
		va_list args;
		va_start(args, count);

		for (int i=0; i<count; ++i)
		{
			values.Add(va_arg(args, Variant));
		}

		va_end(args);

		return Call(func, values);
	}*/

	virtual Variant Invoke(Object* obj, const Array<Variant>& values)
	{
		return Call(func, values);
	}
};

template <typename Func>
class MethodClass : public MethodBase
{
public:
	Func func;

	MethodClass() :
		MethodBase()
	{
		ReturnType = GetReturnType(func);
		Parameters = GetParameters(func);
	}

	virtual Variant Invoke(Object* obj)
	{
		return Variant();//Call(*obj, func, Array<Variant>());
	}

	virtual Variant Invoke(Object* obj, const Array<Variant>& values)
	{
		return Variant();//Call(*obj, func, values);
	}
};

class ScriptField
{
public:
	String Name;
	const TypeInfo* Type;
	void* Offset;
};

class ScriptClass
{
public:
	String Name;
	Array<ScriptField*> Fields;
	Array<MethodBase*> Methods;
};

SE_INLINE ScriptField* DeclareField(const String& name, const TypeInfo* type, void* offset)
{
	ScriptField* field = new ScriptField();
	field->Name = name;
	field->Type = type;
	field->Offset = offset;
	return field;
}

template <typename Func>
SE_INLINE MethodBase* DeclareMethod(const String& name, Func func)
{
	Method<Func>* method = new Method<Func>();
	method->Name = name;
	method->func = func;
	return method;
}

template <typename Func>
SE_INLINE MethodBase* DeclareMethodClass(const String& name, Func func)
{
	MethodClass<Func>* method = new MethodClass<Func>();
	method->Name = name;
	method->func = func;
	return method;
}

void TestFunction()
{
	Console::WriteLine("test!");
}

bool TestFunctionBool()
{
	Console::WriteLine("test bool!");
	return true;
}

int32 TestFunctionMax(int32 a, int32 b)
{
	Console::WriteLine("test max!");
	return Math::Max(a, b);
}

void TestFunctionPrint(String s)
{
	Console::WriteLine(s);
}

lua_State* _LuaState;

Variant LUAGetValue(lua_State* L, const TypeInfo* type, int nup)
{
	if (type == typeof(bool))
		return (bool)lua_toboolean(L, nup);
	else if (type == typeof(int8))
		return (int8)lua_tonumber(L, nup);
	else if (type == typeof(int16))
		return (int16)lua_tonumber(L, nup);
	else if (type == typeof(int32))
		return (int32)lua_tonumber(L, nup);
	else if (type == typeof(int64))
		return (int64)lua_tonumber(L, nup);
	else if (type == typeof(real32))
		return (real32)lua_tonumber(L, nup);
	else if (type == typeof(real64))
		return (real64)lua_tonumber(L, nup);
	else if (type == typeof(String))
		return (String)lua_tostring(L, nup);
	else
		return Variant::Invalid;
}

void LUASetValue(lua_State* L, const TypeInfo* type, const Variant& value)
{
	if (type == typeof(bool))
		lua_pushboolean(L, value.ToBoolean());
	else if (type == typeof(int8))
		lua_pushnumber(L, value.ToInt8());
	else if (type == typeof(int16))
		lua_pushnumber(L, value.ToInt16());
	else if (type == typeof(int32))
		lua_pushnumber(L, value.ToInt32());
	else if (type == typeof(int64))
		lua_pushnumber(L, value.ToInt64());
	else if (type == typeof(real32))
		lua_pushnumber(L, value.ToReal32());
	else if (type == typeof(real64))
		lua_pushnumber(L, value.ToReal64());
	else if (type == typeof(real))
		lua_pushnumber(L, value.ToReal64());
	else if (type == typeof(String))
		lua_pushstring(L, value.ToString().Data());
}

int DirectCallFunctionDispatcher(lua_State* L)
{
	uchar* buffer = (uchar*)lua_touserdata(L, lua_upvalueindex(1));
	MethodBase* method = *(MethodBase**)buffer;
	int count = lua_gettop(L);
	if (count != method->Parameters.Count())
	{
		Logger::Current()->Log(LogLevel::Error, _T("LUAScriptSystem"),
			_T("Wrong number of parameters."));
		return 0;
	}

	if (count == 0)
	{
		method->Invoke(NULL);
		return 0;
	}
	else
	{
		Array<Variant> values;
		for (int i=0; i<count; ++i)
		{
			values.Add(LUAGetValue(L, method->Parameters[i].Type, i+1));
		}
		
		Variant ret = method->Invoke(NULL, values);

		LUASetValue(L, method->ReturnType, ret);

		return 1;
	}
}

Variant Field_GetValue(ScriptField* field)
{
	void* ptr = field->Offset;

	if (field->Type == typeof(bool))
		return Variant(*(bool*)ptr);
	if (field->Type == typeof(int8))
		return Variant(*(int8*)ptr);
	if (field->Type == typeof(uint8))
		return Variant(*(uint8*)ptr);
	if (field->Type == typeof(int16))
		return Variant(*(int16*)ptr);
	if (field->Type == typeof(uint16))
		return Variant(*(uint16*)ptr);
	if (field->Type == typeof(int32))
		return Variant(*(int32*)ptr);
	if (field->Type == typeof(uint32))
		return Variant(*(uint32*)ptr);
	if (field->Type == typeof(int64))
		return Variant(*(int64*)ptr);
	if (field->Type == typeof(uint64))
		return Variant(*(uint64*)ptr);
	if (field->Type == typeof(real32))
		return Variant(*(real32*)ptr);
	if (field->Type == typeof(real64))
		return Variant(*(real64*)ptr);
	if (field->Type == typeof(real))
		return Variant(*(real*)ptr);
	if (field->Type == typeof(String))
		return Variant(*(String*)ptr);
	if (field->Type->IsStruct())
		return Variant((StructObject*)ptr);
	if (field->Type->IsClass())
	{
		Object** ppObj = (Object**)ptr;
		if (ppObj != NULL)
			return Variant((Object*)*ppObj);
		else
			return Variant((Object*)NULL);
	}
	if (field->Type->IsEnum())
		return Variant((void*)*(int32*)ptr, Variant_Enum);

	return Variant::Invalid;
}

void RegisterLUAField(ScriptField* field, int nup = LUA_GLOBALSINDEX)
{
	lua_pushstring(_LuaState, field->Name.Data());

	LUASetValue(_LuaState, field->Type, Field_GetValue(field));

	lua_settable(_LuaState, nup);
}

void RegisterLUAFunction(MethodBase* method, int nup = LUA_GLOBALSINDEX)
{
	lua_pushstring(_LuaState, method->Name.Data());

	uchar* buffer = (uchar*)lua_newuserdata(_LuaState, sizeof(method));
	memcpy(buffer, &method, sizeof(method));
	lua_pushcclosure(_LuaState, DirectCallFunctionDispatcher, 1);

	lua_settable(_LuaState, nup);
}

ScriptClass* DeclareClass(const String& name, const Array<ScriptField*>& fields, const Array<MethodBase*>& methods)
{
	ScriptClass* cls = new ScriptClass();
	cls->Name = name;
	cls->Fields = fields;
	cls->Methods = methods;
	return cls;
}

void RegisterLUAClass(ScriptClass* cls)
{
	int i;

	lua_newtable(_LuaState);
	lua_pushstring(_LuaState, cls->Name.Data());
	lua_pushvalue(_LuaState, -2);
	lua_settable(_LuaState, LUA_GLOBALSINDEX);
	lua_insert(_LuaState, -1);

	for (i=0; i<cls->Fields.Count(); i++)
	{
		RegisterLUAField(cls->Fields[i], -3);
	}

	for (i=0; i<cls->Methods.Count(); i++)
	{
		RegisterLUAFunction(cls->Methods[i], -3);
	}
}

ScriptClass* DeclareTestClass()
{
	Array<MethodBase*> methods;
	methods.Add(DeclareMethodClass("Print", &TestObject::Print));
	return DeclareClass("TestObject", Array<ScriptField*>(), methods);
}

ScriptClass* DeclareMathClass()
{
	Array<ScriptField*> fields;
	fields.Add(DeclareField("Epsilon", typeof(real), (void*)&Math::Epsilon));
	fields.Add(DeclareField("PI", typeof(real), (void*)&Math::PI));

	Array<MethodBase*> methods;
	methods.Add(DeclareMethod("Sqrt", &Math::Sqrt));
	methods.Add(DeclareMethod("Pow", &Math::Pow));
	methods.Add(DeclareMethod("Exp", &Math::Exp));
	methods.Add(DeclareMethod("Cos", &Math::Cos));
	methods.Add(DeclareMethod("Sin", &Math::Sin));
	methods.Add(DeclareMethod("Tan", &Math::Tan));
	return DeclareClass("Math", fields, methods);
}

void EntryPoint()
{
	Engine::Instance();

#ifdef SE_STATIC
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Debug"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\Release"));
#	endif
#else
#	ifdef _DEBUG
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\DebugDLL"));
#	else
		Environment::SetCurrentDirectory(_T("..\\..\\..\\Build\\ReleaseDLL"));
#	endif
#endif

	Array<MethodBase*> Methods;

	Methods.Add(DeclareMethod("TestFunction", &TestFunction));
	Methods.Add(DeclareMethod("TestFunctionBool", &TestFunctionBool));
	Methods.Add(DeclareMethod("TestFunctionMax", &TestFunctionMax));
	Methods.Add(DeclareMethod("TestFunctionPrint", &TestFunctionPrint));

	ScriptClass* testClass = DeclareTestClass();

	ScriptClass* mathClass = DeclareMathClass();


/*
	Methods[0]->Invoke();
	bool bret = Methods[1]->Invoke().ToBoolean();
	Array<Variant> values; values.Add(10); values.Add(15);
	int32 iret = Methods[2]->Invoke(values).ToInt32();
	Array<Variant> values2; values2.Add(10);;
	Methods[3]->Invoke(values2);
*/

	_LuaState = lua_open();
	lua_settop(_LuaState, 0);

	for (int i=0; i<Methods.Count(); i++)
		RegisterLUAFunction(Methods[i]);

	RegisterLUAClass(testClass);
	RegisterLUAClass(mathClass);

	//TypeBuilder* tb = scriptSystem->DefineType("", NULL, );
	//tb->DefineField();

    lua_dofile(_LuaState, "luatest.lua");

	lua_pushstring(_LuaState, "luatest");
	lua_gettable(_LuaState, LUA_GLOBALSINDEX);
	lua_call(_LuaState, 0, 0);

	lua_close(_LuaState);

	Console::Read();
}
