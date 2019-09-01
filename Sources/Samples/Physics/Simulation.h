/*=============================================================================
Simulation.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "Common.h"

Color32 GetRandomColor();
ShaderMaterial* CreateDefaultShader();

class PhysicsWorld;

SE_BEGIN_ENUM(ShapeType);
	SE_Enum(Plane);
	SE_Enum(Sphere);
	SE_Enum(Box);
	SE_Enum(Capsule);
	SE_Enum(Mesh);
	SE_Enum(Compound);
SE_END_ENUM(ShapeType);

SE_BEGIN_ENUM(BodyType);
	SE_Enum(Static);
	SE_Enum(Dynamic);
SE_END_ENUM(BodyType);

class PhysicsEntity : public RefObject
{
	SE_DECLARE_CLASS(PhysicsEntity, Object);

	SE_BEGIN_REFLECTION(PhysicsEntity);
		SE_Field(_name, String, Public);
		SE_Field(_Position, Vector3, Public);
		SE_Field(_Orientation, Quaternion, Public);
		SE_Field(_LinearVelocity, Vector3, Public);
		SE_Field(_AngularVelocity, Vector3, Public);
		SE_Field(_Color, Color32, Public);
		SE_Field(_ShapeType, ShapeType, Public);
		SE_Field(_BodyType, BodyType, Public);
		SE_Field(_Density, real32, Public);
		SE_Field(_Radius, real32, Public);
		SE_Field(_Extents, Vector3, Public);
	SE_END_REFLECTION(PhysicsEntity);

public:
	PhysicsEntity();
	virtual ~PhysicsEntity();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	PhysicsWorld* GetWorld() const { return _World; }
	void SetWorld(PhysicsWorld* value) { _World = value; }

	String GetName() const { return _name; }
	void SetName(String value);

	Vector3 GetPosition() const { return _Position; }
	void SetPosition(Vector3 value);

	Quaternion GetOrientation() const { return _Orientation; }
	void SetOrientation(Quaternion value);

	Vector3 GetLinearVelocity() const { return _LinearVelocity; }
	void SetLinearVelocity(Vector3 value);

	Vector3 GetAngularVelocity() const { return _AngularVelocity; }
	void SetAngularVelocity(Vector3 value);

	Color32 GetColor() const { return _Color; }
	void SetColor(Color32 value);

	void SetWireColor(Color32 value);

	Mesh* GetMesh() const { return _mesh; }
	void SetMesh(Mesh* value) { _mesh = value; }

	ModelNode* GetNode() const { return _Node; }
	void SetNode(ModelNode* value) { _Node = value; }

	Physics::IBody* GetPhysicsBody() const { return _PhysicsBody; }
	void SetPhysicsBody(Physics::IBody* value) { _PhysicsBody = value; }

	void Initialize(PhysicsWorld* world);

	void CreatePlane(const Plane& plane);
	void CreateSphere(const BoundingSphere& sphere, real32 density);
	void CreateBox(const OBB& box, real32 density);
	void CreateModel(Model* model, real32 density);

	void Create(Mesh* mesh, const Physics::BodyDescription& bodyDesc);

protected:
	PhysicsWorld* _World;
	String _name;
	Vector3 _Position;
	Quaternion _Orientation;
	Vector3 _LinearVelocity;
	Vector3 _AngularVelocity;
	Color32 _Color;

	ShapeType _ShapeType;
	BodyType _BodyType;
	real32 _Density;
	real32 _Radius;
	Vector3 _Extents;

	MeshPtr _mesh;
	ModelNode* _Node;
	Physics::BodyPtr _PhysicsBody;
};

enum CameraMode
{
	CameraMode_Free,
	CameraMode_FirstPerson,
	CameraMode_LookAt
};

class PhysicsWorld : public RefObject
{
	SE_DECLARE_CLASS(PhysicsWorld, Object);

	SE_BEGIN_REFLECTION(PhysicsWorld);
		SE_Field(_name, String, Public);
		SE_Field(_cameraPosition, Vector3, Public);
		SE_Field(_cameraOrientation, Quaternion, Public);
	SE_END_REFLECTION(PhysicsWorld);

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	Scene* GetScene() const { return _Scene; }

	Physics::IScene* GetPhysicsScene() const { return _PhysicsScene; }

	bool GetPaused() const { return _Paused; }
	void SetPaused(bool value) { _Paused = value; }

	IntegrationType GetIntegrationType() const { return _IntegrationType; }
	void SetIntegrationType(IntegrationType value) { _IntegrationType = value; }

	void GetThrownShape(Physics::ShapeType& type, real& density, BoundingSphere& sphere, OBB& box);
	void SetThrownShape(Physics::ShapeType type, real density, BoundingSphere sphere, OBB box);

	void AddEntity(PhysicsEntity* entity);
	void RemoveEntity(PhysicsEntity* entity);
	int GetEntityCount() const;
	PhysicsEntity* GetEntity(int index) const;
	void SetControlledEntity(PhysicsEntity* entity);

	PhysicsEntity* CreateEntity();

	void Create();
	void ApplyForce(Physics::IBody* body, const Vector3& force, bool torque);
	void UpdateCamera();
	void Update(real64 elapsed);
	void Render();

	bool OnPreContact(Object* sender, const Physics::ContactEventArgs* e);

protected:
	void CreateBoundsMesh();
	void CreateContactMesh();
	void CreateNormalMesh();

	void RenderBounds(Physics::IBody* body);
	void RenderContact(const ContactInfo& info);
	void RenderArrow(const Vector3& ptA, const Vector3& ptB, const Color32& color);

protected:
	CameraMode _cameraMode;
	CameraController* _camera;
	Scene* _Scene;
	SkyBox* _SkyBox;
	Physics::ScenePtr _PhysicsScene;
	bool _Paused;

	struct ContactTime
	{
		ContactInfo info;
		real64 t;
	};

	String _name;
	Vector3 _cameraPosition;
	Quaternion _cameraOrientation;

	Array<PhysicsEntity*> _Entities;
	PhysicsEntity* _ControlledEntity;
	BaseArray<ContactTime> _Contacts;
	MeshPtr _BoundsMesh;
	MeshPtr _ContactMesh;
	MeshPtr _NormalMesh;

	IntegrationType _IntegrationType;
	Vector3 _Force;
	real _ForceStrength;

	Physics::ShapeType _ThrownShapeType;
	real _ThrownShapeDensity;
	BoundingSphere _ThrownSphere;
	OBB _ThrownBox;
};

#endif 
