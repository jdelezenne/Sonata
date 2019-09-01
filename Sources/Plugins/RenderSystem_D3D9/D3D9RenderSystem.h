/*=============================================================================
D3D9RenderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9RENDERSYSTEM_H_
#define _SE_D3D9RENDERSYSTEM_H_

#ifdef _WIN32
#include <Platforms/Win32/Win32Platform.h>
#include <windows.h>
#endif

#include <d3d9.h>
#include <d3dx9.h>

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_D3D9
{

/** Direct3D 9 implementation of the renderer. */
class D3D9RenderSystem : public RenderSystem
{
protected:
	bool _isReady;
	Viewport _viewport;
	Color32 _clearColor;
	real32 _depthValue;
	uint32 _stencilValue;
	Matrix4 _projectionTransform;
	Matrix4 _viewTransform;
	Matrix4 _worldTransform;

	Array<RenderTarget*> _renderTargets;

	LPDIRECT3D9 _D3D;
	IDirect3DDevice9* _D3DDevice;
	D3DCAPS9 _D3DCaps;
	IDirect3DSurface9* _D3DBackBuffer;
	LPD3DXFONT _D3DXFont;
	LPD3DXLINE _D3DXLine;

public:
	D3D9RenderSystem();
	virtual ~D3D9RenderSystem();

	virtual bool IsPrimitiveTypeSupported(PrimitiveType primitiveType) const;

	virtual Viewport GetViewport();
	virtual void SetViewport(const Viewport& viewport);

	virtual const Color32& GetClearColor() const { return _clearColor; }
	virtual void SetClearColor(const Color32& clearColor) { _clearColor = clearColor; }

	virtual real32 GetDepthValue() const { return _depthValue; }
	virtual void SetDepthValue(real32 depthValue) { _depthValue = depthValue; }

	virtual uint32 GetStencilValue() const { return _stencilValue; }
	virtual void SetStencilValue(uint32 stencilValue) { _stencilValue = stencilValue; }

	virtual void SetFillMode(FillMode mode);
	virtual void SetShadeMode(ShadeMode mode);
	virtual void SetCullMode(CullMode mode);
	virtual void SetColorWriteEnable(ColorFlag value);
	virtual void SetDepthState(const DepthState& state);
	virtual void SetStencilState(const StencilState& state);
	virtual void SetScissorState(const ScissorState& state);
	virtual void SetDithering(bool value);
	virtual void SetPointState(const PointState& state);
	virtual void SetAlphaState(const AlphaState& state);
	virtual void SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend);
	virtual void SetSamplerState(int stage, const SamplerState& state);
	virtual void DisableSamplerState(int stage);

	virtual const Matrix4& GetProjectionTransform();
	virtual void SetProjectionTransform(const Matrix4& value);

	virtual const Matrix4& GetViewTransform();
	virtual void SetViewTransform(const Matrix4& value);

	virtual const Matrix4& GetWorldTransform();
	virtual void SetWorldTransform(const Matrix4& value);

	virtual void SetAmbientColor(const Color32& value);
	virtual void SetLightState(const LightState& state);
	virtual void SetMaterialState(const MaterialState& state);
	virtual void SetTextureState(int stage, const TextureState& state);
	virtual void SetFogState(const FogState& state);

	virtual void Destroy();
	virtual bool Resize(uint32 width, uint32 height);

	virtual void Clear();
	virtual void ClearColor();
	virtual void ClearDepth();
	virtual void ClearStencil();

	virtual void BeginScene();
	virtual void EndScene();
	virtual void SwapBuffers(WindowHandle handle = NULL);

	virtual void GetColorBuffer(Image** image);
	virtual void Render(RenderData* renderData);

	virtual void SetRenderTarget(int index, RenderTarget* value);
	virtual void RestoreRenderTarget(int index);

	virtual bool CreateRenderContext(Window* window, const RenderContextDescription& desc);
	virtual bool CreateVertexBuffer(uint32 size, HardwareBufferUsage usage, HardwareBuffer** vertexBuffer);
	virtual bool CreateIndexBuffer(uint32 size, IndexBufferFormat format, HardwareBufferUsage usage, HardwareBuffer** indexBuffer);
	virtual bool CreateVertexLayout(VertexLayout** vertexLayout);
	virtual bool UpdateVertexLayout(VertexLayout* vertexLayout);
	virtual bool CreateTexture(Texture** texture);
	virtual bool CreateRenderTarget(TextureType textureType, int32 width, int32 height, RenderTexture** renderTexture);

	virtual void DrawPoint(Pen* pen, real x, real y);
	virtual void DrawLine(Pen* pen, real x0, real y0, real x1, real y1);
	virtual void DrawRectangle(Pen* pen, real x0, real y0, real x1, real y1);
	virtual void DrawCircle(Pen* pen, real x, real y, real radius);
	virtual void DrawTriangle(Pen* pen, real x0, real y0, real x1, real y1, real x2, real y2);
	virtual void DrawPolygon(Pen* pen, const Array<Vector2>& points);

	LPDIRECT3D9 GetD3D() const { return _D3D; }
	IDirect3DDevice9* GetD3DDevice() const { return _D3DDevice; }
};
 
}

#endif
