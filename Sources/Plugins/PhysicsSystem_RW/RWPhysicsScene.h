/*=============================================================================
RWPhysicsScene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RWPHYSICSSCENE_H_
#define _SE_RWPHYSICSSCENE_H_

#include "RWPhysicsSystem.h"

namespace SE_RW
{

/** RW Scene. */
class RWPhysicsScene : public Physics::IScene
{
public:
	RWPhysicsScene(RWPhysicsSystem* physicsSystem, const SceneDescription& desc);
	virtual ~RWPhysicsScene();

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

	RWPhysicsSystem* GetPhysicsSystem() const { return _PhysicsSystem; }
	RwpContext* GetRwpContext() const { return _RwpContext; }

protected:
	RWPhysicsSystem* _PhysicsSystem;
	SceneDescription _Description;
	RwpContext* _RwpContext;

	Array<BodyPtr> _Bodies;
	Dictionary<MaterialIndex, MaterialPtr> _Materials;
	MaterialIndex _DefaultMaterial;
	Array<JointPtr> _Joints;
};

}

#endif 
