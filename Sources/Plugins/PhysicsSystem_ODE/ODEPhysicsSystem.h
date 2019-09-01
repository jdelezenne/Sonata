/*=============================================================================
ODEPhysicsSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ODEPHYSICSSYSTEM_H_
#define _SE_ODEPHYSICSSYSTEM_H_

#include <ode/ode.h>

#include <Core/Core.h>
#include <Physics/Physics.h>

using namespace SonataEngine;
using namespace SonataEngine::Physics;

namespace SE_ODE
{

class ODEHelper
{
public:
	static Vector3 MakeVector3(const dVector3 value)
	{
		return Vector3(value[0], value[1], value[2]);
	}

	static dReal* MakeVector3(const Vector3& value)
	{
		static dVector3 v;
		v[0] = value.X;
		v[1] = value.Y;
		v[2] = value.Z;
		return v;
	}

	static Matrix3 MakeMatrix3(const dMatrix3 value)
	{
		return Matrix3(
			value[0*3+0], value[0*3+1], value[0*3+2],
			value[1*3+0], value[1*3+1], value[1*3+2],
			value[2*3+0], value[2*3+1], value[2*3+2]);
	}

	static dReal* MakeMatrix3(const Matrix3& value)
	{
		static dMatrix3 m;
		return m;
	}

	static Quaternion MakeQuaternion(const dQuaternion value)
	{
		return Quaternion(value[0], value[1], value[2], value[3]);
	}

	static dReal* MakeQuaternion(const Quaternion& value)
	{
		static dQuaternion q;
		q[0] = value.X;
		q[1] = value.Y;
		q[2] = value.Z;
		q[3] = value.W;
		return q;
	}
};

/**  Open Dynamics Engine (ODE) Physics System. */
class ODEPhysicsSystem : public PhysicsSystem
{
public:
	ODEPhysicsSystem();
	virtual ~ODEPhysicsSystem();

	virtual PhysicsSystemDescription GetDescription() const;

	virtual bool Create(const PhysicsSystemDescription& desc);

	virtual void Destroy();

	virtual void Update(real64 elapsed);

	virtual IScene* CreateScene(const SceneDescription& desc);
	virtual void DestroyScene(IScene* scene);

protected:
	PhysicsSystemDescription _Description;

	Array<ScenePtr> _Scenes;
};

}

#endif 
