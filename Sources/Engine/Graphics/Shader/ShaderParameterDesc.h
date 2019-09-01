/*=============================================================================
ShaderParameterDesc.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHADERPARAMETERDESC_H_
#define _SE_SHADERPARAMETERDESC_H_	

#include "Core/Object.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/** Shader Parameter Type. */
enum ShaderParameterType
{
	ShaderParameterType_Void,
	ShaderParameterType_Boolean,
	ShaderParameterType_Integer,
	ShaderParameterType_Float,
	ShaderParameterType_String,
	ShaderParameterType_Texture,
	ShaderParameterType_Texture1D,
	ShaderParameterType_Texture2D,
	ShaderParameterType_Texture3D,
	ShaderParameterType_TextureCube,
	ShaderParameterType_Sampler,
	ShaderParameterType_Sampler1D,
	ShaderParameterType_Sampler2D,
	ShaderParameterType_Sampler3D,
	ShaderParameterType_SamplerCube,
	ShaderParameterType_PixelShader,
	ShaderParameterType_VertexShader,
	ShaderParameterType_GeometryShader
};

/** Shader Parameter Class. */
enum ShaderParameterClass
{
	ShaderParameterClass_Scalar,
	ShaderParameterClass_Vector,
	ShaderParameterClass_Matrix_Rows,
	ShaderParameterClass_Matrix_Columns,
	ShaderParameterClass_Object,
	ShaderParameterClass_Struct
};

SE_BEGIN_ENUM(ShaderParameterType);
	SE_Enum(Uniform);
	SE_Enum(Void);
	SE_Enum(Boolean);
	SE_Enum(Integer);
	SE_Enum(Float);
	SE_Enum(String);
	SE_Enum(Texture);
	SE_Enum(Texture1D);
	SE_Enum(Texture2D);
	SE_Enum(Texture3D);
	SE_Enum(TextureCube);
	SE_Enum(Sampler);
	SE_Enum(Sampler1D);
	SE_Enum(Sampler2D);
	SE_Enum(Sampler3D);
	SE_Enum(SamplerCube);
	SE_Enum(PixelShader);
	SE_Enum(VertexShader);
	SE_Enum(GeometryShader);
SE_END_ENUM(ShaderParameterType);

SE_BEGIN_ENUM(ShaderParameterClass);
	SE_Enum(Scalar);
	SE_Enum(Vector);
	SE_Enum(Matrix_Rows);
	SE_Enum(Matrix_Columns);
	SE_Enum(Object);
	SE_Enum(Struct);
SE_END_ENUM(ShaderParameterClass);

/**
	@brief Represents the type of a shader parameter.
*/
class SE_GRAPHICS_EXPORT ShaderParameterDesc : public Object
{
	SE_DECLARE_CLASS(ShaderParameterDesc, Object);
	SE_BEGIN_REFLECTION(ShaderParameterDesc);
		SE_Field(Bytes, int32, Public);
		SE_Field(StructMembers, int32, Public);
		SE_Field(Elements, int32, Public);
		SE_Field(Columns, int32, Public);
		SE_Field(Rows, int32, Public);
		SE_Field(ParameterType, ShaderParameterType, Public);
		SE_Field(ParameterClass, ShaderParameterClass, Public);
		SE_Field(Semantic, String, Public);
	SE_END_REFLECTION(ShaderParameterDesc);

public:
	/** @name Properties. */
	//@{
	/** Contains the size of the parameter in bytes. */
	int32 Bytes;

	/** Retrieves the number of structure members. */
	int32 StructMembers;

	/** Retrieves the number of elements in the array. */
	int32 Elements;

	/** Retrieves the number of columns in the array. */
	int32 Columns;

	/** Retrieves the number of rows in the array. */
	int32 Rows;

	/** Retrieves the parameter type, which is one of the values in the ShaderParameterType enumeration. */
	ShaderParameterType ParameterType;  

	/** Retrieves the parameter class, which is one of the values in the ShaderParameterClass enumeration. */
	ShaderParameterClass ParameterClass;

	/** Retrieves the semantic meaning, also called the usage. */
	String Semantic;
	//@}

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	ShaderParameterDesc();

	/** Destructor. */
	virtual ~ShaderParameterDesc();
	//@}
};

}

#endif
