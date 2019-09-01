/*=============================================================================
SampleReflection.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <EntryPoint.h>
#include "SampleReflection.h"

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
	TestObject() : Object() {};

protected:
	String name;
	int32 health;
	real32 size;
	Color32 color;
};

SE_IMPLEMENT_CLASS(TestObject);
SE_IMPLEMENT_REFLECTION(TestObject);

struct TestStruct
{
	String name;
};

SE_DECLARE_STRUCT(TestStruct);
SE_IMPLEMENT_STRUCT(TestStruct);
SE_BEGIN_STRUCT(TestStruct);
	SE_StructField(name, String);
SE_END_STRUCT(TestStruct);

enum TestEnum
{
	TestEnum_Monday,
	TestEnum_Tuesday,
	TestEnum_Wednesday,
	TestEnum_Thursday,
	TestEnum_Friday,
	TestEnum_Saturday,
	TestEnum_Sunday
};

SE_BEGIN_ENUM(TestEnum);
	SE_Enum(Monday);
	SE_Enum(Tuesday);
	SE_Enum(Wednesday);
	SE_Enum(Thursday);
	SE_Enum(Friday);
	SE_Enum(Saturday);
	SE_Enum(Sunday);
SE_END_ENUM(TestEnum);


void DisplayTypes()
{
	TypeInfoList types = TypeFactory::Instance()->GetTypes();
	const TypeInfo* ti;
	foreach (ti, types, TypeInfoList)
	{
		printf("%s", ti->GetName().Data());

		if (ti->IsPrimitive())
			printf(" -Primitive");
		else if (ti->IsEnum())
			printf(" -Enum");
		else if (ti->IsStruct())
			printf(" -Struct");
		else if (ti->IsAbstract())
			printf(" -Abstract");

		printf("\n");

		FieldList fields = ti->GetFields();
		const FieldInfo* fi;
		foreach (fi, fields, FieldList)
		{
			printf("\t%s (%s)\n", fi->GetName().Data(), fi->GetFieldType()->GetName().Data());

			AttributeList attributes = fi->GetAttributes();
			const Attribute* attribute;
			foreach (attribute, attributes, AttributeList)
			{
				printf("\t\t%s\n", "Attribute");
			}
		}
	}
}

void ReflectionTest()
{
	bool exit = false;
	Array<Object*> objects;
	Array<Object*>::Iterator it;
	String typeName, str;
	Object* obj;
	int index;

	const TypeInfo* fieldType;
	FieldList fields;
	FieldList::Iterator fieldIt;
	const FieldInfo* field;

	char buffer[512];

	while (!exit)
	{
		Console::WriteLine("Choices: ");
		Console::WriteLine("----------");
		Console::WriteLine("[q] Quit");
		Console::WriteLine("[l] List");
		Console::WriteLine("[c] Create");
		Console::WriteLine("[d] Delete");
		Console::WriteLine("[v] View");
		Console::WriteLine("[e] Edit");
		Console::WriteLine("----------");
		Console::WriteLine();

		int key = getch();
		switch (key)
		{
		case 27:
		case 'q':
			exit = true;
			break;

		case 'l':
			Console::WriteLine(String::ToString(objects.Count()) + " object(s) created.");
			Console::WriteLine("Objects:");
			it = objects.GetIterator();
			index = 0;
			while (it.Next())
			{
				Console::Write("[" + String::ToString(index) + "] ");
				Console::WriteLine(it.Current()->GetType()->GetName());
				index++;
			}
			break;

		case 'c':
			Console::WriteLine("Enter a type name: ");
			//typeName = Console::ReadLine();
			char buffer[512];
			typeName = fgets(buffer, 512, stdin);
			typeName = typeName.Left(typeName.Length()-1);

			obj = TypeFactory::Instance()->CreateInstance(typeName);
			if (obj == NULL)
			{
				Console::WriteLine("Failed to create the object.");
			}
			else
			{
				Console::WriteLine("Object successfully created.");
				objects.Add(obj);
			}
			break;

		case 'd':
			Console::WriteLine("Enter an object index: ");
			//str = Console::ReadLine();
			str = fgets(buffer, 512, stdin);
			str = str.Left(str.Length()-1);

			index = str.ToInt32();
			if (index < 0 || index >= objects.Count())
			{
				Console::WriteLine("Invalid index.");
			}
			else
			{
				objects.Remove(objects[index]);
				Console::WriteLine("Object successfully deleted.");
			}
			break;

		case 'v':
			Console::WriteLine("Enter an object index: ");
			//str = Console::ReadLine();
			typeName = fgets(buffer, 512, stdin);
			typeName = typeName.Left(typeName.Length()-1);

			index = str.ToInt32();
			if (index < 0 || index >= objects.Count())
			{
				Console::WriteLine("Invalid index.");
			}
			else
			{
				obj = objects[index];
				fields = obj->GetType()->GetFields();

				fieldIt = fields.GetIterator();
				while (fieldIt.Next())
				{
					const TypeInfo* fieldType = fieldIt.Current()->GetFieldType();
					Console::Write(fieldType->GetName() + " ");
					Console::Write(fieldIt.Current()->GetName());
					Console::Write(" = ");

					Variant value = fieldIt.Current()->GetValue(obj);
					switch (value.GetType())
					{
					case Variant_Boolean:
						Console::WriteLine(value.ToBoolean());
						break;
					case Variant_Int32:
						Console::WriteLine(value.ToInt32());
						break;
					case Variant_UInt32:
						Console::WriteLine(value.ToUInt32());
						break;
					case Variant_Int64:
						Console::WriteLine(value.ToInt64());
						break;
					case Variant_UInt64:
						Console::WriteLine(value.ToUInt64());
						break;
					case Variant_Real64:
						Console::WriteLine(value.ToReal64());
						break;
					case Variant_String:
						Console::WriteLine(value.ToString());
						break;
					}
				}
			}
			break;

		case 'e':
			Console::WriteLine("Enter an object index: ");
			//str = Console::ReadLine();
			str = fgets(buffer, 512, stdin);
			str = str.Left(str.Length()-1);

			index = str.ToInt32();
			if (index < 0 || index >= objects.Count())
			{
				Console::WriteLine("Invalid index.");
			}
			else
			{
				obj = objects[index];
				fields = obj->GetType()->GetFields();

				Console::WriteLine("Enter a member name: ");
				//str = Console::ReadLine();
				str = fgets(buffer, 512, stdin);
				str = str.Left(str.Length()-1);

				field = NULL;
				fieldIt = fields.GetIterator();
				while (fieldIt.Next())
				{
					if (fieldIt.Current()->GetName() == str)
					{
						field = fieldIt.Current();
						break;
					}
				}

				if (field == NULL)
				{
					Console::WriteLine("Invalid member name.");
				}
				else
				{
					Console::WriteLine("Enter a value: ");
					//str = Console::ReadLine();
					str = fgets(buffer, 512, stdin);
					str = str.Left(str.Length()-1);

					Variant* value;
					fieldType = field->GetFieldType();
					if (fieldType == typeof(bool))
						value = new Variant(str != "0");
					else if (fieldType == typeof(int8))
						value = new Variant(str.ToInt8());
					else if (fieldType == typeof(uint8))
						value = new Variant(str.ToUInt8());
					else if (fieldType == typeof(int16))
						value = new Variant(str.ToInt16());
					else if (fieldType == typeof(uint16))
						value = new Variant(str.ToUInt16());
					else if (fieldType == typeof(int32))
						value = new Variant(str.ToInt32());
					else if (fieldType == typeof(uint32))
						value = new Variant(str.ToUInt32());
					else if (fieldType == typeof(int64))
						value = new Variant(str.ToInt64());
					else if (fieldType == typeof(uint64))
						value = new Variant(str.ToUInt64());
					else if (fieldType == typeof(real32))
						value = new Variant(str.ToReal32());
					else if (fieldType == typeof(real64))
						value = new Variant(str.ToReal64());
					else if (fieldType == typeof(String))
						value = new Variant(str);

					field->SetValue(obj, *value);
				}
			}
			break;
		}

		Console::WriteLine();
	}
}

void EntryPoint()
{
	Engine::Instance();

	Array<String> str = EnumObject::GetNames(typeof(TestEnum));
	Array<Variant> val = EnumObject::GetValues(typeof(TestEnum));

	DisplayTypes();
	ReflectionTest();

	system("pause");
}
