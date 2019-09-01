/*=============================================================================
RenderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RENDERSYSTEM_H_
#define _SE_RENDERSYSTEM_H_

#include "Graphics/Common.h"
#include "Graphics/Image.h"
#include "Graphics/Viewport.h"
#include "Graphics/Model/MeshPart.h"
#include "Graphics/System/HardwareBuffer.h"
#include "Graphics/System/RenderContext.h"
#include "Graphics/System/RenderTarget.h"
#include "Graphics/System/RenderTexture.h"
#include "Graphics/States/States.h"

namespace SonataEngine
{

const int SE_MAX_TEXTURE_STAGES = 8;

class SE_GRAPHICS_EXPORT Pen
{
public:
	Color32 Color;
	int32 Width;
	Matrix4 Transform;
};

/** Base class for the renderers. */
class SE_GRAPHICS_EXPORT RenderSystem : public Manager, public Context<RenderSystem>
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	RenderSystem();

	/** Destructor. */
	virtual ~RenderSystem();
	//@}

	/** @name Capabilities. */
	//@{
	virtual bool IsPrimitiveTypeSupported(PrimitiveType primitiveType) const = 0;
	//@}

	/** @name Properties. */
	//@{
	virtual Viewport GetViewport() = 0;
	virtual void SetViewport(const Viewport& value) = 0;

	virtual const Color32& GetClearColor() const = 0;
	virtual void SetClearColor(const Color32& value) = 0;

	virtual real32 GetDepthValue() const = 0;
	virtual void SetDepthValue(real32 value) = 0;

	virtual uint32 GetStencilValue() const = 0;
	virtual void SetStencilValue(uint32 value) = 0;
	//@}

	/** @name Render States. */
	//@{
	/** Scissor. */
	virtual void SetFillMode(FillMode mode) = 0;

	virtual void SetShadeMode(ShadeMode mode) = 0;

	virtual void SetCullMode(CullMode mode) = 0;

	virtual void SetColorWriteEnable(ColorFlag value) = 0;

	virtual void SetDepthState(const DepthState& state) = 0;

	virtual void SetStencilState(const StencilState& state) = 0;

	virtual void SetScissorState(const ScissorState& state) = 0;

	virtual void SetDithering(bool value) = 0;

	virtual void SetPointState(const PointState& state) = 0;

	virtual void SetAlphaState(const AlphaState& state) = 0;

	virtual void SetBlendModes(BlendMode source, BlendMode destination) = 0;

	virtual void SetSamplerState(int stage, const SamplerState& state) = 0;

	virtual void DisableSamplerState(int stage) = 0;
	//@}

	/** @name Fixed Pipeline. */
	//@{
	virtual const Matrix4& GetProjectionTransform() = 0;
	virtual void SetProjectionTransform(const Matrix4& value) = 0;

	virtual const Matrix4& GetViewTransform() = 0;
	virtual void SetViewTransform(const Matrix4& value) = 0;

	virtual const Matrix4& GetWorldTransform() = 0;
	virtual void SetWorldTransform(const Matrix4& value) = 0;

	/** Ambient light color. */
	virtual void SetAmbientColor(const Color32& value) = 0;

	virtual void SetLightState(const LightState& state) = 0;

	virtual void SetMaterialState(const MaterialState& state) = 0;

	virtual void SetTextureState(int stage, const TextureState& state) = 0;

	virtual void SetFogState(const FogState& state) = 0;
	//@}

	/** @name Operations. */
	//@{
	/** Creates the render system. */
	virtual	bool Create();

	/** Destroys the render system. */
	virtual	void Destroy() = 0;

	/** Updates the render system. */
	virtual void Update(const TimeValue& timeValue);

	/** Creates the scene. */
	virtual bool Resize(uint32 width, uint32 height) = 0;

	/** Clears all the buffers. */
	virtual void Clear() = 0;

	/** Clears all the buffers. */
	virtual void ClearColor() = 0;

	/** Clears the depth buffer. */
	virtual void ClearDepth() = 0;

	/** Clears the stencil buffer. */
	virtual void ClearStencil() = 0;

	/** Begins a scene. */
	virtual void BeginScene() = 0;

	/** Ends a scene. */
	virtual void EndScene() = 0;

	/**
		Swap the color buffers using the specified handle for the destination window.
		@param handle The handle of the destination window, or NULL for the default window.
	*/
	virtual void SwapBuffers(WindowHandle handle = NULL) = 0;

	/** Gets the content of the color buffer. */
	virtual void GetColorBuffer(Image** image) = 0;

	/** Renders data. */
	virtual void Render(RenderData* renderData) = 0;

	/** Sets the current render target. */
	virtual void SetRenderTarget(int index, RenderTarget* value) = 0;

	/* Restore the backbuffer render target. */
	virtual void RestoreRenderTarget(int index) = 0;
	//@}

	/** @name Resources. */
	//@{
	/** Creates a render context. */
	virtual bool CreateRenderContext(Window* window, const RenderContextDescription& desc) = 0;

	/** Creates a vertex buffer. */
	virtual bool CreateVertexBuffer(uint32 size, HardwareBufferUsage usage, HardwareBuffer** vertexBuffer) = 0;

	/** Creates an index buffer. */
	virtual bool CreateIndexBuffer(uint32 size, IndexBufferFormat format, HardwareBufferUsage usage, HardwareBuffer** indexBuffer) = 0;

	/** Creates a vertex layout. */
	virtual bool CreateVertexLayout(VertexLayout** vertexLayout) = 0;

	/** Updates a vertex layout. */
	virtual bool UpdateVertexLayout(VertexLayout* vertexLayout) = 0;

	/** Creates a texture. */
	virtual bool CreateTexture(Texture** texture) = 0;

	/** Creates a render target. */
	virtual bool CreateRenderTarget(TextureType textureType, int32 width, int32 height, RenderTexture** renderTexture) = 0;
	//@}

	/** @name 2D Drawing. */
	//@{
	/** Draws a point. */
	virtual void DrawPoint(Pen* pen, real x, real y) = 0;
	SE_INLINE void DrawPoint(Pen* pen, const Vector2& p)
	{
		DrawPoint(pen, p.X, p.Y);
	}

	/** Draws a line. */
	virtual void DrawLine(Pen* pen, real x0, real y0, real x1, real y1) = 0;
	SE_INLINE void DrawLine(Pen* pen, const Vector2& p0, const Vector2& p1)
	{
		DrawLine(pen, p0.X, p0.Y, p1.X, p1.Y);
	}

	/** Draws a rectangle. */
	virtual void DrawRectangle(Pen* pen, real x0, real y0, real x1, real y1) = 0;
	SE_INLINE void DrawRectangle(Pen* pen, const Vector2& p0, const Vector2& p1)
	{
		DrawRectangle(pen, p0.X, p0.Y, p1.X, p1.Y);
	}

	/** Draws a circle. */
	virtual void DrawCircle(Pen* pen, real x, real y, real radius) = 0;
	SE_INLINE void DrawCircle(Pen* pen, const Vector2& p, real radius)
	{
		DrawCircle(pen, p.X, p.Y, radius);
	}

	/** Draws a triangle. */
	virtual void DrawTriangle(Pen* pen, real x0, real y0, real x1, real y1, real x2, real y2) = 0;
	SE_INLINE void DrawTriangle(Pen* pen, const Vector2& p0, const Vector2& p1, const Vector2& p2)
	{
		DrawTriangle(pen, p0.X, p0.Y, p1.X, p1.Y, p2.X, p2.Y);
	}

	/** Draws a polygon. */
	virtual void DrawPolygon(Pen* pen, const Array<Vector2>& points) = 0;
	//@}
};

SEPointer(RenderSystem);

}

#endif
