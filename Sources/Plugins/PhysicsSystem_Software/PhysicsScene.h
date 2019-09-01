/*=============================================================================
PhysicsScene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PHYSICSSCENE_H_
#define _SE_PHYSICSSCENE_H_

#include "SoftwarePhysicsSystem.h"

namespace SE_Software
{

class SpaceDivisionObject : public Object
{
public:
	SpaceDivisionObject();
	virtual ~SpaceDivisionObject();

	virtual AABB GetLocalBoundingBox() const = 0;
	virtual AABB GetWorldBoundingBox() const = 0;
};

class SpaceDivision : public Object
{
public:
	SpaceDivision();
	virtual ~SpaceDivision();

	virtual void AddObject(SpaceDivisionObject* object) = 0;
	virtual void RemoveObject(SpaceDivisionObject* object) = 0;
};

class OctreeSpace : public SpaceDivision
{
public:
	class Node
	{
	public:
		Node();

		Array<SpaceDivisionObject*> _Objects;
		Vector3 _Position;
		Node* _Parent;
		Node* _Children[8];
	};

	OctreeSpace();
	virtual ~OctreeSpace();

	virtual void AddObject(SpaceDivisionObject* object);
	virtual void RemoveObject(SpaceDivisionObject* object);

private:
	void InternalAddNode(Node* node);
	void InternalRemoveNode(Node* node);

protected:
	Node* _RootNode;
};

/** Scene. */
class PhysicsScene : public Physics::IScene
{
public:
	PhysicsScene(PhysicsSystem* physicsSystem, const SceneDescription& desc);
	virtual ~PhysicsScene();

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

	PhysicsSystem* GetPhysicsSystem() const { return _PhysicsSystem; }

protected:
	void GenerateContactPairs();
	void GenerateContactInfos();
	void ResolveCollisions(real64 elapsed);

protected:
	PhysicsSystem* _PhysicsSystem;
	SceneDescription _Description;

	Array<BodyPtr> _Bodies;
	Dictionary<MaterialIndex, MaterialPtr> _Materials;
	MaterialIndex _DefaultMaterial;
	Array<JointPtr> _Joints;

	Array<ContactPair> _ContactPairs;
	Array<ContactInfo> _ContactInfos;
};

}

#endif 
