/*=============================================================================
NXPhysicsScene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_NXPHYSICSSCENE_H_
#define _SE_NXPHYSICSSCENE_H_

#include "NXPhysicsSystem.h"

namespace SE_NX
{

/** PhysX Scene. */
class NXPhysicsScene : public Physics::IScene
{
public:
	NXPhysicsScene(NXPhysicsSystem* physicsSystem, const SceneDescription& desc);
	virtual ~NXPhysicsScene();

	virtual SceneDescription GetDescription() const;

	virtual Vector3 GetGravity() const;

	virtual void SetGravity(const Vector3& value);

	virtual TimeStepType GetTimeStepType() const;
	virtual void SetTimeStepType(TimeStepType value);

	virtual real GetTimeStep() const;
	virtual void SetTimeStep(real value);

	virtual uint32 GetIterations() const;
	virtual void SetIterations(uint32 value);

	virtual IBody* CreateBody(const BodyDescription& desc);
	virtual void DestroyBody(IBody* body);

	virtual int GetBodyCount() const;
	virtual IBody* GetBody(int index) const;

	virtual IMaterial* CreateMaterial();
	virtual void DestroyMaterial(MaterialIndex material);

	virtual IMaterial* GetMaterial(MaterialIndex index) const;

	virtual MaterialIndex GetDefaultMaterial() const;

	virtual void SetDefaultMaterial(MaterialIndex value);

	virtual IJoint* CreateJoint();
	virtual void DestroyJoint(IJoint* joint);

	virtual void Update(real64 elapsed);

	NXPhysicsSystem* GetPhysicsSystem() const { return _PhysicsSystem; }
	NxScene* GetNxScene() const { return _NxScene; }

protected:
	NXPhysicsSystem* _PhysicsSystem;
	SceneDescription _Description;
	NxScene* _NxScene;

	Array<BodyPtr> _Bodies;
	Dictionary<MaterialIndex, MaterialPtr> _Materials;
	MaterialIndex _DefaultMaterial;
	Array<JointPtr> _Joints;
};

}

#endif 
