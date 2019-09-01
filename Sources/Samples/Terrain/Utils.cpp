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
	if (!RenderSystem::Current()->CreateTexture(image, TextureUsage_Static, &texture))
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
	RenderSystem::Current()->CreateTexture(fontImage, TextureUsage_Static, &texture);

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
					fontGlyth.Rectangle.X = glythAttributes[i]->GetValue().ToInt32();
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

	SamplerState* samplerState = new SamplerState();
	samplerState->SetTexture(CreateTexture(_T("tswater.dds")));
	pass->AddSamplerState(samplerState);

	TextureState* textureState = new TextureState();
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

ShaderMaterial* CreateTerrainShader_DiffuseDetail()
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();
	ShaderPass* pass = new ShaderPass();

	pass->LightState.Lighting = true;

	SamplerState* samplerDiffuse = new SamplerState();
	samplerDiffuse->SetTexture(CreateTexture(_T("Terrain\\grass.png")));
	pass->AddSamplerState(samplerDiffuse);
	TextureState* textureDiffuse = new TextureState();
	textureDiffuse->TextureCoordinateIndex = 0;
	pass->AddTextureState(textureDiffuse);

	SamplerState* samplerDetail = new SamplerState();
	samplerDetail->SetTexture(CreateTexture(_T("Terrain\\detail1.png")));
	pass->AddSamplerState(samplerDetail);
	TextureState* textureDetail = new TextureState();
	textureDetail->TextureCoordinateIndex = 1;
	textureDetail->ColorOperation = TextureOperation_AddSigned;
	textureDetail->ColorArgument1 = TextureArgument_textureColor;
	textureDetail->ColorArgument2 = TextureArgument_current;
	pass->AddTextureState(textureDetail);

	technique->AddPass(pass);
	shader->AddTechnique(technique);
	
	return shader;
}

ShaderMaterial* CreateTerrainShader_DiffuseGenerate(Terrain* terrain)
{
	Array<TerrainTextureLayer> layers;
	TerrainTextureLayer layer0;
	layer0.SetImage(CreateImage(_T("Terrain\\grass.jpg")));
	layer0.SetHeight(RangeReal32(70.0f/255.0f, 255.0f/255.0f));
	layers.Add(layer0);

	TerrainTextureLayer layer1;
	layer1.SetImage(CreateImage(_T("Terrain\\sand.jpg")));
	layer1.SetHeight(RangeReal32(0.0f/255.0f, 80.0f/255.0f));
	layers.Add(layer1);

	TerrainTextureGenerator gen;
	gen.SetWidth(terrain->GetHeightField()->GetWidth());
	gen.SetHeight(terrain->GetHeightField()->GetHeight());
	gen.SetTerrain(terrain);
	gen.SetHeightField(terrain->GetHeightField());
	gen.SetLayers(layers);

	if (!gen.Create())
		return NULL;

	Image* image = gen.GetImage();
	Texture* texture;
	if (!RenderSystem::Current()->CreateTexture(image, TextureUsage_Static, &texture))
		return NULL;

	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();
	ShaderPass* pass = new ShaderPass();

	pass->LightState.Lighting = true;

	SamplerState* samplerDiffuse = new SamplerState();
	samplerDiffuse->SetTexture(texture);
	pass->AddSamplerState(samplerDiffuse);
	TextureState* textureDiffuse = new TextureState();
	textureDiffuse->TextureCoordinateIndex = 0;
	pass->AddTextureState(textureDiffuse);

	/*pass->AddSamplerState(new SamplerState());
	TextureState* textureDetail = new TextureState();
	textureDetail->SetTexture(CreateTexture(_T("Terrain\\detail1.png")));
	textureDetail->TextureCoordinateIndex = 1;
	textureDetail->ColorOperation = TextureOperation_AddSigned;
	textureDetail->ColorArgument1 = TextureArgument_textureColor;
	textureDetail->ColorArgument2 = TextureArgument_current;
	pass->AddTextureState(textureDetail);*/

	technique->AddPass(pass);
	shader->AddTechnique(technique);
	
	return shader;
}

ShaderMaterial* CreateTerrainShader_Splatting(Terrain* terrain)
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();
	ShaderPass* pass = new ShaderPass();
	
	pass->LightState.Lighting = true;
	//pass->AlphaState.BlendEnable[0] = true;
	//pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
	//pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
	//pass->SetVertexProgram(CreateVP("Terrain\\splattingVS.cg", "main"));
	pass->SetPixelProgram(CreatePP("Terrain\\splattingPS.cg", "main"));

	SamplerState* samplerAlpha = new SamplerState();
	samplerAlpha->SetTexture(CreateTexture("Terrain\\alphamap.tga"));
	pass->AddSamplerState(samplerAlpha);
	pass->AddTextureState(new TextureState());

	SamplerState* samplerDiffuse1 = new SamplerState();
	samplerDiffuse1->SetTexture(CreateTexture("Terrain\\texture1.png"));
	pass->AddSamplerState(samplerDiffuse1);
	pass->AddTextureState(new TextureState());

	SamplerState* samplerDiffuse2 = new SamplerState();
	samplerDiffuse2->SetTexture(CreateTexture("Terrain\\texture2.png"));
	pass->AddSamplerState(samplerDiffuse2);
	pass->AddTextureState(new TextureState());

	SamplerState* samplerDiffuse3 = new SamplerState();
	samplerDiffuse3->SetTexture(CreateTexture("Terrain\\texture3.png"));
	pass->AddSamplerState(samplerDiffuse3);
	pass->AddTextureState(new TextureState());

	SamplerState* samplerDiffuse4 = new SamplerState();
	samplerDiffuse4->SetTexture(CreateTexture("Terrain\\texture4.png"));
	pass->AddSamplerState(samplerDiffuse4);
	pass->AddTextureState(new TextureState());

	technique->AddPass(pass);
	shader->AddTechnique(technique);
	
	return shader;
}
