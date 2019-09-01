/*=============================================================================
Light.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LIGHT_H_
#define _SE_LIGHT_H_

#include "Graphics/Common.h"
#include "Graphics/Scene/SceneObject.h"

namespace SonataEngine
{

/** Light Type. */
enum LightType
{
	/// Directional Light.
	LightType_Directional,

	/// Point Light.
	LightType_Point,

	/// Spot Light.
	LightType_Spot
};

SE_BEGIN_ENUM(LightType);
	SE_Enum(Directional);
	SE_Enum(Point);
	SE_Enum(Spot);
SE_END_ENUM(LightType);

/**
	@brief Base class for the lights.
*/
class SE_GRAPHICS_EXPORT Light : public SceneObject
{
	SE_DECLARE_ABSTRACT(Light, SceneObject);
	SE_BEGIN_REFLECTION(Light);
		SE_Field(_isEnabled, bool, Public);
		SE_Field(_ambientColor, Color32, Public);
		SE_Field(_diffuseColor, Color32, Public);
		SE_Field(_specularColor, Color32, Public);
		SE_Field(_intensity, real32, Public);
		SE_Field(_isShadowCaster, bool, Public);
	SE_END_REFLECTION(Light);

protected:
	bool _isEnabled;
	Array<String> _lightGroups;
	Color32 _ambientColor;
	Color32 _diffuseColor;
	Color32 _specularColor;
	real32 _intensity;
	bool _isShadowCaster;

public:
	/** @name Constructors / Destructor. */
	//@{
	Light();
	virtual ~Light();
	//@}

	/** @name Properties. */
	//@{
	/** Gets a value indicating the type of the light. */
	virtual LightType GetLightType() const = 0;

	/** Gets or sets a value indicating whether the light is enabled. */
	bool IsEnabled() const { return _isEnabled; }
	void SetEnabled(bool value) { _isEnabled = value; }

	/** Gets the groups that this light is part of. */
	const Array<String>& GetLightGroups() const { return _lightGroups; }

	/** Adds the specified group to the groups that this light is part of. */
	void AddLightGroup(const String& value);

	/** Removes the specified group from the groups that this light is part of. */
	void RemoveLightGroup(const String& value);

	/** Gets or sets the ambient color of the light. */
	const Color32& GetAmbientColor() const { return _ambientColor; }
	void SetAmbientColor(const Color32& value) { _ambientColor = value; }

	/** Gets or sets the diffuse color of the light. */
	const Color32& GetDiffuseColor() const { return _diffuseColor; }
	void SetDiffuseColor(const Color32& value) { _diffuseColor = value; }

	/** Gets or sets the specular color of the light. */
	const Color32& GetSpecularColor() const { return _specularColor; }
	void SetSpecularColor(const Color32& value) { _specularColor = value; }

	/** Gets or sets the intensity of the light. */
	real32 GetIntensity() const { return _intensity; }
	void SetIntensity(real32 value) { _intensity = value; }

	/** Gets or sets a value indicating whether the light is casting shadows. */
	bool IsShadowCaster() const { return _isShadowCaster; }
	void SetShadowCaster(bool value) { _isShadowCaster = value; }
	//@}
};

SEPointer(Light);

}

#endif
