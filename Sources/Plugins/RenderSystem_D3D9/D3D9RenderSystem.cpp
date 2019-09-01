/*=============================================================================
D3D9RenderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9RenderSystem.h"
#include "D3D9Helper.h"
#include "D3D9RenderContext.h"
#include "D3D9VertexBuffer.h"
#include "D3D9IndexBuffer.h"
#include "D3D9VertexLayout.h"
#include "D3D9Texture.h"

namespace SE_D3D9
{

#define D3D9_SETTEXTURESTAGE_VALUE(type, value) \
	_D3DDevice->SetTextureStageState(stage, type, value);

#define D3D9_SETTEXTURESTAGE_OP(type, value) \
	_D3DDevice->SetTextureStageState(stage, type, \
		D3D9Helper::GetTextureOperation(value));

#define D3D9_SETTEXTURESTAGE_ARG(type, value) \
	_D3DDevice->SetTextureStageState(stage, type, \
		D3D9Helper::GetTextureArgument(value));

D3D9RenderSystem::D3D9RenderSystem() :
	RenderSystem(),
	_clearColor(Color32::Black),
	_projectionTransform(Matrix4::Identity),
	_viewTransform(Matrix4::Identity),
	_worldTransform(Matrix4::Identity)
{
	_isReady = false;
	_depthValue = 1.0f;
	_stencilValue = 0;

	_D3DDevice = NULL;
	_D3DBackBuffer = NULL;
	_D3DXLine = NULL;

	// Create the D3D object.
	_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (_D3D == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem"),
			_T("Failed to create the Direct3D9 object."));
		return;
	}
}

D3D9RenderSystem::~D3D9RenderSystem()
{
	Destroy();

	SE_RELEASE(_D3D);
}

bool D3D9RenderSystem::IsPrimitiveTypeSupported(PrimitiveType primitiveType) const
{
	switch (primitiveType)
	{
	case PrimitiveType_PointList:
	case PrimitiveType_LineList:
	case PrimitiveType_LineStrip:
	case PrimitiveType_TriangleList:
	case PrimitiveType_TriangleStrip:
	case PrimitiveType_TriangleFan:
		return true;

	default:
		return false;
	}
}

Viewport D3D9RenderSystem::GetViewport()
{
	return _viewport;
}

void D3D9RenderSystem::SetViewport(const Viewport& value)
{
	_viewport = value;

	D3DVIEWPORT9 d3dViewport;
	d3dViewport.X = _viewport.GetLeft();
	d3dViewport.Y = _viewport.GetTop();
	d3dViewport.Width = _viewport.GetWidth();
	d3dViewport.Height = _viewport.GetHeight();
	d3dViewport.MinZ = 0.0f;
	d3dViewport.MaxZ = 1.0f;
	_D3DDevice->SetViewport(&d3dViewport);
}

void D3D9RenderSystem::SetFillMode(FillMode mode)
{
	D3DFILLMODE value;
	switch (mode)
	{
	case FillMode_Point: value = D3DFILL_POINT; break;
	case FillMode_WireFrame: value = D3DFILL_WIREFRAME; break;
	case FillMode_Solid: value = D3DFILL_SOLID; break;
	default: value = D3DFILL_SOLID;
	}

	_D3DDevice->SetRenderState(D3DRS_FILLMODE, value);
}

void D3D9RenderSystem::SetShadeMode(ShadeMode mode)
{
	D3DSHADEMODE value;
	switch (mode)
	{
	case ShadeMode_Flat: value = D3DSHADE_FLAT; break;
	case ShadeMode_Smooth: value = D3DSHADE_GOURAUD; break;
	default: value = D3DSHADE_FLAT;
	}

	_D3DDevice->SetRenderState(D3DRS_SHADEMODE, value);
}

void D3D9RenderSystem::SetCullMode(CullMode mode)
{
	D3DCULL value;
	switch (mode)
	{
	case CullMode_None: value = D3DCULL_NONE; break;
	case CullMode_Front: value = D3DCULL_CCW; break;
	case CullMode_Back: value = D3DCULL_CW; break;
	default: value = D3DCULL_CW;
	}

	_D3DDevice->SetRenderState(D3DRS_CULLMODE, value);
}

void D3D9RenderSystem::SetColorWriteEnable(ColorFlag value)
{
	DWORD d3dValue = 0;
	if ((value & ColorFlag_Red) != 0) d3dValue |= D3DCOLORWRITEENABLE_RED;
	if ((value & ColorFlag_Green) != 0) d3dValue |= D3DCOLORWRITEENABLE_GREEN;
	if ((value & ColorFlag_Blue) != 0) d3dValue |= D3DCOLORWRITEENABLE_BLUE;
    if ((value & ColorFlag_Alpha) != 0) d3dValue |= D3DCOLORWRITEENABLE_ALPHA;
	_D3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, d3dValue);
}

void D3D9RenderSystem::SetDepthState(const DepthState& state)
{
	_D3DDevice->SetRenderState(D3DRS_ZENABLE, state.Enable);
	_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, state.WriteEnable);
	_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3D9Helper::GetCmpFunction(state.Function));
	_D3DDevice->SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)&state.DepthBias));
}

void D3D9RenderSystem::SetStencilState(const StencilState& state)
{
	_D3DDevice->SetRenderState(D3DRS_STENCILENABLE, state.Enable);
	_D3DDevice->SetRenderState(D3DRS_STENCILFAIL, state.FrontFace.Fail);
	_D3DDevice->SetRenderState(D3DRS_STENCILZFAIL, state.FrontFace.DepthBufferFail);
	_D3DDevice->SetRenderState(D3DRS_STENCILPASS, state.FrontFace.Pass);
	_D3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3D9Helper::GetCmpFunction(state.FrontFace.Function));
	_D3DDevice->SetRenderState(D3DRS_STENCILREF, state.Reference);
	_D3DDevice->SetRenderState(D3DRS_STENCILMASK, state.ReadMask);
	_D3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, state.WriteMask);
}

void D3D9RenderSystem::SetScissorState(const ScissorState& state)
{
	_D3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, state.Enable);

	if (state.Enable)
	{
		RECT rect = { state.Rectangle.X, state.Rectangle.Y,
			state.Rectangle.X + state.Rectangle.Width,
			state.Rectangle.Y + state.Rectangle.Height };
		_D3DDevice->SetScissorRect(&rect);
	}
}

void D3D9RenderSystem::SetDithering(bool value)
{
	_D3DDevice->SetRenderState(D3DRS_DITHERENABLE, value);
}

void D3D9RenderSystem::SetPointState(const PointState& state)
{
	_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, state.Enable);

	if (state.Enable)
	{
		_D3DDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&state.Size));
		_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&state.MinSize));
		_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&state.MaxSize));
		_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, state.ScaleEnable);

		if (state.ScaleEnable)
		{
			_D3DDevice->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&state.ConstantScale));
			_D3DDevice->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&state.LinearScale));
			_D3DDevice->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&state.QuadraticScale));
		}
	}
}

void D3D9RenderSystem::SetAlphaState(const AlphaState& state)
{
	_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, state.TestEnable);
	_D3DDevice->SetRenderState(D3DRS_ALPHAREF, state.Reference);
	_D3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3D9Helper::GetCmpFunction(state.Function));

	_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, state.BlendEnable[0]);
	SetBlendModes(state.SourceBlend, state.DestinationBlend);
	_D3DDevice->SetRenderState(D3DRS_BLENDOP, D3D9Helper::GetBlendOperation(state.BlendOp));
	_D3DDevice->SetRenderState(D3DRS_BLENDFACTOR, state.BlendFactor);
}

void D3D9RenderSystem::SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend)
{
	_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3D9Helper::GetBlendMode(sourceBlend));
	_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3D9Helper::GetBlendMode(destinationBlend));
}

void D3D9RenderSystem::SetSamplerState(int stage, const SamplerState& state)
{
	D3D9Texture* texture = (D3D9Texture*)state.GetTexture();
	if (texture == NULL)
	{
		_D3DDevice->SetTexture(stage, NULL);
	}
	else
	{
		_D3DDevice->SetTexture(stage, texture->GetD3DBaseTexture());

		_D3DDevice->SetSamplerState(stage, D3DSAMP_ADDRESSU,
			D3D9Helper::GetAddressMode(state.AddressModeU));
		_D3DDevice->SetSamplerState(stage, D3DSAMP_ADDRESSV,
			D3D9Helper::GetAddressMode(state.AddressModeV));

		_D3DDevice->SetSamplerState(stage, D3DSAMP_MAGFILTER,
			D3D9Helper::GetFilterType(state.MagFilter));
		_D3DDevice->SetSamplerState(stage, D3DSAMP_MINFILTER,
			D3D9Helper::GetFilterType(state.MinFilter));
		_D3DDevice->SetSamplerState(stage, D3DSAMP_MIPFILTER,
			D3D9Helper::GetFilterType(state.MipFilter));
	}
}

void D3D9RenderSystem::DisableSamplerState(int stage)
{
	_D3DDevice->SetTexture(stage, NULL);
}


const Matrix4& D3D9RenderSystem::GetProjectionTransform()
{
	return _projectionTransform;
}

void D3D9RenderSystem::SetProjectionTransform(const Matrix4& value)
{
	_projectionTransform = value;

	D3DXMATRIX d3dxProjection = D3D9Helper::MakeD3DXMatrix(value);
	_D3DDevice->SetTransform(D3DTS_PROJECTION, &d3dxProjection);
}

const Matrix4& D3D9RenderSystem::GetViewTransform()
{
	return _viewTransform;
}

void D3D9RenderSystem::SetViewTransform(const Matrix4& value)
{
	_viewTransform = value;

	D3DXMATRIX d3dxView = D3D9Helper::MakeD3DXMatrix(value);
	_D3DDevice->SetTransform(D3DTS_VIEW, &d3dxView);
}

const Matrix4& D3D9RenderSystem::GetWorldTransform()
{
	return _worldTransform;
}

void D3D9RenderSystem::SetWorldTransform(const Matrix4& value)
{
	_worldTransform = value;

	D3DXMATRIX d3dxWorld = D3D9Helper::MakeD3DXMatrix(value);
	_D3DDevice->SetTransform(D3DTS_WORLD, &d3dxWorld);
}

void D3D9RenderSystem::SetAmbientColor(const Color32& value)
{
	_D3DDevice->SetRenderState(D3DRS_AMBIENT, D3D9Helper::MakeD3DColor(value));
}

void D3D9RenderSystem::SetLightState(const LightState& state)
{
	int count = 0;

	_D3DDevice->SetRenderState(D3DRS_LIGHTING, state.Lighting);
	_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, state.NormalizeNormals);

	if (state.Lighting)
	{
		count = Math::Min(state.Lights.Count(), 8);
		for (int i = 0; i < count; i++)
		{
			LightSource light = state.Lights[i];

			if (!light.IsEnabled)
			{
				_D3DDevice->LightEnable(i, FALSE);
				continue;
			}

			D3DLIGHT9 d3dLight;
			::ZeroMemory(&d3dLight, sizeof(D3DLIGHT9));

			d3dLight.Ambient = D3D9Helper::MakeD3DXColor(light.AmbientColor);
			d3dLight.Diffuse = D3D9Helper::MakeD3DXColor(light.DiffuseColor);
			d3dLight.Specular = D3D9Helper::MakeD3DXColor(light.SpecularColor);

			d3dLight.Attenuation0 = light.ConstantAttenuation;
			d3dLight.Attenuation1 = light.LinearAttenuation;
			d3dLight.Attenuation2 = light.QuadraticAttenuation;

			if (light.LightType == LightType_Directional)
			{
				d3dLight.Type = D3DLIGHT_DIRECTIONAL;
				d3dLight.Direction = D3DXVECTOR3(light.Direction);
			}
			else if (light.LightType == LightType_Point)
			{
				d3dLight.Type = D3DLIGHT_POINT;
				d3dLight.Position = D3DXVECTOR3(light.Position);
				d3dLight.Range = light.Range;
			}
			else if (light.LightType == LightType_Spot)
			{
				d3dLight.Type = D3DLIGHT_SPOT;
				d3dLight.Position = D3DXVECTOR3(light.Position);
				d3dLight.Direction = D3DXVECTOR3(light.Direction);
				d3dLight.Range = light.Range;

				d3dLight.Falloff = light.FalloffExponent;
				d3dLight.Theta = light.InnerAngle;
				d3dLight.Phi = light.OuterAngle;
			}

			_D3DDevice->SetLight(i, &d3dLight);
			_D3DDevice->LightEnable(i, TRUE);
		}
	}

	if (count < 8)
	{
		for (int i = count; i < 8; i++)
		{
			_D3DDevice->LightEnable(i, FALSE);
		}
	}
}

void D3D9RenderSystem::SetMaterialState(const MaterialState& state)
{
	D3DMATERIAL9 d3dMaterial;
	d3dMaterial.Ambient = D3D9Helper::MakeD3DXColor(state.AmbientColor);
	d3dMaterial.Diffuse = D3D9Helper::MakeD3DXColor(state.DiffuseColor);
	d3dMaterial.Specular = D3D9Helper::MakeD3DXColor(state.SpecularColor);
	d3dMaterial.Emissive = D3D9Helper::MakeD3DXColor(state.EmissiveColor);
	d3dMaterial.Power = state.Shininess;
	_D3DDevice->SetMaterial(&d3dMaterial);

	_D3DDevice->SetRenderState(D3DRS_COLORVERTEX, state.VertexColor);
	_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, state.Shininess > 0.0f);
}

void D3D9RenderSystem::SetTextureState(int stage, const TextureState& state)
{
	D3D9_SETTEXTURESTAGE_OP(D3DTSS_COLOROP, state.ColorOperation);
	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_COLORARG1, state.ColorArgument1);
	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_COLORARG2, state.ColorArgument2);
	D3D9_SETTEXTURESTAGE_OP(D3DTSS_ALPHAOP, state.AlphaOperation);
	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_ALPHAARG1, state.AlphaArgument1);
	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_ALPHAARG2, state.AlphaArgument2);
	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT00, *((DWORD*)&state.BumpEnvironmentMaterial00));
	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT01, *((DWORD*)&state.BumpEnvironmentMaterial01));
	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT10, *((DWORD*)&state.BumpEnvironmentMaterial10));
	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT11, *((DWORD*)&state.BumpEnvironmentMaterial11));

	_D3DDevice->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX,
		D3D9Helper::GetTextureCoordinateMode(state.TextureCoordinateGeneration) |
		state.TextureCoordinateIndex);

	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVLSCALE, *((DWORD*)&state.BumpEnvironmentLuminanceScale));
	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVLOFFSET, *((DWORD*)&state.BumpEnvironmentLuminanceOffset));

	_D3DDevice->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS,
		D3D9Helper::GetTextureTransform(state.TextureTransform));

	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_COLORARG0, state.ColorArgument0);
	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_ALPHAARG0, state.AlphaArgument0);
	D3D9_SETTEXTURESTAGE_ARG(D3DTSS_RESULTARG, state.ResultArgument);
	D3D9_SETTEXTURESTAGE_VALUE(D3DTSS_CONSTANT, D3D9Helper::MakeD3DColor(state.ConstantColor));
}

void D3D9RenderSystem::SetFogState(const FogState& state)
{
	_D3DDevice->SetRenderState(D3DRS_FOGENABLE, state.Enable);

	if (state.Enable)
	{
		_D3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3D9Helper::MakeD3DColor(state.Color));
		_D3DDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)&state.Density));
		_D3DDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)&state.Start));
		_D3DDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)&state.End));
		_D3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, state.RangeEnable);
		_D3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3D9Helper::GetFogMode(state.VertexMode));
		_D3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3D9Helper::GetFogMode(state.PixelMode));
	}
}

void D3D9RenderSystem::Destroy()
{
	if (!_isReady)
		return;

	SE_RELEASE(_D3DXLine);

	SE_DELETE_ARRAY(_D3DBackBuffer);

	SE_RELEASE(_D3DDevice);
}

void D3D9RenderSystem::SetRenderTarget(int index, RenderTarget* value)
{
	if (value == NULL || !value->IsTexture())
	{
		return;
	}
	if (index < 0 || index > (int)_D3DCaps.NumSimultaneousRTs)
	{
		return;
	}

	D3D9Texture* texture = (D3D9Texture*)((RenderTexture*)value)->GetTexture();
	if (texture == NULL)
	{
		return;
	}

	// First time the RT at that index is set, gets the backbuffer
	if (_D3DBackBuffer == NULL)
	{
		_D3DDevice->GetRenderTarget(index, &_D3DBackBuffer);
	}

	IDirect3DSurface9* d3dSurface;
	if (texture->GetTextureType() == TextureType_Texture2D)
		d3dSurface = texture->GetD3DSurface();
	if (texture->GetTextureType() == TextureType_Texture2D)
		d3dSurface = texture->GetD3DCubeFaceSurface((CubeTextureFace)index);

	_D3DDevice->SetRenderTarget(index, d3dSurface);
}

void D3D9RenderSystem::RestoreRenderTarget(int index)
{
	if (index < 0 || index > (int)_D3DCaps.NumSimultaneousRTs)
	{
		return;
	}

	_D3DDevice->SetRenderTarget(index, _D3DBackBuffer);
}

bool D3D9RenderSystem::Resize(uint32 width, uint32 height)
{
	SetViewport(Viewport(0, 0, width, height));
	return true;
}

void D3D9RenderSystem::Clear()
{
	if (_D3DDevice == NULL)
	{
		return;
	}

	_D3DDevice->Clear(
		0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_COLORVALUE(_clearColor.R, _clearColor.G, _clearColor.B, 1.0f),
		_depthValue, _stencilValue);
}

void D3D9RenderSystem::ClearColor()
{
	if (_D3DDevice == NULL)
	{
		return;
	}

	_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_COLORVALUE(_clearColor.R, _clearColor.G, _clearColor.B, 1.0f),
		1.0f, 0xffffffff);
}

void D3D9RenderSystem::ClearDepth()
{
	if (_D3DDevice == NULL)
	{
		return;
	}

	_D3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, _depthValue, 0xffffffff);
}

void D3D9RenderSystem::ClearStencil()
{
	if (_D3DDevice == NULL)
	{
		return;
	}

	_D3DDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, _stencilValue);
}

void D3D9RenderSystem::BeginScene()
{
	_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	_D3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	_D3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	// Begin the scene
	if (FAILED(_D3DDevice->BeginScene()))
	{
		return;
	}
}

void D3D9RenderSystem::EndScene()
{
	// End the scene
	_D3DDevice->EndScene();
}

void D3D9RenderSystem::SwapBuffers(WindowHandle handle)
{
	Array<RenderTarget*>::Iterator it = _renderTargets.GetIterator();
	while (it.Next())
	{
		if (!it.Current()->IsTexture())
		{
			((D3D9RenderContext*)it.Current())->SwapBuffers(handle);
		}
	}
}

void D3D9RenderSystem::GetColorBuffer(Image** image)
{
	// get the adapter
	D3DDEVICE_CREATION_PARAMETERS dcp;
	dcp.AdapterOrdinal = D3DADAPTER_DEFAULT;
	_D3DDevice->GetCreationParameters(&dcp);

	// get the display size
	D3DDISPLAYMODE dm;
	dm.Width = dm.Height = 0;
	_D3D->GetAdapterDisplayMode(dcp.AdapterOrdinal, &dm);

	// create a surface
	IDirect3DSurface9* d3dSurface = NULL;
	_D3DDevice->CreateOffscreenPlainSurface(
		dm.Width, dm.Height, D3DFMT_A8R8G8B8, 
		D3DPOOL_SYSTEMMEM, &d3dSurface, NULL);

	// get the front buffer
	_D3DDevice->GetFrontBufferData(0, d3dSurface);

	LPBYTE data = NULL;
	data = new BYTE[dm.Width * dm.Height * 3 + 1];
	if (data == NULL)
	{
		SEthrow(OutOfMemoryException());
	}

	// lock the surface for reading
	D3DLOCKED_RECT d3dRect;
	if (FAILED(d3dSurface->LockRect(&d3dRect, NULL, D3DLOCK_READONLY)))
	{
		SEthrow(Exception("IDirect3DSurface9::LockRect failed"));
	}

	//*image = new Image();
	//d3dRect.pBits

	// unlock and release the surface
	d3dSurface->UnlockRect();
	SE_RELEASE(d3dSurface);
}

bool D3D9RenderSystem::CreateRenderContext(Window* window, const RenderContextDescription& desc)
{
	D3D9RenderContext* renderContext = new D3D9RenderContext();
	if (_D3DDevice != NULL)
	{
		renderContext->SetDevice(_D3DDevice);
	}
	if (!renderContext->Create(window, desc))
	{
		delete renderContext;

		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.CreateRenderContext"),
			_T("Failed to create the render context."));
		return false;
	}

	if (_D3DDevice == NULL)
	{
		_D3DDevice = renderContext->GetDevice();

		_D3DDevice->GetDeviceCaps(&_D3DCaps);

		// Default states
		_D3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
		_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		// Number of pixels per logical inch along the screen height
		D3DXCreateLine(_D3DDevice, &_D3DXLine);

		Resize(renderContext->GetWidth(), renderContext->GetHeight());

		_isReady = true;
	}

	_renderTargets.Add(renderContext);

	return true;
}

bool D3D9RenderSystem::CreateVertexBuffer(uint32 size, HardwareBufferUsage usage, HardwareBuffer** vertexBuffer)
{
	if (vertexBuffer == NULL)
		return false;

	HRESULT hr;
	DWORD d3dUsage;
	D3DPOOL d3dPool = D3DPOOL_MANAGED;
	if (usage == HardwareBufferUsage_Static)
	{
		d3dUsage = D3DUSAGE_WRITEONLY;
	}
	else if (usage == HardwareBufferUsage_Dynamic)
	{
		d3dUsage = D3DUSAGE_DYNAMIC;
		d3dPool = D3DPOOL_DEFAULT;
	}
	else
	{
		d3dUsage = 0;
		Logger::Current()->Log(LogLevel::Warning, _T("D3D9RenderSystem.CreateVertexBuffer"),
			_T("Invalid hardware buffer usage."));
	}

	IDirect3DVertexBuffer9* d3dVertexBuffer;
	hr = _D3DDevice->CreateVertexBuffer(size, d3dUsage,
		0, d3dPool, &d3dVertexBuffer, 0);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.CreateVertexBuffer"),
			_T("Failed to create the vertex buffer."));
		return false;
	}

	*vertexBuffer = new D3D9VertexBuffer(d3dVertexBuffer);

	return true;
}

bool D3D9RenderSystem::CreateIndexBuffer(uint32 size, IndexBufferFormat format, HardwareBufferUsage usage, HardwareBuffer** indexBuffer)
{
	if (indexBuffer == NULL)
		return false;

	HRESULT hr;
	D3DFORMAT d3dFormat;
	if (format == IndexBufferFormat_Int16)
	{
		d3dFormat = D3DFMT_INDEX16;
	}
	else if (format == IndexBufferFormat_Int32)
	{
		d3dFormat = D3DFMT_INDEX32;
	}
	else
	{
		Logger::Current()->Log(LogLevel::Warning, _T("D3D9RenderSystem.CreateIndexBuffer"),
			_T("Invalid index buffer format."));
		return false;
	}

	DWORD d3dUsage;
	if (usage == HardwareBufferUsage_Static)
	{
		d3dUsage = D3DUSAGE_WRITEONLY;
	}
	else if (usage == HardwareBufferUsage_Dynamic)
	{
		d3dUsage = 0;
	}
	else
	{
		Logger::Current()->Log(LogLevel::Warning, _T("D3D9RenderSystem.CreateIndexBuffer"),
			_T("Invalid hardware buffer usage."));
		return false;
	}

	IDirect3DIndexBuffer9* d3dIndexBuffer;
	hr = _D3DDevice->CreateIndexBuffer(size,
        d3dUsage, d3dFormat, D3DPOOL_MANAGED, &d3dIndexBuffer, NULL);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.CreateIndexBuffer"),
			_T("Failed to create the index buffer."));
		return false;
	}

	*indexBuffer = new D3D9IndexBuffer(d3dIndexBuffer);

	return true;
}

bool D3D9RenderSystem::CreateVertexLayout(VertexLayout** vertexLayout)
{
	if (vertexLayout == NULL)
		return false;

	*vertexLayout = new D3D9VertexLayout();

	return true;
}

bool D3D9RenderSystem::UpdateVertexLayout(VertexLayout* vertexLayout)
{
	if (vertexLayout == NULL)
		return false;

	HRESULT hr;
	int32 elementCount = vertexLayout->GetElementCount();
	D3DVERTEXELEMENT9* d3dElements = new D3DVERTEXELEMENT9[elementCount + 1];

	VertexLayout::VertexElementList::Iterator it = vertexLayout->GetElementIterator();
	for (int i = 0; it.Next(); i++)
	{
		const VertexElement& element = it.Current();

		d3dElements[i].Method = D3DDECLMETHOD_DEFAULT;
		d3dElements[i].Stream = element.GetStream();
		d3dElements[i].Offset = element.GetOffset();
		//FIX
		if (element.GetVertexSemantic() == VertexSemantic_Color)
		{
			d3dElements[i].Type = D3DDECLTYPE_D3DCOLOR;
		}
		else
		{
			d3dElements[i].Type = D3D9Helper::GetVertexFormatType(element.GetVertexFormat());
		}
		d3dElements[i].Usage = D3D9Helper::GetVertexSemantic(element.GetVertexSemantic());
		d3dElements[i].UsageIndex = element.GetSemanticIndex();
	}

	// Last vertex element (D3DDECL_END)
	uint last = elementCount;
	d3dElements[last].Stream = 0xFF;
	d3dElements[last].Offset = 0;
	d3dElements[last].Type = D3DDECLTYPE_UNUSED;
	d3dElements[last].Method = 0;
	d3dElements[last].Usage = 0;
	d3dElements[last].UsageIndex = 0;

	IDirect3DVertexDeclaration9* d3dDeclaration;
	hr = _D3DDevice->CreateVertexDeclaration(d3dElements, &d3dDeclaration);
	if (FAILED(hr))
	{
		delete d3dElements;

		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.UpdateVertexLayout"),
			_T("Failed to create the vertex layout."));
		return false;
	}

	D3D9VertexLayout* d3dVertexLayout = (D3D9VertexLayout*)vertexLayout;
	d3dVertexLayout->SetDeclaration(d3dDeclaration);

	delete d3dElements;

	return true;
}

bool D3D9RenderSystem::CreateTexture(Texture** texture)
{
	if (texture == NULL)
	{
		return false;
	}

	*texture = new D3D9Texture(_D3DDevice);
	return true;
}

bool D3D9RenderSystem::CreateRenderTarget(TextureType textureType, int32 width, int32 height, RenderTexture** renderTexture)
{
	if (renderTexture == NULL)
	{
		return false;
	}

	if (textureType != TextureType_Texture2D && textureType != TextureType_TextureCube)
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.CreateRenderTarget"),
			_T("Only Texture2D and TextureCube are supported as render textures."));
		return false;
	}

	Texture* texture;
	if (!CreateTexture(&texture))
	{
		return false;
	}

	if (!texture->Create(textureType, PixelFormat_R8G8B8A8, width, height, 1, 0, TextureUsage_RenderTarget))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.CreateRenderTarget"),
			_T("Failed to create the render texture."));
		return false;
	}

	*renderTexture = new RenderTexture();
	(*renderTexture)->SetTexture(texture);
	return true;
}

void D3D9RenderSystem::Render(RenderData* renderData)
{
	if (renderData == NULL)
		return;

	if (renderData->PrimitiveCount == 0 || !IsPrimitiveTypeSupported(renderData->Type))
		return;

	if (renderData->VertexData->VertexCount == 0)
		return;

	if (renderData->IsIndexed)
	{
		if (renderData->Type == PrimitiveType_PointList)
			return;

		if (renderData->IndexData->IndexCount == 0)
			return;
	}

	D3D9VertexLayout* vertexLayout = (D3D9VertexLayout*)renderData->VertexData->VertexLayout.Get();
	if (vertexLayout == NULL)
		return;

	if (vertexLayout->GetDeclaration() == NULL)
	{
		if (!UpdateVertexLayout(vertexLayout))
		{
			return;
		}
	}

	_D3DDevice->SetVertexDeclaration(vertexLayout->GetDeclaration());

	VertexData* vertexData = renderData->VertexData;
	IndexData* indexData = renderData->IndexData;

	D3DPRIMITIVETYPE primitiveType;
	switch (renderData->Type)
	{
	case PrimitiveType_PointList:
		primitiveType = D3DPT_POINTLIST;
		break;
	case PrimitiveType_LineList:
		primitiveType = D3DPT_LINELIST;
		break;
	case PrimitiveType_LineStrip:
		primitiveType = D3DPT_LINESTRIP;
		break;
	case PrimitiveType_TriangleList:
		primitiveType = D3DPT_TRIANGLELIST;
		break;
	case PrimitiveType_TriangleStrip:
		primitiveType = D3DPT_TRIANGLESTRIP;
		break;
	case PrimitiveType_TriangleFan:
		primitiveType = D3DPT_TRIANGLEFAN;
		break;
	default:
		return;
	}

	if (renderData->IsIndexed)
	{
		D3D9IndexBuffer* indexBuffer = (D3D9IndexBuffer*)indexData->IndexBuffer.Get();
		_D3DDevice->SetIndices(indexBuffer->GetBuffer());
	}

	for (int i = 0; i < vertexData->VertexStreams.Count(); i++)
	{
		D3D9VertexBuffer* vertexBuffer = (D3D9VertexBuffer*)vertexData->VertexStreams[i].VertexBuffer.Get();
		_D3DDevice->SetStreamSource(i, vertexBuffer->GetBuffer(),
			0, vertexData->VertexStreams[i].Stride);

		HRESULT hr;
		if (renderData->IsIndexed)
		{
			hr = _D3DDevice->DrawIndexedPrimitive(primitiveType, renderData->StartVertex, 0,
				vertexData->VertexCount, renderData->StartIndex, renderData->PrimitiveCount);	
		}
		else
		{
			hr = _D3DDevice->DrawPrimitive(primitiveType, renderData->StartVertex, renderData->PrimitiveCount);	
		}

		if (FAILED(hr))
		{
			Logger::Current()->Log(LogLevel::Error, _T("D3D9RenderSystem.RenderPrimitive"),
				_T("Failed to draw the primitive."));
		}

		_D3DDevice->SetStreamSource(i, NULL, 0, 0);
	}
}

void D3D9RenderSystem::DrawPoint(Pen* pen, real x, real y)
{
	DrawLine(pen, x, y, x, y);
}

void D3D9RenderSystem::DrawLine(Pen* pen, real x0, real y0, real x1, real y1)
{
	if (pen == NULL)
		return;

	D3DXVECTOR2 vertices[2] =
	{
		D3DXVECTOR2(x0, y0),
		D3DXVECTOR2(x1, y1)
	};

	_D3DXLine->SetWidth(pen->Width);
	_D3DXLine->Draw(vertices, 2, D3D9Helper::MakeD3DColor(pen->Color));
}

void D3D9RenderSystem::DrawRectangle(Pen* pen, real x0, real y0, real x1, real y1)
{
	if (pen == NULL)
		return;

	D3DCOLOR d3dColor = D3D9Helper::MakeD3DColor(pen->Color);

	if (pen->Width > 0)
	{
		/*real32 halfWidth = (real32)pen->Width / 2.0f;
		D3DXVECTOR2 vertices[5] =
		{
			D3DXVECTOR2(x0+halfWidth, y0+halfWidth),
			D3DXVECTOR2(x1-halfWidth, y0+halfWidth),
			D3DXVECTOR2(x1-halfWidth, y1-halfWidth),
			D3DXVECTOR2(x0+halfWidth, y1-halfWidth),
			D3DXVECTOR2(x0+halfWidth, y0+halfWidth)
		};

		_D3DXLine->SetWidth(pen->Width);
		_D3DXLine->Draw(vertices, 5, d3dColor);*/

		real32 width = (real32)pen->Width;
		pen->Width = 0;
		DrawRectangle(pen, x0, y0, x1, y0+width);
		DrawRectangle(pen, x0, y1, x1, y1-width);
		DrawRectangle(pen, x0, y0+width, x0+width, y1-width);
		DrawRectangle(pen, x1-width, y0+width, x1, y1-width);
	}
	else
	{
		struct Vertex_PC
		{
			D3DXVECTOR3 position;
			D3DCOLOR color;
		};

		Vertex_PC vertices[4] =
		{
			D3DXVECTOR3(x0, y0, 0), d3dColor,
			D3DXVECTOR3(x1, y0, 0), d3dColor,
			D3DXVECTOR3(x1, y1, 0), d3dColor,
			D3DXVECTOR3(x0, y1, 0), d3dColor
		};

		_D3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(Vertex_PC));
	}
}

void D3D9RenderSystem::DrawCircle(Pen* pen, real x, real y, real radius)
{
	if (pen == NULL)
		return;

	D3DCOLOR d3dColor = D3D9Helper::MakeD3DColor(pen->Color);
	bool lines = (pen->Width > 0);

	struct Vertex_PC
	{
		D3DXVECTOR3 position;
		D3DCOLOR color;
	};

	D3DXVECTOR2 vertices_p[17];
	Vertex_PC vertices_pc[16];

	for (int i = 0; i < 16; ++i)
	{
		real ang = i * 2 * Math::Pi / 16;
		real xp = x + radius * Math::Cos(ang);
		real yp = y + radius * Math::Sin(ang);
		if (lines)
		{
            vertices_p[i].x = xp;
            vertices_p[i].y = yp;
		}
		else
		{
			vertices_pc[i].position = D3DXVECTOR3(xp, yp, 0);
			vertices_pc[i].color = d3dColor;
		}
	}

	if (lines)
	{
		// Full loop
		vertices_p[16] = vertices_p[0];
		_D3DXLine->SetWidth(pen->Width);
		_D3DXLine->Draw(vertices_p, 17, d3dColor);
	}
	else
	{
		_D3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 16 - 2, vertices_pc, sizeof(Vertex_PC));
	}
}

void D3D9RenderSystem::DrawTriangle(Pen* pen, real x0, real y0, real x1, real y1, real x2, real y2)
{
	if (pen == NULL)
		return;

	D3DCOLOR d3dColor = D3D9Helper::MakeD3DColor(pen->Color);

	if (pen->Width > 0)
	{
		D3DXVECTOR2 vertices[3] =
		{
			D3DXVECTOR2(x0, y0),
			D3DXVECTOR2(x1, y1),
			D3DXVECTOR2(x2, y2)
		};

		_D3DXLine->SetWidth(pen->Width);
		_D3DXLine->Draw(vertices, 3, pen->Color);
	}
	else
	{
		struct Vertex_PC
		{
			D3DXVECTOR3 position;
			D3DCOLOR color;
		};

		Vertex_PC vertices[3] =
		{
			D3DXVECTOR3(x0, y0, 0), d3dColor,
			D3DXVECTOR3(x1, y1, 0), d3dColor,
			D3DXVECTOR3(x2, y2, 0), d3dColor
		};

		_D3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(Vertex_PC));
	}
}

void D3D9RenderSystem::DrawPolygon(Pen* pen, const Array<Vector2>& points)
{
	if (pen == NULL)
		return;

	int count = points.Count();
	if (count < 2)
		return;

	for (int i = 0; i < count; ++i)
	{
		const Vector2 p0 = points[i];
		Vector2 p1;
		if (i < count-1)
		{
			p1 = points[i+1];
		}
		else
		{
			p1 = points[0];
		}

		D3DXVECTOR2 vertices[2] =
		{
			D3DXVECTOR2(p0.X, p0.Y),
			D3DXVECTOR2(p1.X, p1.Y)
		};

		_D3DXLine->SetWidth(pen->Width);
		_D3DXLine->Draw(vertices, 2, D3D9Helper::MakeD3DColor(pen->Color));
	}
}

}
