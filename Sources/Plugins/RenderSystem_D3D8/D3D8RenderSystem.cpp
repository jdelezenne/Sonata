/*=============================================================================
D3D8RenderSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8RenderSystem.h"
#include "D3D8Helper.h"
#include "D3D8RenderContext.h"
#include "D3D8VertexBuffer.h"
#include "D3D8IndexBuffer.h"
#include "D3D8VertexLayout.h"
#include "D3D8Texture.h"

namespace SE_D3D8
{

#define D3D8_SETTEXTURESTAGE_VALUE(type, value) \
	_D3DDevice->SetTextureStageState(stage, type, value);

#define D3D8_SETTEXTURESTAGE_OP(type, value) \
	_D3DDevice->SetTextureStageState(stage, type, \
		D3D8Helper::GetTextureOperation(value));

#define D3D8_SETTEXTURESTAGE_ARG(type, value) \
	_D3DDevice->SetTextureStageState(stage, type, \
		D3D8Helper::GetTextureArgument(value));

D3D8RenderSystem::D3D8RenderSystem() :
	RenderSystem(),
	_clearColor(Color32::Black),
	_projectionTransform(Matrix4::Identity),
	_worldTransform(Matrix4::Identity)
{
	_isReady = false;
	_depthValue = 1.0f;
	_stencilValue = 0xffffffff;

	_D3DDevice = NULL;
	_D3DXFont = NULL;

	// Create the D3D object.
	_D3D = Direct3DCreate8(D3D_SDK_VERSION);
	if (_D3D == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8RenderSystem"),
			_T("Failed to create the Direct3D8 object."));
		return;
	}
}

D3D8RenderSystem::~D3D8RenderSystem()
{
	Destroy();

	SE_RELEASE(_D3D);
}

void D3D8RenderSystem::Destroy()
{
	if (!_isReady)
		return;

	SE_RELEASE(_D3DXFont);
	SE_RELEASE(_D3DDevice);
}

void D3D8RenderSystem::SetRenderTarget(int index, RenderTarget* value)
{
	//TODO
	/*IDirect3DSurface8* pRenderTarget;
	pRenderTarget = (IDirect3DSurface8*)value->GetColorSurface();
	if (pRenderTarget == NULL)
		return;

	IDirect3DSurface8* pZStencilSurface;
	if (FAILED(_D3DDevice->GetDepthStencilSurface(&pZStencilSurface)))
	{
		SE_RELEASE(pRenderTarget);
		return;
	}

	_D3DDevice->SetRenderTarget(index, pRenderTarget, pZStencilSurface);

	SE_RELEASE(pRenderTarget);
	SE_RELEASE(pZStencilSurface);*/
}

Viewport D3D8RenderSystem::GetViewport()
{
	return _viewport;
}

void D3D8RenderSystem::SetViewport(const Viewport& value)
{
	_viewport = value;

	D3DVIEWPORT8 d3dViewport;
	d3dViewport.X = _viewport.GetLeft();
	d3dViewport.Y = _viewport.GetTop();
	d3dViewport.Width = _viewport.GetWidth();
	d3dViewport.Height = _viewport.GetHeight();
	d3dViewport.MinZ = 0.0f;
	d3dViewport.MaxZ = 1.0f;
	_D3DDevice->SetViewport(&d3dViewport);
}

const Matrix4& D3D8RenderSystem::GetProjectionTransform()
{
	return _projectionTransform;
}

void D3D8RenderSystem::SetProjectionTransform(const Matrix4& value)
{
	_projectionTransform = value;

	D3DXMATRIX d3dxProjection = D3D8Helper::MakeD3DXMatrix(value);
	_D3DDevice->SetTransform(D3DTS_PROJECTION, &d3dxProjection);
}

void D3D8RenderSystem::UpdateProjectionTransform()
{
	if (_camera != NULL)
	{
		_projectionTransform = _camera->GetProjection();
		D3DXMATRIX d3dxProjection = D3D8Helper::MakeD3DXMatrix(_projectionTransform);
		_D3DDevice->SetTransform(D3DTS_PROJECTION, &d3dxProjection);
	}
}

const Matrix4& D3D8RenderSystem::GetWorldTransform()
{
	return _worldTransform;
}

void D3D8RenderSystem::SetWorldTransform(const Matrix4& value)
{
	_worldTransform = value;

	if (_camera == NULL)
	{
		D3DXMATRIX d3dxWorld = D3D8Helper::MakeD3DXMatrix(value);
		_D3DDevice->SetTransform(D3DTS_VIEW, &d3dxWorld);
	}
	else
	{
		Matrix4 matView = _camera->GetView();
		Matrix4 matWorldView = matView * value;

		D3DXMATRIX d3dxWorldView = D3D8Helper::MakeD3DXMatrix(matWorldView);
		_D3DDevice->SetTransform(D3DTS_VIEW, &d3dxWorldView);
	}
}

void D3D8RenderSystem::SetAmbientColor(const Color32& value)
{
	_D3DDevice->SetRenderState(D3DRS_AMBIENT, D3D8Helper::MakeD3DColor(value));
}

void D3D8RenderSystem::SetScissorState(ScissorState state)
{
	//TODO
}

void D3D8RenderSystem::SetFogState(FogState state)
{
	_D3DDevice->SetRenderState(D3DRS_FOGENABLE, state.Enable);

	if (state.Enable)
	{
		_D3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3D8Helper::MakeD3DColor(state.Color));
		_D3DDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)&state.Density));
		_D3DDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)&state.Start));
		_D3DDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)&state.End));
		_D3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, state.RangeEnable);
		_D3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3D8Helper::GetFogMode(state.VertexMode));
		_D3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3D8Helper::GetFogMode(state.PixelMode));
	}
}

void D3D8RenderSystem::SetPointState(PointState state)
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

void D3D8RenderSystem::SetFillMode(FillMode mode)
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

void D3D8RenderSystem::SetShadeMode(ShadeMode mode)
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

void D3D8RenderSystem::SetCullMode(CullMode mode)
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

void D3D8RenderSystem::SetNormalizeNormals(bool value)
{
	_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, value);
}

void D3D8RenderSystem::SetDithering(bool value)
{
	_D3DDevice->SetRenderState(D3DRS_DITHERENABLE, value);
}

void D3D8RenderSystem::SetLighting(bool value)
{
	_D3DDevice->SetRenderState(D3DRS_LIGHTING, value);
}

void D3D8RenderSystem::SetMaterialState(MaterialState state)
{
	D3DMATERIAL8 d3dMaterial;
	d3dMaterial.Ambient = D3D8Helper::MakeD3DXColor(state.AmbientColor);
	d3dMaterial.Diffuse = D3D8Helper::MakeD3DXColor(state.DiffuseColor);
	d3dMaterial.Specular = D3D8Helper::MakeD3DXColor(state.SpecularColor);
	d3dMaterial.Emissive = D3D8Helper::MakeD3DXColor(state.EmissiveColor);
	d3dMaterial.Power = state.Shininess;
	_D3DDevice->SetMaterial(&d3dMaterial);

	_D3DDevice->SetRenderState(D3DRS_COLORVERTEX, state.VertexColor);
}

void D3D8RenderSystem::SetLightState(LightState state)
{
	int count = 0;

	if (state.Lighting)
	{
		count = Math::Min(state.Lights.Count(), 8);
		for (int i = 0; i < count; i++)
		{
			Light* light = state.Lights[i];

			if (!light->IsEnabled())
			{
				_D3DDevice->LightEnable(i, FALSE);
				continue;
			}

			D3DLIGHT8 d3dLight;
			::ZeroMemory(&d3dLight, sizeof(D3DLIGHT8));

			d3dLight.Ambient = D3D8Helper::MakeD3DXColor(light->GetAmbientColor());
			d3dLight.Diffuse = D3D8Helper::MakeD3DXColor(light->GetDiffuseColor());
			d3dLight.Specular = D3D8Helper::MakeD3DXColor(light->GetSpecularColor());

			d3dLight.Attenuation0 = light->GetConstantAttenuation();
			d3dLight.Attenuation1 = light->GetLinearAttenuation();
			d3dLight.Attenuation2 = light->GetQuadraticAttenuation();

			if (light->GetLightType() == LightType_Directional)
			{
				DirectionalLight* dirLight = (DirectionalLight*)light;

				d3dLight.Type = D3DLIGHT_DIRECTIONAL;
				d3dLight.Direction = D3DXVECTOR3(dirLight->GetDirection());
			}
			else if (light->GetLightType() == LightType_Point)
			{
				PointLight* pointLight = (PointLight*)light;

				d3dLight.Type = D3DLIGHT_POINT;
				d3dLight.Position = D3DXVECTOR3(pointLight->GetPosition());
				d3dLight.Range = Math::Sqrt(FLT_MAX);
			}
			else if (light->GetLightType() == LightType_Spot)
			{
				SpotLight* spotLight = (SpotLight*)light;

				d3dLight.Type = D3DLIGHT_SPOT;
				d3dLight.Position = D3DXVECTOR3(spotLight->GetPosition());
				d3dLight.Direction = D3DXVECTOR3(spotLight->GetDirection());
				d3dLight.Range = Math::Sqrt(FLT_MAX);

				d3dLight.Phi = spotLight->GetFalloffExponent();
				d3dLight.Falloff = spotLight->GetCutoffAngle();
			}

			_D3DDevice->SetLight(i, &d3dLight);
			_D3DDevice->LightEnable(i, TRUE);
		}
	}

	if (count >= 8)
	{
		for (int i = count; i < 8; i++)
		{
			_D3DDevice->LightEnable(i, FALSE);
		}
	}

	SetLighting(state.Lighting);
}

void D3D8RenderSystem::SetSamplerState(int stage, SamplerState state)
{
	_D3DDevice->SetTextureStageState(stage, D3DTSS_ADDRESSU,
		D3D8Helper::GetAddressMode(state.GetAddressModeU()));
	_D3DDevice->SetTextureStageState(stage, D3DTSS_ADDRESSV,
		D3D8Helper::GetAddressMode(state.GetAddressModeV()));

	_D3DDevice->SetTextureStageState(stage, D3DTSS_MAGFILTER,
		D3D8Helper::GetFilterType(state.GetMagFilter()));
	_D3DDevice->SetTextureStageState(stage, D3DTSS_MINFILTER,
		D3D8Helper::GetFilterType(state.GetMinFilter()));
	_D3DDevice->SetTextureStageState(stage, D3DTSS_MIPFILTER,
		D3D8Helper::GetFilterType(state.GetMipFilter()));
}

void D3D8RenderSystem::SetTextureState(int stage, TextureState state)
{
	D3D8Texture* texture = (D3D8Texture*)state.GetTexture();
	if (texture == NULL)
	{
		_D3DDevice->SetTexture(stage, NULL);
	}
	else
	{
		//_D3DDevice->SetTransform(D3DTS_TEXTURE0 + stage,
		//	D3D8Helper::MakeD3DXMatrix(state.GetTextureTranform()));

		_D3DDevice->SetTexture(stage, texture->GetTexture());

		D3D8_SETTEXTURESTAGE_OP(D3DTSS_COLOROP, state.ColorOperation);
		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_COLORARG1, state.ColorArgument1);
		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_COLORARG2, state.ColorArgument2);
		D3D8_SETTEXTURESTAGE_OP(D3DTSS_ALPHAOP, state.AlphaOperation);
		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_ALPHAARG1, state.AlphaArgument1);
		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_ALPHAARG2, state.AlphaArgument2);
		D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT00, *((DWORD*)&state.BumpEnvironmentMaterial00));
		D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT01, *((DWORD*)&state.BumpEnvironmentMaterial01));
		D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT10, *((DWORD*)&state.BumpEnvironmentMaterial10));
		D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVMAT11, *((DWORD*)&state.BumpEnvironmentMaterial11));

		_D3DDevice->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX,
			D3D8Helper::GetTextureCoordinateMode(state.TextureCoordinateGeneration) |
			state.TextureCoordinateIndex);

		D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVLSCALE, *((DWORD*)&state.BumpEnvironmentLuminanceScale));
		D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_BUMPENVLOFFSET, *((DWORD*)&state.BumpEnvironmentLuminanceOffset));

		_D3DDevice->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS,
			D3D8Helper::GetTextureTransform(state.TextureTransform));

		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_COLORARG0, state.ColorArgument0);
		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_ALPHAARG0, state.AlphaArgument0);
		D3D8_SETTEXTURESTAGE_ARG(D3DTSS_RESULTARG, state.ResultArgument);
		//D3D8_SETTEXTURESTAGE_VALUE(D3DTSS_CONSTANT, D3D8Helper::MakeD3DColor(state.ConstantColor));
	}
}

void D3D8RenderSystem::DisableSamplerState(int stage)
{
	_D3DDevice->SetTexture(stage, NULL);
}

void D3D8RenderSystem::SetColorWriteEnable(ColorFlag value)
{
	DWORD d3dValue = 0;
	if ((value & ColorFlag_Red) != 0) d3dValue |= D3DCOLORWRITEENABLE_RED;
	if ((value & ColorFlag_Green) != 0) d3dValue |= D3DCOLORWRITEENABLE_GREEN;
	if ((value & ColorFlag_Blue) != 0) d3dValue |= D3DCOLORWRITEENABLE_BLUE;
    if ((value & ColorFlag_Alpha) != 0) d3dValue |= D3DCOLORWRITEENABLE_ALPHA;
	_D3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, d3dValue);
	
}

void D3D8RenderSystem::SetDepthState(DepthState state)
{
	SetZBufferEnable(state.Enable);
	SetZBufferWriteEnable(state.WriteEnable);
	SetZBufferFunction(state.Function);
	SetDepthBias(state.DepthBias);
}

void D3D8RenderSystem::SetZBufferEnable(bool value)
{
	_D3DDevice->SetRenderState(D3DRS_ZENABLE, value);
}

void D3D8RenderSystem::SetZBufferWriteEnable(bool value)
{
	_D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, value);
}

void D3D8RenderSystem::SetZBufferFunction(ComparisonFunction value)
{
	_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3D8Helper::GetCmpFunction(value));
}

void D3D8RenderSystem::SetDepthBias(real32 value)
{
	_D3DDevice->SetRenderState(D3DRS_ZBIAS, *((DWORD*)&value));
}

void D3D8RenderSystem::SetStencilState(StencilState state)
{
	_D3DDevice->SetRenderState(D3DRS_STENCILENABLE, state.Enable);
	_D3DDevice->SetRenderState(D3DRS_STENCILFAIL, state.FrontFace.Fail);
	_D3DDevice->SetRenderState(D3DRS_STENCILZFAIL, state.FrontFace.DepthBufferFail);
	_D3DDevice->SetRenderState(D3DRS_STENCILPASS, state.FrontFace.Pass);
	_D3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3D8Helper::GetCmpFunction(state.FrontFace.Function));
	_D3DDevice->SetRenderState(D3DRS_STENCILREF, state.Reference);
	_D3DDevice->SetRenderState(D3DRS_STENCILMASK, state.ReadMask);
	_D3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, state.WriteMask);
}

void D3D8RenderSystem::SetAlphaState(AlphaState state)
{
	_D3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, state.TestEnable);
	_D3DDevice->SetRenderState(D3DRS_ALPHAREF, state.Reference);
	_D3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3D8Helper::GetCmpFunction(state.Function));

	_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, state.BlendEnable[0]);
	SetBlendModes(state.SourceBlend, state.DestinationBlend);
	_D3DDevice->SetRenderState(D3DRS_BLENDOP, D3D8Helper::GetBlendOperation(state.BlendOp));
	//_D3DDevice->SetRenderState(D3DRS_BLENDFACTOR, state.BlendFactor);
}

void D3D8RenderSystem::SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend)
{
	_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3D8Helper::GetBlendMode(sourceBlend));
	_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3D8Helper::GetBlendMode(destinationBlend));
}

bool D3D8RenderSystem::Resize(uint32 width, uint32 height)
{
	return true;
}

void D3D8RenderSystem::Clear()
{
	//_D3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	_D3DDevice->Clear(
		0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER/* | D3DCLEAR_STENCIL*/,
		D3DCOLOR_COLORVALUE(_clearColor.r, _clearColor.g, _clearColor.b, 1.0f),
		_depthValue, 0);
}

void D3D8RenderSystem::BeginScene()
{
	_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	// Begin the scene
	if (FAILED(_D3DDevice->BeginScene()))
		return;

	if (_camera != NULL)
	{
		Matrix4 matProj = _camera->GetProjection();

		D3DXMATRIX d3dxProj = D3D8Helper::MakeD3DXMatrix(matProj);
		_D3DDevice->SetTransform(D3DTS_PROJECTION, &d3dxProj);
	}
}

void D3D8RenderSystem::EndScene()
{
	// End the scene
	_D3DDevice->EndScene();

	// Present the backbuffer contents to the display
	_D3DDevice->Present(NULL, NULL, NULL, NULL);

	Array<RenderTarget*>::Iterator it = _renderTargets.GetIterator();
	while (it.Next())
	{
		if (!it.Current()->IsTexture())
		{
			((D3D8RenderContext*)it.Current())->SwapBuffers();
		}
	}
}

void D3D8RenderSystem::GetColorBuffer(Image** image)
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
	IDirect3DSurface8* d3dSurface = NULL;
	_D3DDevice->CreateImageSurface(
		dm.Width, dm.Height, D3DFMT_A8R8G8B8, &d3dSurface);

	// get the front buffer
	_D3DDevice->GetFrontBuffer(d3dSurface);

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
		SEthrow(Exception("IDirect3DSurface8::LockRect"));
	}

	//*image = new Image();
	//d3dRect.pBits

	// unlock and release the surface
	d3dSurface->UnlockRect();
	SE_RELEASE(d3dSurface);
}

bool D3D8RenderSystem::CreateRenderContext(Window* window, const RenderContextDescription& desc)
{
	D3D8RenderContext* renderContext = new D3D8RenderContext();
	if (_D3DDevice != NULL)
	{
		renderContext->SetDevice(_D3DDevice);
	}
	if (!renderContext->Create(window, desc))
	{
		delete renderContext;

		Logger::Current()->Log(LogLevel::Error, _T("D3D8RenderSystem.CreateRenderContext"),
			_T("Failed to create the render context."));
		return false;
	}

	if (_D3DDevice == NULL)
	{
		_D3DDevice = renderContext->GetDevice();

		// Default states
		_D3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
		_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		_D3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		_D3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

		// Number of pixels per logical inch along the screen height
		UINT nHeight = -::MulDiv(8, ::GetDeviceCaps(::GetDC((HWND)window->GetHandle()), LOGPIXELSY), 72);

		//TODO
		HFONT hFont = NULL;//CreateFont(nHeight, 0);

		D3DXCreateFont(_D3DDevice, hFont, &_D3DXFont);

		SetViewport(Viewport(0, 0, renderContext->GetWidth(), renderContext->GetHeight()));

		_isReady = true;
	}

	_renderTargets.Add(renderContext);

	return true;
}

bool D3D8RenderSystem::CreateVertexBuffer(uint32 size, HardwareBufferUsage usage, HardwareBuffer** vertexBuffer)
{
	if (vertexBuffer == NULL)
		return false;

	HRESULT hr;
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
		Logger::Current()->Log(LogLevel::Warning, _T("D3D8RenderSystem.CreateVertexBuffer"),
			_T("Invalid hardware buffer usage."));
		return false;
	}

	IDirect3DVertexBuffer8* d3dVertexBuffer;
	hr = _D3DDevice->CreateVertexBuffer(size, d3dUsage,
		0, D3DPOOL_MANAGED, &d3dVertexBuffer);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8RenderSystem.CreateVertexBuffer"),
			_T("Failed to create the vertex buffer."));
		return false;
	}

	*vertexBuffer = new D3D8VertexBuffer(d3dVertexBuffer);

	return true;
}

bool D3D8RenderSystem::CreateIndexBuffer(uint32 size, IndexBufferFormat format, HardwareBufferUsage usage, HardwareBuffer** indexBuffer)
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
		Logger::Current()->Log(LogLevel::Warning, _T("D3D8RenderSystem.CreateIndexBuffer"),
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
		Logger::Current()->Log(LogLevel::Warning, _T("D3D8RenderSystem.CreateIndexBuffer"),
			_T("Invalid hardware buffer usage."));
		return false;
	}

	IDirect3DIndexBuffer8* d3dIndexBuffer;
	hr = _D3DDevice->CreateIndexBuffer(size,
        d3dUsage, d3dFormat, D3DPOOL_MANAGED, &d3dIndexBuffer);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8RenderSystem.CreateIndexBuffer"),
			_T("Failed to create the index buffer."));
		return false;
	}

	*indexBuffer = new D3D8IndexBuffer(d3dIndexBuffer);

	return true;
}

bool D3D8RenderSystem::CreateVertexLayout(VertexLayout** vertexLayout)
{
	if (vertexLayout == NULL)
		return false;

	*vertexLayout = new D3D8VertexLayout();

	return true;
}

bool D3D8RenderSystem::UpdateVertexLayout(VertexLayout* vertexLayout)
{
	if (vertexLayout == NULL)
		return false;

	int32 elementCount = vertexLayout->GetElementCount();
	DWORD* d3dDeclaration = new DWORD[elementCount + 2];
	d3dDeclaration[0] = D3DVSD_STREAM(0);

	VertexLayout::VertexElementList::Iterator it = vertexLayout->GetElementIterator();
	for (int i = 0; it.Next(); i++)
	{
		const VertexElement& element = it.Current();

		SHORT usage = D3D8Helper::GetVertexSemantic(element.GetVertexSemantic());
		SHORT type = D3D8Helper::GetVertexFormatType(element.GetVertexFormat());

		if (element.GetVertexSemantic() == VertexSemantic_TextureCoordinate)
		{
			usage = D3DVSDE_TEXCOORD0 + element.GetSemanticIndex();
		}

		d3dDeclaration[i+1] = D3DVSD_REG(usage, type);
	}

	// Last vertex element (D3DDECL_END)
	uint last = elementCount;
	d3dDeclaration[last] = D3DVSD_END();

	D3D8VertexLayout* d3dVertexLayout = (D3D8VertexLayout*)vertexLayout;
	d3dVertexLayout->SetDeclaration(d3dDeclaration);

	return true;
}

bool D3D8RenderSystem::CreateTexture(Image* image, Texture** texture)
{
	if (image == NULL)
		return false;

	if (texture == NULL)
		return false;

	*texture = new D3D8Texture(_D3DDevice);

	return (*texture)->Create(image);
}

bool D3D8RenderSystem::CreateRenderTarget(int32 width, int32 height, Texture** texture)
{
	return false;
}

void D3D8RenderSystem::RenderPrimitive(PrimitiveStream* primitive)
{
	if (primitive == NULL)
		return;

	VertexLayout* layout = primitive->GetVertexLayout();
	if (layout == NULL)
		return;

	if (primitive->GetVertexBuffer().GetVertexCount() == 0)
		return;

	if (primitive->IsIndexed())
	{
		if (primitive->GetPrimitiveType() == PrimitiveType_PointList)
			return;

		if (primitive->GetIndexBuffer().GetIndexCount() == 0)
			return;
	}

	HRESULT hr;

	D3D8VertexLayout* d3dLayout = (D3D8VertexLayout*)layout;
	if (d3dLayout->GetDeclaration() == NULL)
	{
		if (!UpdateVertexLayout(d3dLayout))
		{
			return;
		}
	}

	DWORD d3dFVF;
	hr = D3DXFVFFromDeclarator(d3dLayout->GetDeclaration(), &d3dFVF);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error,
			_T("D3D8RenderSystem.RenderPrimitive (D3DXFVFFromDeclarator)"),
			_T("Failed to get the FVF from the declaration."));
		return;
	}

	_D3DDevice->SetVertexShader(d3dFVF);

	const VertexBufferDescription& vbDesc = primitive->GetVertexBuffer();
	const IndexBufferDescription& ibDesc = primitive->GetIndexBuffer();

	D3D8VertexBuffer* vertexBuffer = (D3D8VertexBuffer*)vbDesc.GetBuffer();
	_D3DDevice->SetStreamSource(0, vertexBuffer->GetBuffer(), layout->GetSize());

	if (primitive->IsIndexed())
	{
		D3D8IndexBuffer* indexBuffer = (D3D8IndexBuffer*)ibDesc.GetBuffer();
		_D3DDevice->SetIndices(indexBuffer->GetBuffer(), ibDesc.GetStartIndex());
	}

	uint32 primitiveCount;
	if (primitive->IsIndexed())
		primitiveCount = ibDesc.GetIndexCount();
	else
		primitiveCount = vbDesc.GetVertexCount();

	D3DPRIMITIVETYPE d3dPrimitiveType;
	UINT d3dPrimitiveCount = 0;
	switch (primitive->GetPrimitiveType())
	{
	case PrimitiveType_PointList:
		d3dPrimitiveType = D3DPT_POINTLIST;
		d3dPrimitiveCount = primitiveCount;
		break;
	case PrimitiveType_LineList:
		d3dPrimitiveType = D3DPT_LINELIST;
		d3dPrimitiveCount = primitiveCount / 2;
		break;
	case PrimitiveType_LineStrip:
		d3dPrimitiveType = D3DPT_LINESTRIP;
		d3dPrimitiveCount = primitiveCount - 1;
		break;
	case PrimitiveType_TriangleList:
		d3dPrimitiveType = D3DPT_TRIANGLELIST;
		d3dPrimitiveCount = primitiveCount / 3;
		break;
	case PrimitiveType_TriangleStrip:
		d3dPrimitiveType = D3DPT_TRIANGLESTRIP;
		d3dPrimitiveCount = primitiveCount - 2;
		break;
	case PrimitiveType_TriangleFan:
		d3dPrimitiveType = D3DPT_TRIANGLEFAN;
		d3dPrimitiveCount = primitiveCount - 2;
		break;
	default:
		d3dPrimitiveType = D3DPT_POINTLIST;
		d3dPrimitiveCount = primitiveCount;
	}

	if (primitive->IsIndexed())
	{
		hr = _D3DDevice->DrawIndexedPrimitive(d3dPrimitiveType,
			ibDesc.GetMinimumVertexIndex(),
			vbDesc.GetVertexCount(), ibDesc.GetStartIndex(), d3dPrimitiveCount);	
	}
	else
	{
		hr = _D3DDevice->DrawPrimitive(d3dPrimitiveType, vbDesc.GetStartVertex(), d3dPrimitiveCount);	
	}

	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8RenderSystem.RenderPrimitive"),
			_T("Failed to draw the primitive."));
	}
}

void D3D8RenderSystem::DrawPoint(Pen* pen, real x, real y)
{
	DrawLine(pen, x, y, x, y);
}

void D3D8RenderSystem::DrawLine(Pen* pen, real x0, real y0, real x1, real y1)
{
	if (pen == NULL)
		return;

	D3DXVECTOR2 vertices[2] =
	{
		D3DXVECTOR2(x0, y0),
		D3DXVECTOR2(x1, y1)
	};

	//_D3DXLine->SetWidth(pen->Width);
	//_D3DXLine->Draw(vertices, 2, D3D8Helper::MakeD3DColor(pen->Color));
}

void D3D8RenderSystem::DrawRectangle(Pen* pen, real x0, real y0, real x1, real y1)
{
	if (pen == NULL)
		return;

	D3DCOLOR d3dColor = D3D8Helper::MakeD3DColor(pen->Color);

	if (pen->Width > 0)
	{
		D3DXVECTOR2 vertices[5] =
		{
			D3DXVECTOR2(x0, y0),
			D3DXVECTOR2(x1, y0),
			D3DXVECTOR2(x1, y1),
			D3DXVECTOR2(x0, y1),
			D3DXVECTOR2(x0, y0)
		};

		//_D3DXLine->SetWidth(pen->Width);
		//_D3DXLine->Draw(vertices, 5, d3dColor);
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

		_D3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(Vertex_PC));
	}
}

void D3D8RenderSystem::DrawCircle(Pen* pen, real x, real y, real radius)
{
	if (pen == NULL)
		return;

	D3DCOLOR d3dColor = D3D8Helper::MakeD3DColor(pen->Color);
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
		//_D3DXLine->SetWidth(pen->Width);
		//_D3DXLine->Draw(vertices_p, 17, d3dColor);
	}
	else
	{
		_D3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 16 - 2, vertices_pc, sizeof(Vertex_PC));
	}
}

void D3D8RenderSystem::DrawTriangle(Pen* pen, real x0, real y0, real x1, real y1, real x2, real y2)
{
	if (pen == NULL)
		return;

	D3DCOLOR d3dColor = D3D8Helper::MakeD3DColor(pen->Color);

	if (pen->Width > 0)
	{
		D3DXVECTOR2 vertices[3] =
		{
			D3DXVECTOR2(x0, y0),
			D3DXVECTOR2(x1, y1),
			D3DXVECTOR2(x2, y2)
		};

		//_D3DXLine->SetWidth(pen->Width);
		//_D3DXLine->Draw(vertices, 3, pen->Color);
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

		_D3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(Vertex_PC));
	}
}

void D3D8RenderSystem::DrawPolygon(Pen* pen, const Array<Vector2>& points)
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

		//_D3DXLine->SetWidth(pen->Width);
		//_D3DXLine->Draw(vertices, 2, D3D8Helper::MakeD3DColor(pen->Color));
	}
}

}
