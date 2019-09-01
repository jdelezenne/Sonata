/*=============================================================================
RTSceneObject.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_SCENEOBJECT_H_
#define _RAYTRACER_SCENEOBJECT_H_

#include "Common.h"

namespace Raytracer
{
	class RTShader;

	class TraceResult
	{
	public:
		TraceResult();

	public:
		bool _Hit;
		real32 _Distance;
	};


	class RTSceneObject : public RefObject
	{
		SE_DECLARE_ABSTRACT(RTSceneObject, Object);

		SE_BEGIN_REFLECTION(RTSceneObject);
			SE_Field(_Name, String, Public);
			//SE_Field(_Shader, RTShader, Public);
		SE_END_REFLECTION(RTSceneObject);

	public:
		//@{
		RTSceneObject();
		virtual ~RTSceneObject();
		//@}

		//@{
		String GetName() const { return _Name; }
		void SetName(const String& value) { _Name = value; }

		Transform& GetTransform() { return _Transform; }

		RTShader* GetShader() const { return _Shader; }
		void SetShader(RTShader* value) { _Shader = value; }
		//@}

		//@{
		virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
		virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

		virtual AABB GetAABB() = 0;
		virtual void Intersect(RenderState& state, const Ray3& ray, TraceResult& result) = 0;
		virtual Vector3 GetNormal(const Vector3& p) = 0;
		virtual Vector2 GetUV(const Vector3& p) = 0;
		//@}

	protected:
		String _Name;
		Transform _Transform;
		RTShader* _Shader;
	};
}

#endif 
