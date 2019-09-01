/*=============================================================================
NXPhysicsSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NXPHYSICSSYSTEM_H_
#define _SE_NXPHYSICSSYSTEM_H_

#include <NxPhysics.h>
#include <NxCooking.h>

#include <Core/Core.h>
#include <Physics/Physics.h>

using namespace SonataEngine;
using namespace SonataEngine::Physics;

namespace SE_NX
{

class NXHelper
{
public:
	static Vector3 MakeVector3(const NxVec3& value)
	{
		return Vector3(value.x, value.y, value.z);
	}

	static NxVec3 MakeVector3(const Vector3& value)
	{
		return NxVec3(value.X, value.Y, value.Z);
	}

	static Matrix3 MakeMatrix33(const NxMat33& value)
	{
		return Matrix3(
			value(0,0), value(0,1), value(0,2),
			value(1,0), value(1,1), value(1,2),
			value(2,0), value(2,1), value(2,2));
	}

	static NxMat33 MakeMatrix33(const Matrix3& value)
	{
		return NxMat33(
			NxVec3(value(0,0), value(0,1), value(0,2)),
			NxVec3(value(1,0), value(1,1), value(1,2)),
			NxVec3(value(2,0), value(2,1), value(2,2)));
	}

	static Quaternion MakeQuaternion(const NxQuat& value)
	{
		return Quaternion(value.x, value.y, value.z, value.w);
	}

	static NxQuat MakeQuaternion(const Quaternion& value)
	{
		return NxQuat(NxVec3(value.X, value.Y, value.Z), value.W);
	}
};

class MyAllocator : public NxUserAllocator
{
public:
	MyAllocator();
	~MyAllocator();

	void* malloc(NxU32 size);
	void* mallocDEBUG(NxU32 size, const char* fileName, int line);
	void* mallocDEBUG(NxU32 size, const char* fileName, int line, const char* className, NxMemoryType type);
	void* realloc(void* memory, NxU32 size);
	void free(void* memory);
};

/** Ageia PhysX Physics System (NovodeX). */
class NXPhysicsSystem : public PhysicsSystem
{
public:
	NXPhysicsSystem();
	virtual ~NXPhysicsSystem();

	virtual PhysicsSystemDescription GetDescription() const;

	virtual bool Create(const PhysicsSystemDescription& desc);

	virtual void Destroy();

	virtual void Update(real64 elapsed);

	virtual IScene* CreateScene(const SceneDescription& desc);
	virtual void DestroyScene(IScene* scene);

	NxPhysicsSDK* GetNxSDK() const { return _NxPhysicsSDK; }

protected:
	PhysicsSystemDescription _Description;
	NxPhysicsSDK* _NxPhysicsSDK;
	MyAllocator _myAllocator;

	Array<ScenePtr> _Scenes;
};

}

#endif 
