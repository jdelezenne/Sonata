/*=============================================================================
SoftwareShader.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _SOFTWARESHADER_H_
#define _SOFTWARESHADER_H_

#include "OGPR/OGPR.h"
#include "Material/Shader.h"
#include "Software/SoftwareRenderer.h"
using namespace Teesside;

/**
 * Software shader.
 */
class SoftwareShader : public Shader
{
public:
	/** Constructors / Destructor. */
	//@{
	SoftwareShader();
	virtual ~SoftwareShader();
	//@}

	static OGPRVertexFuncEnv* GetVertexEnvironment() { return _VertexEnv; }
	static void SetVertexEnvironment(OGPRVertexFuncEnv* env) { _VertexEnv = env; }

	static OGPRFragmentFuncEnv* GetFragmentEnvironment() { return _FragmentEnv; }
	static void SetFragmentEnvironment(OGPRFragmentFuncEnv* env) { _FragmentEnv = env; }

	virtual void Initialize(Renderer* renderer, Mesh* mesh);

	virtual Vector4 vs_main(OGPRVertexFuncEnv* env) = 0;
	virtual Colour32 fs_main(OGPRFragmentFuncEnv* env) = 0;

protected:
	static OGPRVertexFuncEnv* _VertexEnv;
	static OGPRFragmentFuncEnv* _FragmentEnv;

	SoftwareStates States;
	bool* VertexSM;
	bool* FragmentSM;

	void inStream(uint32 stream, void* s);
};

Colour32 tex1D(uint32 tex, real32 s);
Colour32 tex2D(uint32 tex, const Vector2& s);
Colour32 tex3D(uint32 tex, const Vector3& s);
Colour32 texCUBE(uint32 tex, const Vector3& s);
Colour32 texRECT(uint32 tex, const Vector2& s);

#endif
