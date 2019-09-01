/*=============================================================================
Utils.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Utils.h"

Image* CreateImage(const String& fileName)
{
	Resource* resource;

	String path = FileSystem::Instance()->GetFullPath(fileName);
	resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_IMAGE);
	if (resource == NULL)
		return NULL;

	return (Image*)resource->GetData();
}

Texture* CreateTexture(const String& fileName)
{
	Texture* texture;
	Image* image;

	image = CreateImage(fileName);
	if (!RenderSystem::Current()->CreateTexture(image, &texture))
		return NULL;

	return texture;
}

Font* CreateFont(const String& fileName)
{
	String path = FileSystem::Instance()->GetFullPath(fileName);
	Resource* resImage = ResourceHelper::LoadFromFile(path, SE_ID_DATA_IMAGE);
	if (resImage == NULL)
		return NULL;

	Image* fontImage = (Image*)resImage->GetData();
	if (fontImage == NULL)
		return NULL;

	Texture* texture;
	RenderSystem::Current()->CreateTexture(fontImage, &texture);

	String xmlName = Path::Combine(Path::GetDirectoryName(path),
		Path::ChangeExtension(path, _T("xml")));
	XMLReader reader(xmlName);
	SonataEngine::XMLDocument* document = reader.GetDocument();
	if (document == NULL)
		return NULL;

	Font* font = new Font();
	font->SetTexture(texture);

	const XMLNode::XMLAttributeList& fontAttributes = document->GetDocumentElement()->GetAttributes();
	for (int i=0; i<fontAttributes.Count(); i++)
	{
		if (fontAttributes[i]->GetName().CompareTo("Spacing", true) == 0)
			font->SetSpacing(fontAttributes[i]->GetValue().ToInt32());
		else if (fontAttributes[i]->GetName().CompareTo("SpaceWidth", true) == 0)
			font->SetSpaceWidth(fontAttributes[i]->GetValue().ToInt32());
		else if (fontAttributes[i]->GetName().CompareTo("Height", true) == 0)
			font->SetHeight(fontAttributes[i]->GetValue().ToInt32());
	}

	XMLNode* glyths = document->GetDocumentElement()->GetFirstChild();
	XMLNode* glyth = glyths->GetFirstChild();
	while (glyth != NULL)
	{
		FontGlyth fontGlyth;

		const XMLNode::XMLAttributeList& glythAttributes = glyth->GetAttributes();
		for (int i=0; i<glythAttributes.Count(); i++)
		{
			try
			{
				if (glythAttributes[i]->GetName().CompareTo("Character", true) == 0)
					fontGlyth.Character = glythAttributes[i]->GetValue().ToChar();
				else if (glythAttributes[i]->GetName().CompareTo("X", true) == 0)
					fontGlyth.Rectangle.X = glythAttributes[i]->GetValue().ToInt32();
				else if (glythAttributes[i]->GetName().CompareTo("Y", true) == 0)
					fontGlyth.Rectangle.Y = glythAttributes[i]->GetValue().ToInt32();
				else if (glythAttributes[i]->GetName().CompareTo("Width", true) == 0)
					fontGlyth.Rectangle.Width = glythAttributes[i]->GetValue().ToInt32();
			}
			catch (const FormatException&)
			{
			}
		}

		fontGlyth.Rectangle.Width -= fontGlyth.Rectangle.X;
		fontGlyth.Rectangle.Height = font->GetHeight();

		if (fontGlyth.Character != 0 && !fontGlyth.Rectangle.IsEmpty())
		{
			font->SetGlyth(fontGlyth.Character, fontGlyth);
		}

		glyth = glyth->GetNextSibling();
	}

	font->Build();

	delete fontImage;

	return font;
}

Sprite* CreateSprite(const String& fileName)
{
	Texture* image = CreateTexture(fileName);
	if (image == NULL)
		return NULL;

	Sprite* sprite = new Sprite();
	SpriteFrame* frame = new SpriteFrame();

	SizeInt imageSize = SizeInt(image->GetWidth(), image->GetHeight());
	frame->SetSourceRect(RectangleInt(PointInt(0, 0), imageSize));
	frame->SetTexture(image);

	sprite->AddSpriteFrame(frame);
	sprite->SetSize(Vector2(imageSize.Width, imageSize.Height));

	return sprite;
}

Model* CreateModel(const String& fileName)
{
	Model* model;
	Resource* resource;

	String path = FileSystem::Instance()->GetFullPath(fileName);
	resource = ResourceHelper::LoadFromFile(path, SE_ID_DATA_MODEL);
	if (resource == NULL)
		return NULL;

	model = (Model*)resource->GetData();
	model->SetName(Path::GetFileNameWithoutExtension(path));

	return model;
}

ShaderProgram* CreateVP(const String& fileName, const String& entryPoint)
{
	String path = FileSystem::Instance()->GetFullPath(fileName);
	File* file = new File(path);
	if (file == NULL)
		return NULL;

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read, FileShare_Read);
	if (stream == NULL)
	{
		delete file;
		return NULL;
	}

	TextStream text((Stream*)&(*stream));
	String source = text.ReadToEnd();

	ShaderProgram* vp = ShaderSystem::Current()->CreateShaderProgram(ShaderProgramType_Vertex);
	vp->SetSourceData(source);
	vp->SetEntryPoint(entryPoint);
	vp->Compile();
	vp->Create();
	return vp;
}

ShaderProgram* CreatePP(const String& fileName, const String& entryPoint)
{
	String path = FileSystem::Instance()->GetFullPath(fileName);
	File* file = new File(path);
	if (file == NULL)
		return NULL;

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read, FileShare_Read);
	if (stream == NULL)
	{
		delete file;
		return NULL;
	}

	TextStream text((Stream*)&(*stream));
	String source = text.ReadToEnd();

	ShaderProgram* pp = ShaderSystem::Current()->CreateShaderProgram(ShaderProgramType_Pixel);
	pp->SetSourceData(source);
	pp->SetEntryPoint(entryPoint);
	pp->Compile();
	pp->Create();

	return pp;
}

ShaderMaterial* CreateHWShader(const String& vertex, const String& pixel, const String& entryPoint)
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();
	ShaderPass* pass = new ShaderPass();

	pass->AddSamplerState(new SamplerState());
	TextureState* textureState = new TextureState();
	Texture* texture = CreateTexture(_T("tswater.dds"));
	textureState->SetTexture(texture);
	pass->AddTextureState(textureState);

	if (ShaderSystem::Current())
	{
		ShaderProgram* vp = CreateVP(vertex, entryPoint);
		ShaderProgram* pp = CreatePP(pixel, entryPoint);
		pass->SetVertexProgram(vp);
		pass->SetPixelProgram(pp);
	}

	technique->AddPass(pass);
	shader->AddTechnique(technique);
	
	return shader;
}
