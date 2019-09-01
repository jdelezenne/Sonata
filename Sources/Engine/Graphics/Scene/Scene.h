/*=============================================================================
Scene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENE_H_
#define _SE_SCENE_H_

#include "Core/Object.h"
#include "Graphics/Lighting/Light.h"
#include "Graphics/Scene/SceneNode.h"
#include "Graphics/Scene/Sky.h"
#include "Graphics/States/ScissorState.h"
#include "Graphics/States/FogState.h"
#include "Graphics/Animation/AnimationSet.h"

namespace SonataEngine
{

struct ShadowPlane
{
	ShadowPlane() :
		Enabled(true),
		Color(Color32::Gray),
		Normal(Vector3::UnitY),
		Distance(0.0f)
	{
	}

	bool Enabled;
	Color32 Color;
	Vector3 Normal;
	real32 Distance;
};

class SE_GRAPHICS_EXPORT Scene : public SceneObject
{
	SE_DECLARE_CLASS(Scene, SceneObject);
	SE_BEGIN_REFLECTION(Scene);
		SE_Field(_root, SceneNode, Public);
		SE_Field(_ambientColor, Color32, Public);
		SE_Field(_scissorState, ScissorState, Public);
		SE_Field(_fogState, FogState, Public);
	SE_END_REFLECTION(Scene);

public:
	typedef Array<SceneObject*> SceneObjectList;

protected:
	SceneNodePtr _root;
	ScissorState _scissorState;
	Color32 _ambientColor;
	FogState _fogState;
	SkyPtr _sky;
	bool _enableShadowPlanes;
	real32 _shadowPlanesIntensity;
	BaseArray<ShadowPlane> _shadowPlanes;
	bool _enableShadows;

	SceneObjectList _sceneObjects;
	AnimationSet* _animationSet;

public:
	/** @name Constructors / Destructor. */
	//@{
	Scene();
	virtual ~Scene();
	//@}

	/** @name Properties. */
	//@{
	SceneNode* GetRoot() const { return _root; }
	void SetRoot(SceneNode* root) { _root = root; }

	ScissorState GetScissorState() const { return _scissorState; }
	void SetScissorState(ScissorState value) { _scissorState = value; }

	Color32 GetAmbientColor() const { return _ambientColor; }
	void SetAmbientColor(Color32 value) { _ambientColor = value; }

	FogState GetFogState() const { return _fogState; }
	void SetFogState(FogState value) { _fogState = value; }

	Sky* GetSky() const { return _sky; }
	void SetSky(Sky* value) { _sky = value; }

	bool GetEnableShadowPlanes() const { return _enableShadowPlanes; }
	void SetEnableShadowPlanes(bool value) { _enableShadowPlanes = value; }

	real32 GetShadowPlanesIntensity() const { return _shadowPlanesIntensity; }
	void SetShadowPlanesIntensity(real32 value) { _shadowPlanesIntensity = value; }

	BaseArray<ShadowPlane>& GetShadowPlanes() { return _shadowPlanes; }

	bool GetEnableShadows() const { return _enableShadows; }
	void SetEnableShadows(bool value) { _enableShadows = value; }
	//@}

	/** @name Scene objects. */
	//@{
	void AddObject(SceneObject* obj);
	SceneObject* FindObject(const String& name);
	SceneObject* FindObjectOfType(const String& name, const TypeInfo* type);
	void RemoveObject(SceneObject* obj);
	SceneObjectList::Iterator GetSceneObjectIterator() const;
	//@}

	/** @name Animations. */
	//@{
	/** Gets the animation set. */
	AnimationSet* GetAnimationSet() const { return _animationSet; }

	/** Sets the animation set. */
	void SetAnimationSet(AnimationSet* value) { _animationSet = value; }
	//@}

	virtual void Initialize();
	virtual void Update(const TimeValue& timeValue);
	virtual void Render();
};

SEPointer(Scene);

}

#endif
