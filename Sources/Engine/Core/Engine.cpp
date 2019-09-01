/*=============================================================================
Engine.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Engine.h"

namespace SonataEngine
{

#define SE_REFLECTION_PRIMITIVE(t) \
	TypeFactory::Instance()->RegisterType(new TypeInfo(#t));

namespace
{
	static struct PrimitiveTypes
	{
		PrimitiveTypes()
		{
			SE_REFLECTION_PRIMITIVE(bool);
			SE_REFLECTION_PRIMITIVE(uchar);
			SE_REFLECTION_PRIMITIVE(char);
			SE_REFLECTION_PRIMITIVE(byte);
			SE_REFLECTION_PRIMITIVE(uint8);
			SE_REFLECTION_PRIMITIVE(int8);
			SE_REFLECTION_PRIMITIVE(uint32);
			SE_REFLECTION_PRIMITIVE(int32);
			SE_REFLECTION_PRIMITIVE(uint64);
			SE_REFLECTION_PRIMITIVE(int64);
			SE_REFLECTION_PRIMITIVE(real);
			SE_REFLECTION_PRIMITIVE(real32);
			SE_REFLECTION_PRIMITIVE(real64);
			SE_REFLECTION_PRIMITIVE(String);
		}
	} PrimitiveTypesInstance;

	SE_DECLARE_STRUCT(PointInt);
	SE_IMPLEMENT_STRUCT(PointInt);
	SE_BEGIN_STRUCT(PointInt);
		SE_StructField(X, int32);
		SE_StructField(Y, int32);
	SE_END_STRUCT(PointInt);

	SE_DECLARE_STRUCT(SizeInt);
	SE_IMPLEMENT_STRUCT(SizeInt);
	SE_BEGIN_STRUCT(SizeInt);
		SE_StructField(Width, int32);
		SE_StructField(Height, int32);
	SE_END_STRUCT(SizeInt);

	SE_DECLARE_STRUCT(RectangleInt);
	SE_IMPLEMENT_STRUCT(RectangleInt);
	SE_BEGIN_STRUCT(RectangleInt);
		SE_StructField(X, int32);
		SE_StructField(Y, int32);
		SE_StructField(Width, int32);
		SE_StructField(Height, int32);
	SE_END_STRUCT(RectangleInt);

	SE_DECLARE_STRUCT(PointReal);
	SE_IMPLEMENT_STRUCT(PointReal);
	SE_BEGIN_STRUCT(PointReal);
		SE_StructField(X, real32);
		SE_StructField(Y, real32);
	SE_END_STRUCT(PointReal);

	SE_DECLARE_STRUCT(SizeReal);
	SE_IMPLEMENT_STRUCT(SizeReal);
	SE_BEGIN_STRUCT(SizeReal);
		SE_StructField(Width, real32);
		SE_StructField(Height, real32);
	SE_END_STRUCT(SizeReal);

	SE_DECLARE_STRUCT(RectangleReal);
	SE_IMPLEMENT_STRUCT(RectangleReal);
	SE_BEGIN_STRUCT(RectangleReal);
		SE_StructField(X, real32);
		SE_StructField(Y, real32);
		SE_StructField(Width, real32);
		SE_StructField(Height, real32);
	SE_END_STRUCT(RectangleReal);

	SE_DECLARE_STRUCT(Color8);
	SE_IMPLEMENT_STRUCT(Color8);
	SE_BEGIN_STRUCT(Color8);
		SE_Attribute(DisplayNameAttribute("Red"));
		SE_Attribute(DescriptionAttribute("Red component"));
		SE_StructField(R, uint8);

		SE_Attribute(DisplayNameAttribute("Green"));
		SE_Attribute(DescriptionAttribute("Green component"));
		SE_StructField(G, uint8);

		SE_Attribute(DisplayNameAttribute("Blue"));
		SE_Attribute(DescriptionAttribute("Blue component"));
		SE_StructField(B, uint8);

		SE_Attribute(DisplayNameAttribute("Alpha"));
		SE_Attribute(DescriptionAttribute("Alpha component"));
		SE_StructField(A, uint8);
	SE_END_STRUCT(Color8);

	SE_DECLARE_STRUCT(Color32);
	SE_IMPLEMENT_STRUCT(Color32);
	SE_BEGIN_STRUCT(Color32);
		SE_Attribute(DisplayNameAttribute("Red"));
		SE_Attribute(DescriptionAttribute("Red component"));
		SE_StructField(R, real32);

		SE_Attribute(DisplayNameAttribute("Green"));
		SE_Attribute(DescriptionAttribute("Green component"));
		SE_StructField(G, real32);

		SE_Attribute(DisplayNameAttribute("Blue"));
		SE_Attribute(DescriptionAttribute("Blue component"));
		SE_StructField(B, real32);

		SE_Attribute(DisplayNameAttribute("Alpha"));
		SE_Attribute(DescriptionAttribute("Alpha component"));
		SE_StructField(A, real32);
	SE_END_STRUCT(Color32);

	SE_DECLARE_STRUCT(Vector2);
	SE_IMPLEMENT_STRUCT(Vector2);
	SE_BEGIN_STRUCT(Vector2);
		SE_StructField(X, real32);
		SE_StructField(Y, real32);
	SE_END_STRUCT(Vector2);

	SE_DECLARE_STRUCT(Vector3);
	SE_IMPLEMENT_STRUCT(Vector3);
	SE_BEGIN_STRUCT(Vector3);
		SE_StructField(X, real32);
		SE_StructField(Y, real32);
		SE_StructField(Z, real32);
	SE_END_STRUCT(Vector3);

	SE_DECLARE_STRUCT(Vector4);
	SE_IMPLEMENT_STRUCT(Vector4);
	SE_BEGIN_STRUCT(Vector4);
		SE_StructField(X, real32);
		SE_StructField(Y, real32);
		SE_StructField(Z, real32);
		SE_StructField(W, real32);
	SE_END_STRUCT(Vector4);

	SE_DECLARE_STRUCT(Quaternion);
	SE_IMPLEMENT_STRUCT(Quaternion);
	SE_BEGIN_STRUCT(Quaternion);
		SE_StructField(X, real32);
		SE_StructField(Y, real32);
		SE_StructField(Z, real32);
		SE_StructField(W, real32);
	SE_END_STRUCT(Quaternion);
}

Engine::Engine()
{
}

Engine::~Engine()
{
	TypeFactory::DestroyInstance();
	FileSystem::DestroyInstance();
	ResourceManager::DestroyInstance();
	PluginManager::DestroyInstance();
}

}
