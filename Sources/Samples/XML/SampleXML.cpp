/*=============================================================================
SampleXML.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SampleXML.h"

XMLElement* CreateReal32Element(XMLDocument* document, const String& name, real32 value)
{
	XMLElement* element = document->CreateElement(name);

	element->SetAttribute("value", String::ToString(value));

	return element;
}

XMLElement* CreateStringElement(XMLDocument* document, const String& name, const String& value)
{
	XMLElement* element = document->CreateElement(name);

	element->SetAttribute("value", value);

	return element;
}

XMLElement* CreateColor32Element(XMLDocument* document, const String& name, const Color32& value)
{
	XMLElement* element = document->CreateElement(name);

	element->SetAttribute("r", String::ToString(value.r));
	element->SetAttribute("g", String::ToString(value.g));
	element->SetAttribute("b", String::ToString(value.b));
	element->SetAttribute("a", String::ToString(value.a));

	return element;
}

XMLDocument* CreateXML()
{
	XMLDocument* document = new XMLDocument();

		XMLComment* DocumentComment = document->CreateComment("This is a document.");
		document->AppendChild(DocumentComment);

		XMLElement* Shader = document->CreateElement("Shader");
			XMLComment* ShaderComment = document->CreateComment("This is a shader.");
			XMLElement* ShaderName = CreateStringElement(document, "Name", "myShader");
		Shader->AppendChild(ShaderComment);
		Shader->AppendChild(ShaderName);
			XMLElement* Material = document->CreateElement("Material");
				XMLComment* MaterialComment = document->CreateComment("This is a material.");
				XMLElement* AmbientColor = CreateColor32Element(document, "AmbientColor",
					Color32(1.0f, 1.0f, 1.0f, 0.0f));
				XMLElement* DiffuseColor = CreateColor32Element(document, "DiffuseColor",
					Color32(1.0f, 0.0f, 0.0f, 0.0f));
				XMLElement* SpecularColor = CreateColor32Element(document, "SpecularColor",
					Color32(0.0f, 1.0f, 0.0f, 0.0f));
				XMLElement* EmissiveColor = CreateColor32Element(document, "EmissiveColor",
					Color32(0.0f, 0.0f, 0.0f, 0.0f));
				XMLElement* Shininess = CreateReal32Element(document, "Shininess", 1.0f);
			Material->AppendChild(MaterialComment);
			Material->AppendChild(AmbientColor);
			Material->AppendChild(DiffuseColor);
			Material->AppendChild(SpecularColor);
			Material->AppendChild(EmissiveColor);
			Material->AppendChild(Shininess);
		Shader->AppendChild(Material);
	document->AppendChild(Shader);

	return document;
}

int g_Inc = 0;

void PrintInc()
{
	for (int i=0; i<g_Inc; ++i)
		Console::Write("\t");
}

void PrintNode(XMLNode* node)
{
	int i;

	switch (node->GetNodeType())
	{
	case XMLNodeType_Element:
	{
		PrintInc();
		Console::WriteLine("Element: " + node->GetName());

		XMLElement* element = (XMLElement*)node;

		XMLNode::XMLAttributeList attributes = element->GetAttributes();
		for (i=0; i<attributes.Count(); i++)
		{
			PrintInc();
			Console::WriteLine("\tAttribute: " + attributes[i]->GetName() +
				" = " + attributes[i]->GetValue());
		}
	}
	break;
	case XMLNodeType_Text:
	{
		PrintInc();
		Console::WriteLine("Text");

		PrintInc();
		Console::WriteLine("\t" + node->GetValue());
	}
	break;
	case XMLNodeType_Comment:
	{
		PrintInc();
		Console::WriteLine("Comment");

		PrintInc();
		Console::WriteLine("\t" + node->GetValue());
	}
	break;
	case XMLNodeType_Document:
	{
		PrintInc();
		Console::WriteLine("Document");
	}
	break;
	}

	XMLNode::XMLNodeList nodes = node->GetChildNodes();
	for (i=0; i<nodes.Count(); i++)
	{
		g_Inc++;
		PrintNode(nodes[i]);
		g_Inc--;
	}
}

void EntryPoint()
{
	Engine::Instance();
	FileSystem::Instance()->AddRootPath(_T("Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\Data"));
	FileSystem::Instance()->AddRootPath(_T("..\\..\\..\\Data"));

	String path = FileSystem::Instance()->GetFullPath("material.xml");

	// Creates an XML document
	XMLDocument* document = CreateXML();

	// Writes the XML document
	XMLWriter writer(document, path);

	// Destroys it
	delete document;
	document = NULL;

	// Reads the XML document
	XMLReader reader(path);
	document = reader.GetDocument();

	// Prints the XML document content
	PrintNode(document);

	// Destroys it
	delete document;
	document = NULL;

	system("pause");

	// Print the file content
	File* file = new File(path);
	FileStreamPtr stream = file->Open(FileMode_Open);
	TextStream textReader(stream.Get());
	String text = textReader.ReadToEnd();
	Console::Write(text);

	delete file;

	system("pause");
}
