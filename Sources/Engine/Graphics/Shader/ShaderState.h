/*=============================================================================
ShaderState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHADERSTATE_H_
#define _SE_SHADERSTATE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	@brief Base class for the shader states.
*/
class SE_GRAPHICS_EXPORT ShaderState : public RefObject
{
protected:
	String _state;
	String _value;

public:
	/** Destructor. */
	virtual ~ShaderState();

	/** Gets or sets the state name. */
	const String& GetState() const { return _state; }
	void SetState(const String& value) { _state = value; }

	/** Gets or sets the state value. */
	const String& GetValue() const { return _value; }
	void SetValue(const String& value) { _value = value; }

protected:
	/** Constructor. */
	ShaderState();
};

}

#endif
