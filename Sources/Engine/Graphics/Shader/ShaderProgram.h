/*=============================================================================
ShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHADERPROGRAM_H_
#define _SE_SHADERPROGRAM_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/ShaderParameter.h"

namespace SonataEngine
{

class ShaderSystem;

/** Shader program type. */
enum ShaderProgramType
{
	/// Vertex Shader.
	ShaderProgramType_Vertex,

	/// Pixel Shader.
	ShaderProgramType_Pixel,

	/// Geometry Shader.
	ShaderProgramType_Geometry
};

/**
	@brief Base class for the Shader Programs.
*/
class SE_GRAPHICS_EXPORT ShaderProgram : public NamedObject
{
public:
	/** @name Properties. */
	//@{
	virtual ShaderSystem* GetShaderSystem() const = 0;

	virtual ShaderProgramType GetProgramType() const = 0;

	virtual String GetSourceData() const = 0;
	virtual void SetSourceData(String value) = 0;

	virtual String GetEntryPoint() const = 0;
	virtual void SetEntryPoint(String value) = 0;

	virtual String GetProfile() const = 0;
	virtual void SetProfile(String value) = 0;
	//@}

	/** @name Parameters. */
	//@{
	virtual int GetParameterCount() const = 0;
	virtual ShaderParameter* GetParameterByIndex(int index) const = 0;
	virtual ShaderParameter* GetParameterByName(const String& name) const = 0;
	//@}

	/** @name Operations. */
	//@{
	virtual bool IsCompiled() const = 0;
	virtual bool Compile() = 0;
	virtual String GetErrorMessages() const = 0;

	virtual bool Create() = 0;
	virtual bool Bind() = 0;
	virtual bool Unbind()= 0;
	//@}
};

SEPointer(ShaderProgram);

}

#endif 
