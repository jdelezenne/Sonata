/*=============================================================================
GLSLShaderProgram.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLSLShaderProgram.h"

namespace SE_GL
{
/*
ShaderVariableType GetParameterType(SLtype value)
{
	switch (value)
	{
	case SL_BOOL:
	case SL_BOOL1:
	case SL_BOOL2:
	case SL_BOOL3:
	case SL_BOOL4:
		return ShaderVariableType_Boolean;
	case SL_UNKNOWN_TYPE:
	default:
		return ShaderVariableType_Void;
	}
}

ShaderVariableClass GetParameterClass(SLparameterclass value)
{
	switch (value)
	{
	case SL_PARAMETERCLASS_SCALAR: return ShaderVariableClass_Scalar;
	case SL_PARAMETERCLASS_VECTOR: return ShaderVariableClass_Vector;
	case SL_PARAMETERCLASS_MATRIX: return ShaderVariableClass_Matrix_Rows;
	case SL_PARAMETERCLASS_OBJECT: return ShaderVariableClass_Object;
	case SL_PARAMETERCLASS_STRUCT: return ShaderVariableClass_Struct;
	case SL_PARAMETERCLASS_UNKNOWN:
	default:
		return ShaderVariableClass_Object;
	}
}
*/
GLhandleARB GLSLShaderProgram::_GLProgram = NULL;//fix

GLSLShaderProgram::GLSLShaderProgram(GLSLShaderSystem* shaderSystem) :
	ShaderProgram(),
	_shaderSystem(shaderSystem),
	//_GLProgram(NULL),
	_GLShader(NULL),
	_IsCompiled(false)
{
}

GLSLShaderProgram::~GLSLShaderProgram()
{
	Destroy();
}

int GLSLShaderProgram::GetParameterCount() const
{
	if (_GLProgram == NULL)
		return 0;

	int count;
    glGetObjectParameterivARB(_GLProgram, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);

	return count;
}

ShaderParameter* GLSLShaderProgram::GetParameterByIndex(int index) const
{
	if (_GLProgram == NULL)
		return NULL;

	int count = GetParameterCount();
	if (index < 0 || index >= count)
		return NULL;

	GLint maxLength;
	GLcharARB* name;
	GLsizei length;
	GLint size;
	GLenum type;

	glGetObjectParameterivARB(_GLProgram, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &maxLength);
	name = new GLcharARB[maxLength];

	glGetActiveUniformARB(_GLProgram, index, maxLength, &length, &size, &type, name);

	ShaderParameter* parameter = new ShaderParameter();

	parameter->SetName(name);
	delete name;

	return parameter;
}

ShaderParameter* GLSLShaderProgram::GetParameterByName(const String& name) const
{
	if (_GLProgram == NULL)
		return NULL;

	GLint glParameter = glGetUniformLocationARB(_GLProgram, name.Data());
	if (glParameter == 0)
		return NULL;

	return GetParameterByIndex(glParameter);
}

bool GLSLShaderProgram::SetParameter(int index, ShaderParameter* parameter)
{
	if (_GLProgram == NULL)
		return false;

	if (parameter == NULL)
		return false;

	// Get the GLSL parameter
	int count = GetParameterCount();
	if (index < 0 || index >= count)
		return false;

	return SetGLParameter(index, parameter);
}

bool GLSLShaderProgram::SetParameter(const String& name, ShaderParameter* parameter)
{
	if (_GLProgram == NULL)
		return false;

	if (parameter == NULL)
		return false;

	// Get the GLSL parameter
	GLint glParameter = glGetUniformLocationARB(_GLProgram, name.Data());
	if (glParameter == 0)
		return false;

	return SetGLParameter(glParameter, parameter);
}

bool GLSLShaderProgram::SetGLParameter(GLint glParameter, ShaderParameter* parameter)
{
	// Check the parameter type
	ShaderParameterType type = parameter->GetParameterType();
/*
	SLtype cgType = cgGetParameterType(cgParameter);
	if (GetParameterType(cgType) != type.Type)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.SetParameter"),
			_T("Invalid parameter type."));
		return false;
	}

	SLparameterclass cgClass = cgGetParameterClass(cgParameter);
	if (GetParameterClass(cgClass) != type.Class)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.SetParameter"),
			_T("Invalid parameter class."));
		return false;
	}
*/
	void* data = NULL;
	parameter->GetRawValue(&data);

	glUseProgramObjectARB(_GLProgram);

	// Set the parameter value
	if (type._Type == ShaderVariableType_Float && type._Class == ShaderVariableClass_Scalar)
		glUniform1fvARB(glParameter, 1, (float*)data);
	else if (type._Type == ShaderVariableType_Float && type._Class == ShaderVariableClass_Vector)
		glUniform4fvARB(glParameter, 1, (float*)data);
	else if (type._Type == ShaderVariableType_Float && type._Class == ShaderVariableClass_Matrix_Rows)
		glUniformMatrix4fvARB(glParameter, 1, GL_TRUE, (float*)data);

	glUseProgramObjectARB(NULL);

	return true;
}

bool GLSLShaderProgram::Compile()
{
	Destroy();

	_IsCompiled = false;
	_ErrorMessages = String::Empty;

	_EntryPoint = _T("main");

	// Program object
	if (_GLProgram == NULL) //fix
		_GLProgram = glCreateProgramObjectARB();

	// Shader object
	if (GetProgramType() == ShaderProgramType_Vertex)
		_GLShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	else if (GetProgramType() == ShaderProgramType_Pixel)
		_GLShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// Shader source
	const SEchar* source = _SourceData.Data();
	glShaderSourceARB(_GLShader, 1, (const GLcharARB**)&source, NULL);

	if (glGetError() != GL_NO_ERROR)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.Compile (glProgramStringARB)"),
			_T("Failed to create the shader program."));
	    return false;
	}

	GLint compiled = 0;
	GLint linked = 0;
	GLint maxLength, length;
	GLcharARB* pInfoLog;

	// Compile the shader
	glCompileShaderARB(_GLShader);

	glGetObjectParameterivARB(_GLShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	glGetObjectParameterivARB(_GLShader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);

	pInfoLog = NULL;
	if (maxLength > 1)
	{
		pInfoLog = new GLcharARB[maxLength];
		glGetInfoLogARB(_GLShader, maxLength, &length, pInfoLog);
	}

	if (!compiled)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.Compile (glCompileShaderARB)"),
			_T("Failed to compile the shader program."));

		if (pInfoLog != NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.Compile"), String(pInfoLog));
		}
	    return false;
	}

	if (maxLength > 1)
	{
		_ErrorMessages = pInfoLog;
		delete[] pInfoLog;
	}

	glAttachObjectARB(_GLProgram, _GLShader);
	//glDeleteObjectARB(_GLShader);

	// Link the shader
	glLinkProgramARB(_GLProgram);

	glGetObjectParameterivARB(_GLProgram, GL_OBJECT_LINK_STATUS_ARB, &linked);
	glGetObjectParameterivARB(_GLProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);

	pInfoLog = NULL;
	if (maxLength > 1)
	{
		pInfoLog = new GLcharARB[maxLength];
		glGetInfoLogARB(_GLProgram, maxLength, &length, pInfoLog);
	}

	if (!linked)
	{
		Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.Compile (glLinkProgramARB)"),
			_T("Failed to link the shader program."));

		if (pInfoLog != NULL)
		{
			Logger::Current()->Log(LogLevel::Error, _T("GLSLShaderProgram.Compile"), String(pInfoLog));
		}
	    return false;
	}

	if (maxLength > 1)
	{
		String::Concat(_ErrorMessages, pInfoLog);
		delete[] pInfoLog;
	}

	_IsCompiled = true;

	return true;
}

bool GLSLShaderProgram::Create()
{
	return true;
}

void GLSLShaderProgram::Destroy()
{
	if (_GLProgram != NULL)
		glDeleteObjectARB(_GLProgram);

	if (_GLShader != NULL)
		glDeleteObjectARB(_GLShader);
}

GLSLVertexShaderProgram::GLSLVertexShaderProgram(GLSLShaderSystem* shaderSystem) :
	GLSLShaderProgram(shaderSystem)
{
}

GLSLVertexShaderProgram::~GLSLVertexShaderProgram()
{
	Destroy();
}

bool GLSLVertexShaderProgram::Bind()
{
	if (_GLProgram == NULL)
		return false;

	glUseProgramObjectARB(_GLProgram);

	return true;
}

bool GLSLVertexShaderProgram::Unbind()
{
	if (_GLProgram == NULL)
		return false;

	glUseProgramObjectARB(NULL);

	return true;
}

GLSLPixelShaderProgram::GLSLPixelShaderProgram(GLSLShaderSystem* shaderSystem) :
	GLSLShaderProgram(shaderSystem)
{
}

GLSLPixelShaderProgram::~GLSLPixelShaderProgram()
{
	Destroy();
}

bool GLSLPixelShaderProgram::Bind()
{
	if (_GLProgram == NULL)
		return false;

	glUseProgramObjectARB(_GLProgram);

	return true;
}

bool GLSLPixelShaderProgram::Unbind()
{
	if (_GLProgram == NULL)
		return false;

	glUseProgramObjectARB(NULL);

	return true;
}

}
