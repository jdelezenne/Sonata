/*=============================================================================
RTScene.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_SCENE_H_
#define _RAYTRACER_SCENE_H_

#include "Common.h"
#include "Camera.h"
#include "SceneObject.h"
#include "Light.h"

namespace Raytracer
{
	class RTScene : public RefObject
	{
		SE_DECLARE_CLASS(RTScene, Object);

		SE_BEGIN_REFLECTION(RTScene);
			SE_Field(_BackgroundColour, Colour32, Public);
			SE_Field(_Camera, RTCamera, Public);
		SE_END_REFLECTION(RTScene);

	public:
		//@{
		RTScene();
		virtual ~RTScene();
		//@}

		//@{
		Colour32 GetBackgroundColour() const { return _BackgroundColour; }
		void SetBackgroundColour(Colour32 value) { _BackgroundColour = value; }

		real32 GetGamma() const { return _Gamma; }
		void SetGamma(real32 value) { _Gamma = value; }

		RTCamera* GetCamera() const { return _Camera; }
		void SetCamera(RTCamera* value) { _Camera = value; }

		List<RTLight*>& Lights() { return _Lights; }

		List<RTSceneObject*>& Objects() { return _Objects; }
		//@}

		//@{
		virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
		virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

		void Update();

		void Raytrace(RenderState& state, Colour32& colour);
		void TraceShadow(RenderState& state, RTLight* light, real32& shadow);
		//@}

	protected:
		Colour32 _BackgroundColour;
		real32 _Gamma;
		RTCamera* _Camera;
		List<RTLight*> _Lights;
		List<RTSceneObject*> _Objects;
	};
}

#endif 
