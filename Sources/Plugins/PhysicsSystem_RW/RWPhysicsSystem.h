/*=============================================================================
RWPhysicsSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RWPHYSICSSYSTEM_H_
#define _SE_RWPHYSICSSYSTEM_H_

#include <rwpcore.h>
#include <rwpbody.h>
#include <rwpgeneric.h>
#include <rwpragdoll.h>

#include <Core/Core.h>
#include <Physics/Physics.h>

using namespace SonataEngine;
using namespace SonataEngine::Physics;

namespace SE_RW
{

#if (defined(SKY))
#define rsRESOURCESDEFAULTARENASIZE (8 << 20)
#elif (defined(_XBOX))
#define rsRESOURCESDEFAULTARENASIZE (8 << 20)
#elif (defined(DOLPHIN))
#define rsRESOURCESDEFAULTARENASIZE (4 << 20)
#elif (defined(D3D8_DRVMODEL_H))
#define rsRESOURCESDEFAULTARENASIZE (4 << 20)
#elif (defined(D3D9_DRVMODEL_H))
#define rsRESOURCESDEFAULTARENASIZE (4 << 20)
#elif (defined(OPENGL_DRVMODEL_H))
#if (defined(macintosh))
#define rsRESOURCESDEFAULTARENASIZE (2 << 20)
#else
#define rsRESOURCESDEFAULTARENASIZE (16 << 20)
#endif
#else
#define rsRESOURCESDEFAULTARENASIZE (16 << 20)
#endif

class RWHelper
{
public:
	static Vector3 MakeVector3(RwV3d* value)
	{
		return Vector3(value->x, value->y, value->z);
	}

	static void MakeVector3(RwV3d* out, const Vector3& value)
	{
		RwV3dSet(out, value.X, value.Y, value.Z);
	}

	static Quaternion MakeQuaternion(RtQuat* value)
	{
		return Quaternion(value->imag.x, value->imag.y, value->imag.z, value->real);
	}

	static void MakeQuaternion(RtQuat* out, const Quaternion& value)
	{
		RwV3dSet(&out->imag, value.X, value.Y, value.Z);
		out->real = value.W;
	}
};

/** RenderWare Physics System. */
class RWPhysicsSystem : public PhysicsSystem
{
public:
	RWPhysicsSystem();
	virtual ~RWPhysicsSystem();

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
