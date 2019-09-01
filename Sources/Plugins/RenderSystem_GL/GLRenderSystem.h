/*=============================================================================
GLRenderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLRENDERSYSTEM_H_
#define _SE_GLRENDERSYSTEM_H_

#include "GLCommon.h"

namespace SE_GL
{

/** OpenGL implementation of the renderer. */
class GLRenderSystem : public RenderSystem
{
protected:
	bool _isReady;
	RenderContext* _renderContext;
	Viewport _viewport;
	Color32 _clearColor;
	real32 _depthValue;
	uint32 _stencilValue;
	Matrix4 _projectionTransform;
	Matrix4 _worldTransform;

public:
	GLRenderSystem();
	virtual ~GLRenderSystem();

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

	RenderContext* GetCurrentContext() const { return _renderContext; }
};

class GLRenderContext : public RenderContext
{
public:
	GLRenderContext() : RenderContext() {}
	virtual ~GLRenderContext() {}

#ifdef GLEW_MX
	GLEWContext* GetGLEWContext() { return &_GLEWContext; }
#endif

protected:
#ifdef GLEW_MX
	GLEWContext _GLEWContext;
#endif
};

#ifdef GLEW_MX
#define glewGetContext() \
	((GLRenderContext*)((GLRenderSystem*)RenderSystem::Current())->GetCurrentContext())->GetGLEWContext()
#endif

}

#endif 
