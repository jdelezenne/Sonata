/*=============================================================================
D3D8CgShaderProgram.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D8CgShaderProgram.h"

namespace SE_D3D8Cg
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

D3D8CgShaderProgram::D3D8CgShaderProgram(D3D8CgShaderSystem* shaderSystem) :
	ShaderProgram(),
	_shaderSystem(shaderSystem),
	_CGprogram(NULL),
	_IsCompiled(false)
{
}

D3D8CgShaderProgram::~D3D8CgShaderProgram()
{
	Destroy();
}

int D3D8CgShaderProgram::GetParameterCount() const
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

CGparameter D3D8CgShaderProgram::GetCgParameterByIndex(int index) const
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

ShaderParameter* D3D8CgShaderProgram::GetParameterByIndex(int index) const
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

ShaderParameter* D3D8CgShaderProgram::GetParameterByName(const String& name) const
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

bool D3D8CgShaderProgram::SetParameter(int index, ShaderParameter* parameter)
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

bool D3D8CgShaderProgram::SetParameter(const String& name, ShaderParameter* parameter)
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

bool D3D8CgShaderProgram::SetCgParameter(CGparameter cgParameter, ShaderParameter* parameter)
{
	HRESULT hr;

	// Check the parameter type
	ShaderParameterType type = parameter->GetParameterType();
/*
	CGtype cgType = cgGetParameterType(cgParameter);
	if (GetParameterType(cgType) != type.Type)
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8CgShaderProgram.SetParameter"),
			_T("Invalid parameter type."));
		return false;
	}

	CGparameterclass cgClass = cgGetParameterClass(cgParameter);
	if (GetParameterClass(cgClass) != type.Class)
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8CgShaderProgram.SetParameter"),
			_T("Invalid parameter class."));
		return false;
	}
*/
	void* data = NULL;
	parameter->GetRawValue(&data);

	// Set the parameter value
	hr = cgD3D8SetUniform(cgParameter, data);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8CgShaderProgram.SetParameter"),
			_T("Failed to set the value of the parameter."));
		return false;
	}

	return true;
}

bool D3D8CgShaderProgram::Compile()
{
	HRESULT hr;

	Destroy();

	_IsCompiled = false;

	if (_EntryPoint.IsEmpty())
	{
		_EntryPoint = _T("main");
	}

	CGprofile cgProfile;

	// if no profile is specified, get the highest  profile supported
	if (!_Profile.IsEmpty())
	{
		cgProfile = cgGetProfile(_Profile.Data());
	}
	else
	{
		if (GetProgramType() == ShaderProgramType_Vertex)
			cgProfile = _shaderSystem->GetCGVertexProfile();
		else if (GetProgramType() == ShaderProgramType_Pixel)
			cgProfile = _shaderSystem->GetCGPixelProfile();

		_Profile = cgGetProfileString(cgProfile);
	}

	const char* profileOpts[] =
	{
		cgD3D8GetOptimalOptions(cgProfile),
		NULL
	};

	_CGprogram = cgCreateProgram(_shaderSystem->GetCGContext(), CG_SOURCE,
		_SourceData.Data(), cgProfile, _EntryPoint.Data(), NULL);

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

		Logger::Current()->Log(LogLevel::Error, _T("D3D8CgShaderProgram.Compile (cgCreateProgram)"),
			_T("Failed to create the shader program: ") + _ErrorMessages);
		return false;
	}

	// Assemble the shader
	DWORD vshaderUsage = 0; // hardware
	//TODO
	DWORD* vertexDecl = NULL;
	hr = cgD3D8LoadProgram(_CGprogram, TRUE, 0, vshaderUsage, vertexDecl);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("D3D8CgShaderProgram.Compile (cgD3D8LoadProgram)"),
			_T("Failed to assemble the shader program."));
		return false;
	}

	_IsCompiled = true;

	return true;
}

bool D3D8CgShaderProgram::Create()
{
	return true;
}

void D3D8CgShaderProgram::Destroy()
{
	if (_CGprogram != NULL)
		cgDestroyProgram(_CGprogram);
}

D3D8CgVertexShaderProgram::D3D8CgVertexShaderProgram(D3D8CgShaderSystem* shaderSystem) :
	D3D8CgShaderProgram(shaderSystem)
{
}

D3D8CgVertexShaderProgram::~D3D8CgVertexShaderProgram()
{
	Destroy();
}

bool D3D8CgVertexShaderProgram::Bind()
{
	if (_CGprogram == NULL)
		return false;

	cgD3D8BindProgram(_CGprogram);

	return true;
}

bool D3D8CgVertexShaderProgram::Unbind()
{
	if (_CGprogram == NULL)
		return false;

	_shaderSystem->GetD3DDevice()->SetVertexShader(NULL);

	return true;
}

D3D8CgPixelShaderProgram::D3D8CgPixelShaderProgram(D3D8CgShaderSystem* shaderSystem) :
	D3D8CgShaderProgram(shaderSystem)
{
}

D3D8CgPixelShaderProgram::~D3D8CgPixelShaderProgram()
{
	Destroy();
}

bool D3D8CgPixelShaderProgram::Bind()
{
	if (_CGprogram == NULL)
		return false;

	cgD3D8BindProgram(_CGprogram);

	return true;
}

bool D3D8CgPixelShaderProgram::Unbind()
{
	if (_CGprogram == NULL)
		return false;

	_shaderSystem->GetD3DDevice()->SetPixelShader(NULL);

	return true;
}

}
