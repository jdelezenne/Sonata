/*=============================================================================
SampleSerialization.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SampleSerialization.h"

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

class TestObject : public Object
{
	SE_DECLARE_CLASS(TestObject, Object);

	SE_BEGIN_REFLECTION(TestObject);
		SE_Field(name, String, Public);
		SE_Field(health, int32, Public);
		SE_Field(size, real32, Public);
		SE_Field(pos, Vector2, Public);
		SE_Field(day, TestEnum, Public);
		SE_Field(other, TestObject, Public);
	SE_END_REFLECTION(TestObject);

public:
	TestObject() : Object(), other(NULL) {};
	~TestObject() { SE_DELETE(other) };
	void Print()
	{
		Console::WriteLine("name=" + name);
		Console::WriteLine("health=" + String::ToString(health));
		Console::WriteLine("size=" + String::ToString(size));
		Console::WriteLine("pos=" + pos.ToString());
		Console::WriteLine("day=" + EnumObject::GetName(typeof(TestEnum), Variant((int32)day)));
		Console::Write("indices=");
		for (int i=0; i<indices.Count(); i++)
			Console::Write(indices[i]);
		Console::WriteLine();
		Console::Write("other=");
		if (other != NULL)
			other->Print();
		else
			Console::WriteLine("NULL");
	}

	virtual void OnSerialized(BinarySerializer* context, BinaryStream& stream)
	{
		int count = indices.Count();
		stream.WriteInt32(count);
		for (int i=0; i<count; i++)
		{
			stream.WriteInt32(indices[i]);
		}
	}

	virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream)
	{
		int count = stream.ReadInt32();
		for (int i=0; i<count; i++)
		{
			indices.Add(stream.ReadInt32());
		}
	}

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element)
	{
		XMLDocument* document = element->GetOwnerDocument();
		XMLElement* array = document->CreateElement("indices");
		array->SetAttribute("Count", String::ToString(indices.Count()));
		for (int i=0; i<indices.Count(); i++)
		{
			XMLElement* item = document->CreateElement("index");
			XMLText* value = document->CreateTextNode(String::Empty);
			value->SetValue(String::ToString(indices[i]));
			item->AppendChild(value);
			array->AppendChild(item);
		}
		element->AppendChild(array);
	}

	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element)
	{
		XMLElement* array = (XMLElement*)element->GetItem("indices");
		int count = array->GetAttribute("Count").ToInt32();
		XMLElement* item = (XMLElement*)array->GetItem("index");
		for (int i=0; i<count; i++)
		{
			XMLText* value = (XMLText*)item->GetFirstChild();
			indices.Add(value->GetValue().ToInt32());
			item = (XMLElement*)item->GetNextSibling();
		}
	}

public:
	String name;
	int32 health;
	real32 size;
	Vector2 pos;
	TestEnum day;
	Array<int32> indices;
	TestObject* other;
};

SE_IMPLEMENT_CLASS(TestObject);
SE_IMPLEMENT_REFLECTION(TestObject);

void EntryPoint()
{
	Engine::Instance();
	FileSystem::Instance()->AddRootPath(_T("Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));
	Console::WriteLine();

	String pathBin = FileSystem::Instance()->GetFullPath("serialization.bin");
	String pathXml = FileSystem::Instance()->GetFullPath("serialization.xml");

	TestObject* obj = new TestObject();
	obj->name = "myName";
	obj->health = 200;
	obj->size = 12.5f;
	obj->pos = Vector2(2.0f, 5.0f);
	obj->day = TestEnum_Tuesday;
	obj->indices.Add(1);
	obj->indices.Add(2);
	obj->indices.Add(3);
	obj->other = new TestObject();
	obj->other->name = "myOther";
	obj->other->health = 100;
	obj->other->size = 6.25f;
	obj->other->pos = Vector2(1.0f, 2.5f);
	obj->other->day = TestEnum_Friday;
	obj->other->other = NULL;

	Console::WriteLine("> Before serialization.");
	obj->Print();
	Console::WriteLine();

	File* fileBin = new File(pathBin);
	FileStreamPtr streamBin = fileBin->Open(FileMode_Create);
	if (streamBin == NULL)
		return;

	// Binary Serialization
	Console::WriteLine("Serializing using the binary serializer...");
	BinarySerializer* serializerBin  = new BinarySerializer();
	serializerBin->Serialize(streamBin.Get(), obj);
	streamBin->Close();

	delete obj;
	obj = NULL;

	// Binary Deserialization
	Console::WriteLine("Deserializing using the binary serializer...");
	streamBin = fileBin->Open(FileMode_Open);
	obj = (TestObject*)serializerBin->Deserialize(streamBin.Get());
	streamBin->Close();

	Console::WriteLine();
	Console::WriteLine("> After binary deserialization:");
	obj->Print();
	Console::WriteLine();


	File* fileXml = new File(pathXml);
	FileStreamPtr streamXml = fileXml->Open(FileMode_Create);
	if (streamXml == NULL)
		return;

	// XML Serialization
	Console::WriteLine("Serializing using the XML serializer...");
	XMLSerializer* serializerXml  = new XMLSerializer();
	serializerXml->Serialize(streamXml.Get(), obj);
	streamXml->Close();

	delete obj;
	obj = NULL;

	// XML Deserialization
	Console::WriteLine("Deserializing using the XML serializer...");
	streamXml = fileXml->Open(FileMode_Open);
	obj = (TestObject*)serializerXml->Deserialize(streamXml.Get());
	streamXml->Close();

	Console::WriteLine();
	Console::WriteLine("> After XML deserialization:");
	obj->Print();
	Console::WriteLine();

	delete obj;
	obj = NULL;

	Console::Read();

	delete serializerBin;
	delete serializerXml;
	delete fileBin;
	delete fileXml;

	Application::DestroyInstance();
	Engine::DestroyInstance();
}
