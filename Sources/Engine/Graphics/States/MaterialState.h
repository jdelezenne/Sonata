/*=============================================================================
MaterialState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MATERIALSTATE_H_
#define _SE_MATERIALSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Material state.
*/
class SE_GRAPHICS_EXPORT MaterialState
{
public:
	/** @name Material. */
	//@{
	/// Retrieves or sets a value that specifies the ambient color.
	Color32 AmbientColor;

	/// Retrieves or sets a value that specifies the diffuse color.
	Color32 DiffuseColor;

	/// Retrieves or sets a value that specifies the specular color.
	Color32 SpecularColor;

	/// Retrieves or sets a value that specifies the emissive color.
	Color32 EmissiveColor;

	/// Retrieves or sets a value that specifies the sharpness of specular highlights.
	real32 Shininess;

	/// Vertex color tracking.
	bool VertexColor;
	//@}

public:
	/** @name Constructor. */
	//@{
	MaterialState();
	MaterialState(const Color32& ambientColor, const Color32& diffuseColor, const Color32& specularColor, const Color32& emissiveColor, real32 shininess);
	//@}
};

SE_DECLARE_STRUCT(MaterialState);
SE_BEGIN_STRUCT(MaterialState);
	SE_StructField(AmbientColor, Color32);
	SE_StructField(DiffuseColor, Color32);
	SE_StructField(SpecularColor, Color32);
	SE_StructField(EmissiveColor, Color32);
	SE_StructField(Shininess, real32);
	SE_StructField(VertexColor, bool);
SE_END_STRUCT(MaterialState);

}

#endif
