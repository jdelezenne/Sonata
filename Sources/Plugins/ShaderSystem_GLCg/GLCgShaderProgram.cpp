/*=============================================================================
GLCgShaderProgram.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLCgShaderProgram.h"

namespace SE_GLCg
{

ShaderVariableType GetParameterType(CGtype value)
{
	switch (value)
	{
	case CG_BOOL:
	case CG_BOOL1:
	case CG_BOOL2:
	case CG_BOOL3:
	case CG_BOOL4:
		return ShaderVariableType_Boolean;
	case CG_UNKNOWN_TYPE:
	default:
		return ShaderVariableType_Void;
	}
}

ShaderVariableClass GetParameterClass(CGparameterclass value)
{
	switch (value)
	{
	case CG_PARAMETERCLASS_SCALAR: return ShaderVariableClass_Scalar;
	case CG_PARAMETERCLASS_VECTOR: return ShaderVariableClass_Vector;
	case CG_PARAMETERCLASS_MATRIX: return ShaderVariableClass_Matrix_Rows;
	case CG_PARAMETERCLASS_OBJECT: return ShaderVariableClass_Object;
	case CG_PARAMETERCLASS_STRUCT: return ShaderVariableClass_Struct;
	case CG_PARAMETERCLASS_UNKNOWN:
	default:
		return ShaderVariableClass_Object;
	}
}

GLCgShaderProgram::GLCgShaderProgram(GLCgShaderSystem* shaderSystem) :
	ShaderProgram(),
	_shaderSystem(shaderSystem),
	_CGprogram(NULL),
	_CGprofile(CG_PROFILE_UNKNOWN),
	_IsCompiled(false)
{
}

GLCgShaderProgram::~GLCgShaderProgram()
{
	Destroy();
}

int GLCgShaderProgram::GetParameterCount() const
{
	if (_CGprogram == NULL)
		return 0;

	int count = 0;
	CGparameter cgParameter = cgGetFirstParameter(_CGprogram, CG_PROGRAM);
	while (cgParameter != NULL)
	{
		cgParameter = cgGetNextParameter(cgParameter);
		count++;
	}

	return count;
}

CGparameter GLCgShaderProgram::GetCgParameterByIndex(int index) const
{
	int count = 0;
	CGparameter cgParameter = cgGetFirstParameter(_CGprogram, CG_PROGRAM);
	while (cgParameter != NULL)
	{
		if (index == count)
			break;

		cgParameter = cgGetNextParameter(cgParameter);
		count++;
	}

	if (index != count)
	{
		return NULL;
	}

	return cgParameter;
}

ShaderParameter* GLCgShaderProgram::GetParameterByIndex(int index) const
{
	if (_CGprogram == NULL)
		return NULL;

	int count = 0;
	CGparameter cgParameter = GetCgParameterByIndex(index);
	if (cgParameter == NULL)
	{
		return NULL;
	}

	ShaderParameter* parameter = new ShaderParameter();
	parameter->SetName(cgGetParameterName(cgParameter));
	return parameter;
}

ShaderParameter* GLCgShaderProgram::GetParameterByName(const String& name) const
{
	if (_CGprogram == NULL)
		return NULL;

	CGparameter cgParameter = cgGetNamedParameter(_CGprogram, name.Data());
	if (cgParameter == NULL)
		return NULL;

	CGenum cgVariability = cgGetParameterVariability(cgParameter);
	ShaderParameterVariability variability;
	switch (cgVariability)
	{
	case CG_UNIFORM:
		variability = ShaderParameterVariability_Uniform;
	case CG_LITERAL:
		variability = ShaderParameterVariability_Literal;
	default: return NULL;
	}

	ShaderParameter* parameter = new ShaderParameter();
	parameter->SetName(cgGetParameterName(cgParameter));
	parameter->SetParameterVariability(variability);
 
	ShaderParameterType type;
	CGtype cgType = cgGetParameterNamedType(cgParameter);
	parameter->SetParameterType(type);
	return parameter;
}

bool GLCgShaderProgram::SetParameter(int index, ShaderParameter* parameter)
{
	if (_CGprogram == NULL)
		return false;

	if (parameter == NULL)
		return false;

	// Get the Cg parameter
	CGparameter cgParameter = GetCgParameterByIndex(index);
	if (cgParameter == NULL)
		return false;

	return SetCgParameter(cgParameter, parameter);
}

bool GLCgShaderProgram::SetParameter(const String& name, ShaderParameter* parameter)
{
	if (_CGprogram == NULL)
		return false;

	if (parameter == NULL)
		return false;

	// Get the Cg parameter
	CGparameter cgParameter = cgGetNamedParameter(_CGprogram, name.Data());
	if (cgParameter == NULL)
		return false;

	return SetCgParameter(cgParameter, parameter);
}

bool GLCgShaderProgram::SetCgParameter(CGparameter cgParameter, ShaderParameter* parameter)
{
	// Check the parameter type
	ShaderParameterType type = parameter->GetParameterType();
/*
	CGtype cgType = cgGetParameterType(cgParameter);
	if (GetParameterType(cgType) != type.Type)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLCgShaderProgram.SetParameter"),
			_T("Invalid parameter type."));
		return false;
	}

	CGparameterclass cgClass = cgGetParameterClass(cgParameter);
	if (GetParameterClass(cgClass) != type.Class)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLCgShaderProgram.SetParameter"),
			_T("Invalid parameter class."));
		return false;
	}
*/
	void* data = NULL;
	parameter->GetRawValue(&data);

	// Set the parameter value
	if (type._Type == ShaderVariableType_Float && type._Class == ShaderVariableClass_Scalar)
		cgGLSetParameter4fv(cgParameter, (float*)data);
	else if (type._Type == ShaderVariableType_Float && type._Class == ShaderVariableClass_Vector)
		cgGLSetParameter4fv(cgParameter, (float*)data);
	else if (type._Type == ShaderVariableType_Float && type._Class == ShaderVariableClass_Matrix_Rows)
		cgGLSetMatrixParameterfr(cgParameter, (float*)data);

	if (!cgCheckError())
		return false;

	return true;
}

bool GLCgShaderProgram::Compile()
{
	Destroy();

	_IsCompiled = false;

	if (_EntryPoint.IsEmpty())
	{
		_EntryPoint = _T("main");
	}

	// if no profile is specified, get the highest  profile supported
	if (!_Profile.IsEmpty())
	{
		_CGprofile = cgGetProfile(_Profile.Data());
	}
	else
	{
		if (GetProgramType() == ShaderProgramType_Vertex)
			_CGprofile = _shaderSystem->GetCGVertexProfile();
		else if (GetProgramType() == ShaderProgramType_Pixel)
			_CGprofile = _shaderSystem->GetCGPixelProfile();

		_Profile = cgGetProfileString(_CGprofile);
	}

	cgGLSetOptimalOptions(_CGprofile);

	_CGprogram = cgCreateProgram(_shaderSystem->GetCGContext(), CG_SOURCE,
		_SourceData.Data(), _CGprofile, _EntryPoint.Data(), NULL);

	if (_CGprogram == NULL)
	{
		const char* listing = cgGetLastListing(_shaderSystem->GetCGContext());
		if (listing != NULL)
		{
			_ErrorMessages = listing;
		}
		else
		{
			_ErrorMessages = String::Empty;
		}

		Logger::Current()->Log(LogLevel::Error, _T("GLCgShaderProgram.Compile (cgCreateProgram)"),
			_T("Failed to create the shader program: ") + _ErrorMessages);
		return false;
	}

	// Assemble the shader
	cgGLLoadProgram(_CGprogram);
	if (!cgCheckError())
		return false;

	_IsCompiled = true;

	return true;
}

bool GLCgShaderProgram::Create()
{
	return true;
}

void GLCgShaderProgram::Destroy()
{
	if (_CGprogram != NULL)
		cgDestroyProgram(_CGprogram);
}

GLCgVertexShaderProgram::GLCgVertexShaderProgram(GLCgShaderSystem* shaderSystem) :
	GLCgShaderProgram(shaderSystem)
{
}

GLCgVertexShaderProgram::~GLCgVertexShaderProgram()
{
	Destroy();
}

bool GLCgVertexShaderProgram::Bind()
{
	if (_CGprogram == NULL)
		return false;

	cgGLBindProgram(_CGprogram);
	cgGLEnableProfile(_CGprofile);

	return true;
}

bool GLCgVertexShaderProgram::Unbind()
{
	if (_CGprogram == NULL)
		return false;

	cgGLDisableProfile(_CGprofile);
	cgGLUnbindProgram(_CGprofile);

	return true;
}

GLCgPixelShaderProgram::GLCgPixelShaderProgram(GLCgShaderSystem* shaderSystem) :
	GLCgShaderProgram(shaderSystem)
{
}

GLCgPixelShaderProgram::~GLCgPixelShaderProgram()
{
	Destroy();
}

bool GLCgPixelShaderProgram::Bind()
{
	if (_CGprogram == NULL)
		return false;

	cgGLBindProgram(_CGprogram);
	cgGLEnableProfile(_CGprofile);

	return true;
}

bool GLCgPixelShaderProgram::Unbind()
{
	if (_CGprogram == NULL)
		return false;

	cgGLDisableProfile(_CGprofile);
	cgGLUnbindProgram(_CGprofile);

	return true;
}

}
