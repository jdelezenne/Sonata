/*=============================================================================
D3D8RenderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8RENDERSYSTEM_H_
#define _SE_D3D8RENDERSYSTEM_H_

#ifdef _WIN32
#include <Platforms/Win32/Win32Platform.h>

#include <windows.h>
#endif

#include <d3d8.h>
#include <d3dx8.h>

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_D3D8
{

/** Direct3D 8 implementation of the renderer. */
class D3D8RenderSystem : public RenderSystem
{
protected:
	bool _isReady;
	Viewport _viewport;
	Color32 _clearColor;
	real32 _depthValue;
	uint32 _stencilValue;
	Matrix4 _projectionTransform;
	Matrix4 _worldTransform;

	Array<RenderTarget*> _renderTargets;

	LPDIRECT3D8 _D3D;
	IDirect3DDevice8* _D3DDevice;
	LPD3DXFONT _D3DXFont;

public:
	D3D8RenderSystem();
	virtual ~D3D8RenderSystem();

	virtual void Destroy();

	virtual void SetRenderTarget(int index, RenderTarget* value);

	virtual Viewport GetViewport();
	virtual void SetViewport(const Viewport& viewport);

	virtual const Color32& GetClearColor() const { return _clearColor; }
	virtual void SetClearColor(const Color32& clearColor) { _clearColor = clearColor; }

	virtual real32 GetDepthValue() const { return _depthValue; }
	virtual void SetDepthValue(real32 depthValue) { _depthValue = depthValue; }

	virtual uint32 GetStencilValue() const { return _stencilValue; }
	virtual void SetStencilValue(uint32 stencilValue) { _stencilValue = stencilValue; }

	virtual const Matrix4& GetProjectionTransform();
	virtual void SetProjectionTransform(const Matrix4& value);
	virtual void UpdateProjectionTransform();

	virtual const Matrix4& GetWorldTransform();
	virtual void SetWorldTransform(const Matrix4& value);

	virtual void SetAmbientColor(const Color32& value);
	virtual void SetScissorState(ScissorState state);
	virtual void SetFogState(FogState state);
	virtual void SetPointState(PointState state);
	virtual void SetFillMode(FillMode mode);
	virtual void SetShadeMode(ShadeMode mode);
	virtual void SetCullMode(CullMode mode);
	virtual void SetNormalizeNormals(bool value);
	virtual void SetDithering(bool value);
	virtual void SetLighting(bool value);
	virtual void SetMaterialState(MaterialState state);
	virtual void SetLightState(LightState state);
	virtual void SetSamplerState(int stage, SamplerState state);
	virtual void SetTextureState(int stage, TextureState state);
	virtual void DisableSamplerState(int stage);
	virtual void SetColorWriteEnable(ColorFlag value);

	virtual void SetDepthState(DepthState state);
	void SetZBufferEnable(bool value);
	void SetZBufferWriteEnable(bool value);
	void SetZBufferFunction(ComparisonFunction value);
	void SetDepthBias(real32 value);

	virtual void SetStencilState(StencilState state);
	virtual void SetAlphaState(AlphaState state);
	virtual void SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend);

	virtual bool Resize(uint32 width, uint32 height);
	virtual void Clear();
	virtual void BeginScene();
	virtual void EndScene();

	virtual void GetColorBuffer(Image** image);
	virtual void RenderPrimitive(PrimitiveStream* primitive);

	virtual bool CreateRenderContext(Window* window, const RenderContextDescription& desc);
	virtual bool CreateVertexBuffer(uint32 size, HardwareBufferUsage usage, HardwareBuffer** vertexBuffer);
	virtual bool CreateIndexBuffer(uint32 size, IndexBufferFormat format, HardwareBufferUsage usage, HardwareBuffer** indexBuffer);
	virtual bool CreateVertexLayout(VertexLayout** vertexLayout);
	virtual bool UpdateVertexLayout(VertexLayout* vertexLayout);
	virtual bool CreateTexture(Image* image, Texture** texture);
	virtual bool CreateRenderTarget(int32 width, int32 height, Texture** texture);

	virtual void DrawPoint(Pen* pen, real x, real y);
	virtual void DrawLine(Pen* pen, real x0, real y0, real x1, real y1);
	virtual void DrawRectangle(Pen* pen, real x0, real y0, real x1, real y1);
	virtual void DrawCircle(Pen* pen, real x, real y, real radius);
	virtual void DrawTriangle(Pen* pen, real x0, real y0, real x1, real y1, real x2, real y2);
	virtual void DrawPolygon(Pen* pen, const Array<Vector2>& points);

	LPDIRECT3D8 GetD3D() const { return _D3D; }
	IDirect3DDevice8* GetD3DDevice() const { return _D3DDevice; }
};
 
}

#endif
