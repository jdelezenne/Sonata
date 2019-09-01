/*=============================================================================
SoftwareRenderer.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _SOFTWARERENDERER_H_
#define _SOFTWARERENDERER_H_

#include "OGPR/OGPR.h"
#include "Graphic/Renderer.h"
#include "Mesh/VertexDeclaration.h"
#include "Material/Shader.h"
using namespace Teesside;

// Default is float3 colour buffer
//#define ARGB_COLOUR_BUFFER

typedef Vector3 (*OGPRVertexFunction)   (OGPRVertexFuncEnv* env);
typedef Colour32 (*OGPRFragmentFunction) (OGPRFragmentFuncEnv* env);
typedef void (*OGPRPixelFunction)    (OGPRPixelFuncEnv *env);

enum DeviceBufferType
{
	DBT_ColourBuffer,
	DBT_DepthBuffer,
	DBT_StencilBuffer,
	DBT_Unused
};

struct SoftwareCaps
{
	uint32 MaxStreams;
	uint32 MaxInterpolants;
	uint32 MaxConstants;
	uint32 MaxBuffers;
};

struct SoftwareLight
{
	LightType Type;
	bool Enabled;
	Colour32 AmbientColour;
	Colour32 DiffuseColour;
	Colour32 SpecularColour;
	Vector3 Attenuations;
	Vector3 Direction;
	Vector3 Position;
	real32 Exponent;
	real32 CutOff;
};

struct SoftwareStates
{
	Colour32 Ambient;
	ScissorState ScissorState;
	FogState FogState;
	PointState PointState;
	FillMode FillMode;
	ShadeMode ShadeMode;
	CullMode CullMode;
	bool NormalizeNormals;
	bool Dithering;
	ColourFlag ColourWriteEnable;
	MaterialState MaterialState;
	bool Lighting;
	Array<SoftwareLight> Lights;
	DepthState DepthState;
	StencilState StencilState;
	AlphaState AlphaState;
	List<TextureState> Textures;
};

/** Software implementation of the renderer. */
class SoftwareRenderer : public Renderer
{
public:
	/** Constructors / Destructor. */
	//@{
	SoftwareRenderer();
	virtual ~SoftwareRenderer();
	//@}

	virtual const Colour32& GetClearColour() const { return _ClearColour; }
	virtual void SetClearColour(const Colour32& clearColour) { _ClearColour = clearColour; }

	virtual real32 GetDepthValue() const { return _DepthValue; }
	virtual void SetDepthValue(real32 depthValue) { _DepthValue = depthValue; }

	virtual int32 GetStencilValue() const { return _StencilValue; }
	virtual void SetStencilValue(int32 stencilValue) { _StencilValue = stencilValue; }

	virtual const Matrix4& GetWorldTransform();
	virtual void SetWorldTransform(const Matrix4& matWorld);
	virtual void SetAmbientColour(const Colour32& value);
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
	virtual void SetTextureState(int stage, TextureState state);
	virtual void DisableTextureState(int stage);
	virtual void SetColourWriteEnable(ColourFlag colourWriteEnable);
	virtual void SetDepthState(DepthState state);
	virtual void SetStencilState(StencilState state);
	virtual void SetAlphaState(AlphaState state);
	virtual void SetBlendModes(BlendMode sourceBlend, BlendMode destinationBlend);

	virtual bool Create(uint32 width, uint32 height);
	virtual void GetCaps();
	virtual bool Destroy();
	virtual void Clear();
	virtual void BeginScene();
	virtual void EndScene();

	virtual void DumpColourBuffer(const String& fileName);
	virtual bool CreateVertexBuffer(uint32 length, uint32 components, HardwareBuffer** vertexBuffer);
	virtual bool CreateIndexBuffer(uint32 indexSize, uint32 indexCount, HardwareBuffer** indexBuffer);
	virtual bool CreateTexture(Image* image, Texture** texture);
	virtual void Render(Mesh* mesh);
	virtual void RenderGeometry(Mesh* mesh);
	virtual void DrawText(const String& text, uint32 x, uint32 y, const Colour32& colour);

public:
	OGPRBuffer* GetColourBuffer() const { return _ColourBuffer; }
	OGPRBuffer* GetDepthBuffer() const { return _DepthBuffer; }
	OGPRBuffer* GetStencilBuffer() const { return _StencilBuffer; }

	OGPRRenderer* GetRenderer() const { return _Renderer; }
	VertexDeclaration* GetVertexDeclaration() const { return _VertexDeclaration; }
	OGPRBuffer* GetVertexBuffer() const { return _VertexBuffer; }

	SoftwareStates States;

	/** Vertices Operations. */
	//@{
	/// Clipping.
	bool Clipping(Vector4& position) { return false; }
	//@}

	/** Pixels Operations. */
	//@{
	/// Framebuffer operations.
	bool Raster(OGPRFragmentFuncEnv& env, Colour32& colour);
	//@}

protected:
	bool _Init;
	SoftwareCaps _Caps;
	Colour32 _ClearColour;
	real32 _DepthValue;
	int32 _StencilValue;
	Matrix4 _WorldTransform;
	VertexDeclaration* _VertexDeclaration;
	OGPRBuffer* _VertexBuffer;

	uint32 _Width;
	uint32 _Height;

	OGPRRenderer* _Renderer;
	OGPRBuffer* _ColourBuffer;
	OGPRBuffer* _DepthBuffer;
	OGPRBuffer* _StencilBuffer;

	/** Fragments Operations. */
	//@{
	/// Scissor testing.
	bool ScissorTest(OGPRFragmentFuncEnv& env, const Vector4& position);

	/// Alpha testing.
	bool AlphaTest(OGPRFragmentFuncEnv& env, real32 alpha);

	/// Stencil testing.
	bool StencilTest(OGPRFragmentFuncEnv& env);
	void _StencilOperation(OGPRFragmentFuncEnv& env, StencilOperation operation, int32 current);

	/// Depth testing.
	bool DepthTest(OGPRFragmentFuncEnv& env, real32 depth);

	/// Blending.
	void Blending(const Colour32& current, Colour32& colour);

	/// Fog.
	void PixelFog(real32 depth, Colour32& colour);

	/// Masking.
	void Masking(Colour32& colour);
	//@}
};

#endif
