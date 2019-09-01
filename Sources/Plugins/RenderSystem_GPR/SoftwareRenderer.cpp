/*=============================================================================
SoftwareRenderer.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "SoftwareRenderer.h"
#include "SoftwareHardwareBuffer.h"
#include "SoftwareTexture.h"
#include "SoftwareShader.h"
#include "Core/Core.h"
#include "Material/Effect.h"
#include "Graphic/Scene.h"
#include "Graphic/Camera.h"
#include "Mesh/Mesh.h"

void DefaultVertexShader(GPRVertexFuncEnv* gprEnv)
{
	OGPRVertexFuncEnv env(gprEnv);

	SoftwareRenderer* renderer = (SoftwareRenderer*) Core::Instance()->GetRenderer();
	SoftwareShader* shader = (SoftwareShader*) renderer->Shader;

	if (shader != NULL)
	{
		shader->SetVertexEnvironment(&env);
		Vector4 position = shader->vs_main(&env);
		shader->SetVertexEnvironment(NULL);

		if (!renderer->Clipping(position))
			env.WritePosition(4, (float*)&position);
	}
}

void DefaultFragmentShader(GPRFragmentFuncEnv* gprEnv)
{
	OGPRFragmentFuncEnv env(gprEnv);

	SoftwareRenderer* renderer = (SoftwareRenderer*) Core::Instance()->GetRenderer();
	SoftwareShader* shader = (SoftwareShader*) renderer->Shader;

	if (shader != NULL)
	{
		shader->SetFragmentEnvironment(&env);
		Colour32 colour = shader->fs_main(&env);

		if (renderer->Raster(env, colour))
		{
#ifndef ARGB_COLOUR_BUFFER
			env.WritePixel(DBT_ColourBuffer, &colour);
#else
			uint32 pixel = colour.ToARGB();
			env.WritePixel(DBT_ColourBuffer, &pixel);
#endif
		}

		shader->SetFragmentEnvironment(NULL);
	}
}

GPRCULLTYPE _GetCullMode(CullMode mode)
{
	GPRCULLTYPE value;
	switch (mode)
	{
	case CM_None: value = GPRCULLTYPE_NONE; break;
	case CM_Clockwise: value = GPRCULLTYPE_CLOCKWISE; break;
	case CM_CounterClockwise: value = GPRCULLTYPE_COUNTERCLOCKWISE; break;
	default: value = GPRCULLTYPE_COUNTERCLOCKWISE;
	}

	return value;
}


GPRFILTERTYPE _GetFilterType(TextureFilterType type)
{
	GPRFILTERTYPE value;
	switch (type)
	{
	case TFT_None: value = GPRFILTERTYPE_POINT; break;
	case TFT_Point: value = GPRFILTERTYPE_POINT; break;
	case TFT_Linear: value = GPRFILTERTYPE_LINEAR; break;
	case TFT_Anisotropic: value = GPRFILTERTYPE_LINEAR; break;
	default: value = GPRFILTERTYPE_POINT;
	}

	return value;
}

GPRADDRESSTYPE _GetAddressMode(TextureAddressMode mode)
{
	GPRADDRESSTYPE value;
	switch (mode)
	{
	case TAM_Wrap: value = GPRADDRESSTYPE_WRAP; break;
	case TAM_Mirror: value = GPRADDRESSTYPE_WRAP; break;
	case TAM_Clamp: value = GPRADDRESSTYPE_CLAMP; break;
	case TAM_Border: value = GPRADDRESSTYPE_EDGECLAMP; break;
	default: value = GPRADDRESSTYPE_WRAP;
	}

	return value;
}

GPRDECLTYPE _GetDeclarationType(VertexElementType type)
{
	switch (type)
	{
	case VET_Float1:
		return GPRDECLTYPE_FLOAT;
	case VET_Float2:
		return GPRDECLTYPE_FLOAT;
	case VET_Float3:
		return GPRDECLTYPE_FLOAT;
	case VET_Float4:
		return GPRDECLTYPE_FLOAT;
	case VET_Colour:
		return GPRDECLTYPE_FLOAT;
	case VET_UByte4:
        return GPRDECLTYPE_BYTE;
	case VET_Short2:
		return GPRDECLTYPE_INT;
	case VET_Short4:
		return GPRDECLTYPE_INT;
	}

	return GPRDECLTYPE_BYTE;
}

SoftwareRenderer::SoftwareRenderer() :
	Renderer()
{
	_Init = false;
	_ClearColour = Colour32(0.0f, 0.0f, 0.25f);
	_DepthValue = 1.0f;
	_StencilValue = 0xffffffff;
	_WorldTransform = Matrix4::IDENTITY;
	_VertexDeclaration = NULL;
	_VertexBuffer = NULL;

	_Renderer = NULL;
	_ColourBuffer = NULL;
	_DepthBuffer = NULL;
	_StencilBuffer = NULL;

}

SoftwareRenderer::~SoftwareRenderer()
{
}

const Matrix4& SoftwareRenderer::GetWorldTransform()
{
	return _WorldTransform;
}

void SoftwareRenderer::SetWorldTransform(const Matrix4& matWorld)
{
	_WorldTransform = matWorld;
}

void SoftwareRenderer::SetAmbientColour(const Colour32& value)
{
	States.Ambient = value;
}

void SoftwareRenderer::SetScissorState(ScissorState state)
{
	States.ScissorState = state;
}

void SoftwareRenderer::SetFogState(FogState state)
{
	States.FogState = state;
}

void SoftwareRenderer::SetPointState(PointState state)
{
	States.PointState = state;
}

void SoftwareRenderer::SetFillMode(FillMode mode)
{
	States.FillMode = mode;
}

void SoftwareRenderer::SetShadeMode(ShadeMode mode)
{
	States.ShadeMode = mode;
}

void SoftwareRenderer::SetCullMode(CullMode mode)
{
	States.CullMode = mode;

	_Renderer->SetRenderState(GPRRENDERSTATE_CULL, _GetCullMode(mode));
}

void SoftwareRenderer::SetNormalizeNormals(bool value)
{
	States.NormalizeNormals = value;
}

void SoftwareRenderer::SetDithering(bool value)
{
	States.Dithering = value;
}

void SoftwareRenderer::SetLighting(bool value)
{
	States.Lighting = value;
}

void SoftwareRenderer::SetMaterialState(MaterialState state)
{
	States.MaterialState = state;
}

void SoftwareRenderer::SetLightState(LightState state)
{
	if (state.Lighting)
	{
		LightState::LightList::Iterator it = state.Lights.GetIterator();
		while (it.Next())
		{
			Light* light = it.Current();

			if (!light->IsEnabled())
				continue;

			SoftwareLight swLight;
			swLight.AmbientColour = light->GetAmbientColour();
			swLight.DiffuseColour = light->GetDiffuseColour();
			swLight.SpecularColour = light->GetSpecularColour();
			swLight.Attenuations = Vector3(
				light->GetConstantAttenuation(),
				light->GetLinearAttenuation(),
				light->GetQuadraticAttenuation());
			swLight.Type = light->GetLightType();

			if (light->GetLightType() == LT_Directional)
			{
				DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(light);

				swLight.Direction = dirLight->GetDirection();
			}
			else if (light->GetLightType() == LT_Point)
			{
				PointLight* pointLight = dynamic_cast<PointLight*>(light);

				swLight.Position = pointLight->GetPosition();
			}

			if (light->GetLightType() == LT_Spot)
			{
				SpotLight* spotLight = dynamic_cast<SpotLight*>(light);

				swLight.Position = spotLight->GetPosition();
				swLight.Direction = spotLight->GetDirection();
				swLight.Exponent = spotLight->GetExponent();
				swLight.CutOff = spotLight->GetCutOff();
			}

			swLight.Enabled = true;
			States.Lights.Add(swLight);
		}
	}

	SetLighting(state.Lighting);
}

void SoftwareRenderer::SetTextureState(int stage, TextureState state)
{
	SoftwareTexture* texture = (SoftwareTexture*) state.GetTexture();
	if (texture == NULL)
	{
		_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_Unused + stage,
			NULL, GPRUSAGE_READONLY);
	}
	else
	{
		_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_Unused + stage,
			texture->GetBuffer(), GPRUSAGE_READONLY);

		_Renderer->SetSamplerState(GPRFUNCSTAGE_FRAGMENT, DBT_Unused + stage,
			GPRSAMPLERSTATE_ADDRESS, _GetAddressMode(state.GetAddressModeS()));
		_Renderer->SetSamplerState(GPRFUNCSTAGE_FRAGMENT, DBT_Unused + stage,
			GPRSAMPLERSTATE_FILTER, _GetFilterType(state.GetMagFilter()));
	}

	States.Textures.Add(state);
}

void SoftwareRenderer::DisableTextureState(int stage)
{
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_Unused + stage,
		NULL, GPRUSAGE_READONLY);
}

void SoftwareRenderer::SetColourWriteEnable(ColourFlag value)
{
	States.ColourWriteEnable = value;
}

void SoftwareRenderer::SetDepthState(DepthState state)
{
	States.DepthState = state;
}

void SoftwareRenderer::SetStencilState(StencilState state)
{
	States.StencilState = state;
}

void SoftwareRenderer::SetAlphaState(AlphaState state)
{
	States.AlphaState = state;
}

void SoftwareRenderer::SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend)
{
	States.AlphaState.SourceBlend = sourceBlend;
	States.AlphaState.DestinationBlend = destinationBlend;
}

bool SoftwareRenderer::Create(uint32 width, uint32 height)
{
	// create renderer
	OGPR_CreateRenderer(GPR_SDK_VERSION, &_Renderer);

	GetCaps();

	_Renderer->SetRasterResolution(width, height);
	_Renderer->SetViewport(0, 0, width, height);

	_Renderer->SetVertexFunction(DefaultVertexShader);
	_Renderer->SetFragmentFunction(DefaultFragmentShader);

	_Width = width;
	_Height = height;
	_Init = true;

	SAFE_RELEASE(_StencilBuffer);
	SAFE_RELEASE(_DepthBuffer);
	SAFE_RELEASE(_ColourBuffer);

	UINT bufferSize[] = { _Width, _Height };

	// create an RGB colour buffer
#ifndef ARGB_COLOUR_BUFFER
	_Renderer->CreateBuffer(2, bufferSize, GPRDECLTYPE_FLOAT, 3, &_ColourBuffer);
#else
	_Renderer->CreateBuffer(2, bufferSize, GPRDECLTYPE_INT, 1, &_ColourBuffer);
#endif

	// create a depth buffer (1*float)
	_Renderer->CreateBuffer(2, bufferSize, GPRDECLTYPE_FLOAT, 1, &_DepthBuffer);

	// create a stencil buffer (1*byte)
	_Renderer->CreateBuffer(2, bufferSize, GPRDECLTYPE_BYTE, 1, &_StencilBuffer);

	_Renderer->SetViewport(0, 0, _Width, _Height);

	return true;
}

void SoftwareRenderer::GetCaps()
{
	GPRCAPS swCaps;
	_Renderer->GetCaps(&swCaps);
	
	_Caps.MaxStreams = swCaps.maxStreams;
	_Caps.MaxInterpolants = swCaps.maxStreams;
	_Caps.MaxConstants = swCaps.maxStreams;
	_Caps.MaxBuffers = swCaps.maxStreams;
}

bool SoftwareRenderer::Destroy()
{
	// release interfaces
	SAFE_RELEASE(_StencilBuffer);
	SAFE_RELEASE(_DepthBuffer);
	SAFE_RELEASE(_ColourBuffer);
	SAFE_RELEASE(_Renderer);

	_Init = false;

	return true;
}

void SoftwareRenderer::Clear()
{
#ifndef ARGB_COLOUR_BUFFER
	_ColourBuffer->Clear(&_ClearColour);
#else
	uint32 colour = _ClearColour.ToARGB();
	_ColourBuffer->Clear(&colour);
#endif

	_DepthBuffer->Clear(&_DepthValue);
	_StencilBuffer->Clear(&_StencilValue);
}

void SoftwareRenderer::BeginScene()
{
	// setup buffers
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_ColourBuffer, _ColourBuffer, GPRUSAGE_RENDERTARGET);
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_DepthBuffer, _DepthBuffer, GPRUSAGE_RENDERTARGET);
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_StencilBuffer, _StencilBuffer, GPRUSAGE_RENDERTARGET);

	// reset device
	//for (uint i = 0; i < _Caps.MaxStreams; i++)
	//	_Renderer->SetStreamSource(i, NULL, 0, 0);
}

void SoftwareRenderer::EndScene()
{
	// release buffer from renderer
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_ColourBuffer, 0, GPRUSAGE_READONLY);
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_DepthBuffer, 0, GPRUSAGE_READONLY);
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_StencilBuffer, 0, GPRUSAGE_READONLY);
}

void SoftwareRenderer::DumpColourBuffer(const String& fileName)
{
	_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_ColourBuffer, 0, GPRUSAGE_READONLY);
	//_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_DepthBuffer, 0, GPRUSAGE_READONLY);
	//_Renderer->SetBuffer(GPRFUNCSTAGE_FRAGMENT, DBT_StencilBuffer, 0, GPRUSAGE_READONLY);

	GPRX_SaveTGA(fileName, _ColourBuffer->GetBuffer());
	//GPRX_SaveTGA("data\\depth.tga", _DepthBuffer->GetBuffer());
	//GPRX_SaveTGA("data\\stencil.tga", _StencilBuffer->GetBuffer());
}

bool SoftwareRenderer::CreateVertexBuffer(uint32 length, uint32 components, HardwareBuffer** vertexBuffer)
{
	OGPRBuffer* buffer;

	_Renderer->CreateBuffer(1, &length, GPRDECLTYPE_BYTE, components, &buffer);
	*vertexBuffer = new SoftwareHardwareBuffer(buffer);

	return true;
}

bool SoftwareRenderer::CreateIndexBuffer(uint32 indexSize, uint32 indexCount, HardwareBuffer** indexBuffer)
{
	OGPRBuffer* buffer;

	_Renderer->CreateBuffer(1, &indexCount, GPRDECLTYPE_INT, indexSize, &buffer);
	*indexBuffer = new SoftwareHardwareBuffer(buffer);

	return true;
}

bool SoftwareRenderer::CreateTexture(Image* image, Texture** texture)
{
	*texture = new SoftwareTexture(_Renderer);

	return (*texture)->Create(image);
}

void SoftwareRenderer::Render(Mesh* mesh)
{
	// reset states
	States.Lights.Clear();
	States.Textures.Clear();

	Renderer::Render(mesh);
}

void SoftwareRenderer::RenderGeometry(Mesh* mesh)
{
	_VertexDeclaration = mesh->GetVertexDeclaration();
	_VertexBuffer = ((SoftwareHardwareBuffer*)mesh->GetVertexData())->GetBuffer();

	uint32 count = mesh->GetIndexed() ? mesh->GetIndexCount() : mesh->GetVertexCount();
	GPRPRIMITIVETYPE primitiveType;
	UINT primitiveCount = 0;
	switch (mesh->GetPrimitiveType())
	{
	case PT_PointList:
		primitiveType = GPRPRIMITIVETYPE_POINTLIST;
		primitiveCount = count;
		break;
	case PT_LineList:
		primitiveType = GPRPRIMITIVETYPE_LINELIST;
		primitiveCount = count / 2;
		break;
	case PT_TriangleList:
		primitiveType = GPRPRIMITIVETYPE_TRIANGLELIST;
		primitiveCount = count / 3;
		break;
	default:
		primitiveType = GPRPRIMITIVETYPE_POINTLIST;
		primitiveCount = count;
	}

	if (mesh->GetIndexed())
	{
		OGPRBuffer* indexBuffer = ((SoftwareHardwareBuffer*) mesh->GetIndexData())->GetBuffer();
		_Renderer->DrawIndexedPrimitive(primitiveType,
			indexBuffer->GetBuffer(), 0, primitiveCount);	
	}
	else
	{
		_Renderer->DrawPrimitive(primitiveType, 0, primitiveCount);	
	}
}

void SoftwareRenderer::DrawText(const String& text, uint32 x, uint32 y, const Colour32& colour)
{
	// SDL_TTF
}
