/*=============================================================================
Simulation.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SamplePhysics.h"
#include "Simulation.h"
#include "Utils.h"

struct EntityIntersection
{
	PhysicsEntity* Entity;
	Vector3 Position;
};

Color32 GetRandomColor()
{
	return Color32(
		Math::Random(0.0f, 1.0f),
		Math::Random(0.0f, 1.0f),
		Math::Random(0.0f, 1.0f),
		1.0f);
}

ShaderMaterial* CreateDefaultShader()
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_Solid;
	pass0->MaterialState.DiffuseColor = GetRandomColor();
	pass0->LightState.Lighting = true;
	technique->AddPass(pass0);

	ShaderPass* pass1 = new ShaderPass();
	pass1->RasterizerState.FillMode = FillMode_WireFrame;
	pass1->RasterizerState.CullMode = CullMode_None;
	pass1->DepthState.SlopeScaledDepthBias = 1.0f;
	pass1->MaterialState.DiffuseColor = Color32::Black;
	pass1->LightState.Lighting = true;
	technique->AddPass(pass1);

	shader->AddTechnique(technique);

	return shader;
}

ShaderMaterial* CreatePlaneShader()
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_Solid;
	pass0->LightState.Lighting = false;

	SamplerState* sampler0 = new SamplerState();
	pass0->AddSamplerState(sampler0);
	TextureState* texture0 = new TextureState();
	texture0->SetTexture(CreateTexture(_T("Terrain\\grass.jpg")));
	texture0->SetTile(Vector2(10.0));
	pass0->AddTextureState(texture0);
	technique->AddPass(pass0);

	ShaderPass* pass1 = new ShaderPass();
	pass1->RasterizerState.FillMode = FillMode_WireFrame;
	pass1->RasterizerState.CullMode = CullMode_None;
	pass1->DepthState.SlopeScaledDepthBias = 1.0f;
	pass1->MaterialState.DiffuseColor = Color32::Black;
	pass1->LightState.Lighting = true;
	technique->AddPass(pass1);

	shader->AddTechnique(technique);

	return shader;
}

Ray3 ScreenToWorld(const Vector2& screen)
{
    Ray3 ray;

	Camera* camera = RenderSystem::Current()->GetCamera();
	Viewport viewport = RenderSystem::Current()->GetViewport();

	// Get the ray from the screen coordinates
	Matrix4 matProj = camera->GetProjection();

	// Compute the vector of the pick ray in screen space
	Vector3 v;
	v.X = (((2.0 * screen.X) / viewport.GetWidth()) - 1) / matProj.M[0][0];
	v.Y = -(((2.0 * screen.Y) / viewport.GetHeight()) - 1) / matProj.M[1][1];
	v.Z = 1.0;

	// Get the inverse view matrix
	Matrix4 matInvView = Matrix4::Inverse(camera->GetView());

	// Transform the screen space ray into world space
	ray.Direction = -Vector3(
		v.X*matInvView.M[0][0] + v.Y*matInvView.M[0][1] + v.Z*matInvView.M[0][2],
		v.X*matInvView.M[1][0] + v.Y*matInvView.M[1][1] + v.Z*matInvView.M[1][2],
		v.X*matInvView.M[2][0] + v.Y*matInvView.M[2][1] + v.Z*matInvView.M[2][2]);
	ray.Origin = Vector3(matInvView.M[0][3], matInvView.M[1][3], matInvView.M[2][3]);

	return ray;
}

SE_IMPLEMENT_CLASS(PhysicsEntity);
SE_IMPLEMENT_REFLECTION(PhysicsEntity);

PhysicsEntity::PhysicsEntity() :
	RefObject()
{
	_Position = Vector3::Zero;
	_Orientation = Quaternion::Identity;
	_LinearVelocity = Vector3::Zero;
	_AngularVelocity = Vector3::Zero;
	_Color = Color32::Black;

	_ShapeType = ShapeType_Sphere;
	_BodyType = BodyType_Dynamic;
	_Density = 0.0;
	_Radius = 1.0;
	_Extents = Vector3::One;
}

PhysicsEntity::~PhysicsEntity()
{
}

void PhysicsEntity::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();
}

void PhysicsEntity::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);
}

void PhysicsEntity::SetName(String value)
{
	_name = value;
	if (_PhysicsBody != NULL)
		_PhysicsBody->SetName(_name);
}

void PhysicsEntity::SetPosition(Vector3 value)
{
	_Position = value;
	if (_PhysicsBody != NULL)
		_PhysicsBody->SetPosition(_Position);
}

void PhysicsEntity::SetOrientation(Quaternion value)
{
	_Orientation = value;
	if (_PhysicsBody != NULL)
		_PhysicsBody->SetOrientation(_Orientation);
}

void PhysicsEntity::SetLinearVelocity(Vector3 value)
{
	_LinearVelocity = value;
	if (_PhysicsBody != NULL)
		_PhysicsBody->SetLinearVelocity(_LinearVelocity);
}

void PhysicsEntity::SetAngularVelocity(Vector3 value)
{
	_AngularVelocity = value;
	if (_PhysicsBody != NULL)
		_PhysicsBody->SetAngularVelocity(_AngularVelocity);
}

void PhysicsEntity::SetColor(Color32 value)
{
	_Color = value;
	if (_mesh != NULL)
	{
		_mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(0)->
			MaterialState.DiffuseColor = _Color;
	}
}

void PhysicsEntity::SetWireColor(Color32 value)
{
	if (_mesh != NULL)
	{
		_mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(1)->
			MaterialState.DiffuseColor = value;
	}
}

void PhysicsEntity::Initialize(PhysicsWorld* world)
{
	_World = world;

	switch (_ShapeType)
	{
	case Physics::ShapeType_Plane:
		CreatePlane(Plane(Vector3::UnitY, 0.0));
		break;
	case Physics::ShapeType_Sphere:
		CreateSphere(BoundingSphere(Vector3::Zero, _Radius), _Density);
		break;
	case Physics::ShapeType_Box:
		CreateBox(OBB(Vector3::Zero, _Extents, Matrix3::Identity), _Density);
		break;
	}

	_PhysicsBody->SetName(_name);
	_PhysicsBody->SetPosition(_Position);
	_PhysicsBody->SetOrientation(_Orientation);

	if (_PhysicsBody->GetBodyType() == BodyType_Dynamic)
	{
		_PhysicsBody->SetLinearVelocity(_LinearVelocity);
		_PhysicsBody->SetAngularVelocity(_AngularVelocity);
	}

	_mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(0)->
		MaterialState.DiffuseColor = _Color;
}

void PhysicsEntity::CreatePlane(const Plane& plane)
{
	_ShapeType = ShapeType_Plane;

	ShapeDescription shapeDesc;
	shapeDesc.TextureCoordinates = true;

	PlaneShape* planeShape = new PlaneShape(plane);
	planeShape->SetDivisions(1);
	planeShape->SetScale(Vector2(1024.0f, 1024.0f));
	planeShape->CreateMesh(&shapeDesc);
	planeShape->GetMesh()->SetShader(CreatePlaneShader());

	Physics::BodyDescription bodyDesc;
	bodyDesc._BodyType = BodyType_Static;
	Physics::IPlaneShape* shape = new Physics::IPlaneShape();
	shape->Data = plane;
	shape->SetMaterial(NULL);
	bodyDesc._Shapes.Add(shape);

	Create(planeShape->GetMesh(), bodyDesc);
}

void PhysicsEntity::CreateSphere(const BoundingSphere& sphere, real32 density)
{
	_ShapeType = ShapeType_Sphere;
	_Radius = sphere.Radius;
	_Density = density;

	ShapeDescription shapeDesc;
	shapeDesc.TextureCoordinates = true;

	SphereShape* sphereShape = new SphereShape(sphere);
	sphereShape->SetSlices(16);
	sphereShape->SetStacks(16);
	sphereShape->CreateMesh(&shapeDesc);
	sphereShape->GetMesh()->SetShader(CreateDefaultShader());

	Physics::BodyDescription bodyDesc;
	bodyDesc._BodyType = BodyType_Dynamic;
	Physics::ISphereShape* shape = new Physics::ISphereShape();
	shape->Data = sphere;
	shape->SetDensity(density);
	shape->SetMaterial(NULL);
	bodyDesc._Shapes.Add(shape);

	Create(sphereShape->GetMesh(), bodyDesc);
}

void PhysicsEntity::CreateBox(const OBB& box, real32 density)
{
	_ShapeType = ShapeType_Box;
	_Extents = box.Extents;
	_Density = density;

	ShapeDescription shapeDesc;
	shapeDesc.TextureCoordinates = true;

	BoxShape* boxShape = new BoxShape(BoundingBox(box.GetMinimum(), box.GetMaximum()));
	boxShape->SetDivisions(1);
	boxShape->CreateMesh(&shapeDesc);
	boxShape->GetMesh()->SetShader(CreateDefaultShader());

	Physics::BodyDescription bodyDesc;
	bodyDesc._BodyType = BodyType_Dynamic;
	Physics::IBoxShape* shape = new Physics::IBoxShape();
	shape->Data = box;
	shape->SetDensity(density);
	shape->SetMaterial(NULL);
	bodyDesc._Shapes.Add(shape);

	Create(boxShape->GetMesh(), bodyDesc);
}

void PhysicsEntity::CreateModel(Model* model, real32 density)
{
	_ShapeType = ShapeType_Mesh;
	_Density = density;

	Physics::BodyDescription bodyDesc;
	bodyDesc._BodyType = BodyType_Dynamic;
	//Physics::IMeshShape* shape = new Physics::IMeshShape();
	Physics::IBoxShape* shape = new Physics::IBoxShape();
	shape->Data = OBB(Vector3::Zero, model->GetBoundingBox().Max, Matrix3::Identity);
	shape->SetDensity(density);
	shape->SetMaterial(NULL);
	bodyDesc._Shapes.Add(shape);

	_Node = new ModelNode();
	_Node->SetModel(model);

	_PhysicsBody = _World->GetPhysicsScene()->CreateBody(bodyDesc);
}

void PhysicsEntity::Create(Mesh* mesh, const Physics::BodyDescription& bodyDesc)
{
	_mesh = mesh;

	MeshFrame* frame = new MeshFrame();
	frame->AddMeshLOD(0.0f, _mesh);

	Model* model = new Model();
	model->AddMeshFrame(frame);

	_Node = new ModelNode();
	_Node->SetModel(model);

	_PhysicsBody = _World->GetPhysicsScene()->CreateBody(bodyDesc);
	_PhysicsBody->SetUserData(_mesh.Get());
}


SE_IMPLEMENT_CLASS(PhysicsWorld);
SE_IMPLEMENT_REFLECTION(PhysicsWorld);

PhysicsWorld::PhysicsWorld() :
	RefObject(),
	_SkyBox(NULL),
	_Scene(NULL),
	_PhysicsScene(NULL)
{
	_Paused = false;

	_cameraPosition = Vector3(5.0, 5.0, 20.0);
	_cameraOrientation = Quaternion::Identity;

	_ControlledEntity = NULL;

	_IntegrationType = IntegrationType_Euler;
	_Force = Vector3::Zero;
	_ForceStrength = 500.0;

	_ThrownShapeType = Physics::ShapeType_Sphere;
	_ThrownShapeDensity = 10.0;
	_ThrownSphere = BoundingSphere();
	_ThrownBox = OBB();
}

PhysicsWorld::~PhysicsWorld()
{
	int count = _Entities.Count();
	for (int i=0; i<count; i++)
	{
		delete _Entities[i];
	}

	if (_PhysicsScene != NULL)
	{
		PhysicsSystem::Current()->DestroyScene(_PhysicsScene);
		_PhysicsScene = NULL;
	}
}

void PhysicsWorld::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();

	XMLElement* array = document->CreateElement("Entities");
	int count = _Entities.Count();
	array->SetAttribute("Count", String::ToString(count));
	for (int i=0; i<count; i++)
	{
		XMLElement* item = document->CreateElement("Entity");
		context->Serialize(document, item, _Entities.GetItem(i), _Entities.GetItem(i)->GetType());
		array->AppendChild(item);
	}
	element->AppendChild(array);
}

void PhysicsWorld::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	Create();

	XMLElement* array = (XMLElement*)element->GetItem("Entities");
	int count = array->GetAttribute("Count").ToInt32();
	XMLElement* item = (XMLElement*)array->GetItem("Entity");
	for (int i=0; i<count; i++)
	{
		XMLElement* type = (XMLElement*)item->GetFirstChild();
		PhysicsEntity* entity = (PhysicsEntity*)context->Deserialize(item->GetOwnerDocument(), type);
		if (entity != NULL)
		{
			entity->Initialize(this);
			AddEntity(entity);
		}
		item = (XMLElement*)item->GetNextSibling();
	}

	_camera->GetCamera()->SetLocalPosition(_cameraPosition);
	_camera->GetCamera()->SetLocalOrientation(_cameraOrientation);
}

PhysicsEntity* PhysicsWorld::CreateEntity()
{
	PhysicsEntity* entity = new PhysicsEntity();
	entity->SetWorld(this);
	return entity;
}

void PhysicsWorld::CreateBoundsMesh()
{
	RenderSystem* renderer = RenderSystem::Current();

	VertexLayout* vertexLayout;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	uint32 vertexCount = 8;
	HardwareBuffer* vertexBuffer;
	if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		return;
	}

	uint32 indexCount = 24;
	HardwareBuffer* indexBuffer;
	if (!renderer->CreateIndexBuffer(indexCount * sizeof(uint16),
		IndexBufferFormat_Int16, HardwareBufferUsage_Static, &indexBuffer))
	{
		return;
	}

	uint16 indices[] =
	{
		// x axis
		0, 4,
		1, 5,
		2, 6,
		3, 7,

		// y axis
		0, 2,
		1, 3,
		4, 6,
		5, 7,

		// z axis
		0, 1,
		2, 3,
		4, 5,
		6, 7
	};

	SEbyte* ibData;
	indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData);
	Memory::Copy(ibData, &indices, indexCount * sizeof(uint16));
	indexBuffer->Unmap();

	MeshPart* meshPart = new MeshPart();
	vertexData->VertexLayout = vertexLayout;
	meshPart->SetPrimitiveType(PrimitiveType_LineList);
	meshPart->SetVertexBuffer(VertexBufferDescription(vertexCount, vertexBuffer));
	meshPart->SetIndexBuffer(IndexBufferDescription(indexCount, indexBuffer,
		IndexBufferFormat_Int16));
	meshPart->SetIndexed(true);

	_BoundsMesh = new Mesh();
	_BoundsMesh->SetMeshPart(meshPart);

	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_WireFrame;
	pass0->RasterizerState.CullMode = CullMode_None;
	pass0->DepthState.Enable = false;
	technique->AddPass(pass0);
	shader->AddTechnique(technique);

	_BoundsMesh->SetShader(shader);
}


void PhysicsWorld::CreateContactMesh()
{
	RenderSystem* renderer = RenderSystem::Current();

	VertexLayout* vertexLayout;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float1, VertexSemantic_PSize));
	offset += VertexElement::GetTypeSize(VertexFormat_Float1);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	MeshPart* meshPart = new MeshPart();
	vertexData->VertexLayout = vertexLayout;
	meshPart->SetPrimitiveType(PrimitiveType_PointList);

	_ContactMesh = new Mesh();
	_ContactMesh->SetMeshPart(meshPart);

	HardwareBuffer* vertexBuffer;
	if (renderer->CreateVertexBuffer(vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		meshPart->GetVertexBuffer().SetVertexCount(1);
		meshPart->GetVertexBuffer().SetBuffer(vertexBuffer);
	}

	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_Point;
	pass0->RasterizerState.CullMode = CullMode_None;
	pass0->DepthState.Enable = false;
	technique->AddPass(pass0);
	shader->AddTechnique(technique);

	_ContactMesh->SetShader(shader);
}

void PhysicsWorld::CreateNormalMesh()
{
	RenderSystem* renderer = RenderSystem::Current();

	VertexLayout* vertexLayout;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	MeshPart* meshPart = new MeshPart();
	vertexData->VertexLayout = vertexLayout;
	meshPart->SetPrimitiveType(PrimitiveType_LineList);

	_NormalMesh = new Mesh();
	_NormalMesh->SetMeshPart(meshPart);

	HardwareBuffer* vertexBuffer;
	if (renderer->CreateVertexBuffer(2*vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		meshPart->GetVertexBuffer().SetVertexCount(2);
		meshPart->GetVertexBuffer().SetBuffer(vertexBuffer);
	}

	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_WireFrame;
	pass0->RasterizerState.CullMode = CullMode_None;
	pass0->DepthState.Enable = false;
	technique->AddPass(pass0);
	shader->AddTechnique(technique);

	_NormalMesh->SetShader(shader);
}


void PhysicsWorld::GetThrownShape(Physics::ShapeType& type, real& density, BoundingSphere& sphere, OBB& box)
{
	type = _ThrownShapeType;
	density = _ThrownShapeDensity;
	sphere = _ThrownSphere;
	box = _ThrownBox;
}

void PhysicsWorld::SetThrownShape(Physics::ShapeType type, real density, BoundingSphere sphere, OBB box)
{
	_ThrownShapeType = type;
	_ThrownShapeDensity = density;
	_ThrownSphere = sphere;
	_ThrownBox = box;
}

void PhysicsWorld::AddEntity(PhysicsEntity* entity)
{
	_Scene->GetRoot()->AddChild(entity->GetNode());
	_Entities.Add(entity);

	if (_ControlledEntity == NULL)
		_ControlledEntity = entity;
}

void PhysicsWorld::RemoveEntity(PhysicsEntity* entity)
{
	_Scene->GetRoot()->RemoveChild(entity->GetNode());
	_Entities.Remove(entity);
}

int PhysicsWorld::GetEntityCount() const
{
	return _Entities.Count();
}

PhysicsEntity* PhysicsWorld::GetEntity(int index) const
{
	return _Entities[index];
}

void PhysicsWorld::Create()
{
	Camera* camera = new Camera();
	camera->SetPerspective(45.0f, (real32)
		GameCore::Instance()->GetMainWindow()->GetClientWidth() /
		GameCore::Instance()->GetMainWindow()->GetClientHeight(),
		1.0f, 5000.0f);
	camera->SetLocalPosition(_cameraPosition);
	camera->SetLocalOrientation(_cameraOrientation);

	_cameraMode = CameraMode_FirstPerson;
	_camera = new FirstPersonCameraController();
	_camera->SetCamera(camera);
	RenderSystem::Current()->SetCamera(_camera->GetCamera());

	_SkyBox = new SkyBox();

	_SkyBox->SetPlaneTexture(SkyBoxPlane_Left, CreateTexture(_T("Mountains\\negx.jpg")));
	_SkyBox->SetPlaneTexture(SkyBoxPlane_Right, CreateTexture(_T("Mountains\\posx.jpg")));
	_SkyBox->SetPlaneTexture(SkyBoxPlane_Top, CreateTexture(_T("Mountains\\posy.jpg")));
	_SkyBox->SetPlaneTexture(SkyBoxPlane_Bottom, CreateTexture(_T("Mountains\\negy.jpg")));
	_SkyBox->SetPlaneTexture(SkyBoxPlane_Front, CreateTexture(_T("Mountains\\posz.jpg")));
	_SkyBox->SetPlaneTexture(SkyBoxPlane_Back, CreateTexture(_T("Mountains\\negz.jpg")));
	_SkyBox->SetDistance(1000.0);
	_SkyBox->Create();

	_Scene = new Scene();
	_Scene->SetAmbientColor(Color32::Black);
	_Scene->SetRoot(new SceneNode());

	DirectionalLight* light = new DirectionalLight();
	light->SetDirection(-Vector3::UnitY);
	light->SetDiffuseColor(Color32::Black);
	//_Scene->Lights.Add(light);

	Physics::SceneDescription desc;
	desc._TimeStepType = TimeStepType_Fixed;
	_PhysicsScene = PhysicsSystem::Current()->CreateScene(desc);

	_PhysicsScene->PreContact += new EventMethodSlot<PhysicsWorld>(this,
		(EventMethodSlot<PhysicsWorld>::MethodDelegate)&PhysicsWorld::OnPreContact);

	CreateBoundsMesh();
	CreateContactMesh();
	CreateNormalMesh();
}

void PhysicsWorld::ApplyForce(Physics::IBody* body, const Vector3& force, bool torque)
{
	_Force += force;

	if (!torque)
		body->AddForce(force);
	else
		body->AddTorque(force);
}

void PhysicsWorld::UpdateCamera()
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	if (_cameraMode == CameraMode_Free)
	{
		if (keyboard->IsKeyDown(Key_W))
			_camera->HandleMessage(Message(FreeCameraController::Message_MovePZ));

		if (keyboard->IsKeyDown(Key_S))
			_camera->HandleMessage(Message(FreeCameraController::Message_MoveNZ));

		if (keyboard->IsKeyDown(Key_A))
			_camera->HandleMessage(Message(FreeCameraController::Message_MovePX));

		if (keyboard->IsKeyDown(Key_D))
			_camera->HandleMessage(Message(FreeCameraController::Message_MoveNX));

		if (keyboard->IsKeyDown(Key_Z))
			_camera->HandleMessage(Message(FreeCameraController::Message_MovePY));

		if (keyboard->IsKeyDown(Key_C))
			_camera->HandleMessage(Message(FreeCameraController::Message_MoveNY));

		if (keyboard->IsKeyDown(Key_Up))
			_camera->HandleMessage(Message(FreeCameraController::Message_LookPY));

		if (keyboard->IsKeyDown(Key_Down))
			_camera->HandleMessage(Message(FreeCameraController::Message_LookNY));

		if (keyboard->IsKeyDown(Key_Left))
			_camera->HandleMessage(Message(FreeCameraController::Message_LookPX));

		if (keyboard->IsKeyDown(Key_Right))
			_camera->HandleMessage(Message(FreeCameraController::Message_LookNX));

		if (keyboard->IsKeyDown(Key_Q))
			_camera->HandleMessage(Message(FreeCameraController::Message_LookPZ));

		if (keyboard->IsKeyDown(Key_E))
			_camera->HandleMessage(Message(FreeCameraController::Message_LookNZ));
	}
	else if (_cameraMode == CameraMode_FirstPerson)
	{
		if (keyboard->IsKeyDown(Key_Up))
		{
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_LookUp));
		}

		if (keyboard->IsKeyDown(Key_Down))
		{
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_LookDown));
		}

		if (keyboard->IsKeyDown(Key_Left))
		{
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_LookLeft));
		}

		if (keyboard->IsKeyDown(Key_Right))
		{
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_LookRight));
		}

		if (keyboard->IsKeyDown(Key_W))
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_MoveForward));

		if (keyboard->IsKeyDown(Key_S))
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_MoveBackward));

		if (keyboard->IsKeyDown(Key_A))
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_MoveLeft));

		if (keyboard->IsKeyDown(Key_D))
			_camera->HandleMessage(Message(FirstPersonCameraController::Message_MoveRight));
	}
	else if (_cameraMode == CameraMode_LookAt)
	{
		LookAtCameraController* controller = (LookAtCameraController*)_camera;
		if (keyboard->IsKeyDown(Key_Up))
		{
			controller->SetElevation(controller->GetElevation() + 1);
		}

		if (keyboard->IsKeyDown(Key_Down))
		{
			controller->SetElevation(controller->GetElevation() - 1);
		}

		if (keyboard->IsKeyDown(Key_Left))
		{
			controller->SetAngle(controller->GetAngle() - 10);
		}

		if (keyboard->IsKeyDown(Key_Right))
		{
			controller->SetAngle(controller->GetAngle() + 10);
		}

		_camera->HandleMessage(Message(Entity::Message_Update));
	}

	Camera* camera = _camera->GetCamera();
	Vector3 position = camera->GetLocalPosition();
	if (position.Y < 0.5)
	{
		position.Y = 0.5;
		camera->SetLocalPosition(position);
	}
}

void PhysicsWorld::SetControlledEntity(PhysicsEntity* entity)
{
	if (_ControlledEntity != NULL)
	{
		_ControlledEntity->SetWireColor(Color32::Black);
	}

	_ControlledEntity = entity;

	if (_ControlledEntity != NULL)
	{
		_ControlledEntity->SetWireColor(Color32::Blue);
	}
}

void PhysicsWorld::Update(real64 elapsed)
{
	int i;

	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	InputMouse* mouse = (InputMouse*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Mouse);

	UpdateCamera();

	if (keyboard->IsKeyDown(Key_F1))
	{
		if (_cameraMode != CameraMode_Free)
		{
			Camera* camera = _camera->GetCamera();
			delete _camera;

			_cameraMode = CameraMode_Free;
			_camera = new FreeCameraController();
			_camera->SetCamera(camera);
		}
	}
	else if (keyboard->IsKeyDown(Key_F2))
	{
		if (_cameraMode != CameraMode_FirstPerson)
		{
			Camera* camera = _camera->GetCamera();
			delete _camera;

			_cameraMode = CameraMode_FirstPerson;
			_camera = new FirstPersonCameraController();
			_camera->SetCamera(camera);
		}
	}
	else if (keyboard->IsKeyDown(Key_F3))
	{
		if (_cameraMode != CameraMode_LookAt)
		{
			Camera* camera = _camera->GetCamera();
			delete _camera;

			_cameraMode = CameraMode_LookAt;
			LookAtCameraController* controller = new LookAtCameraController();
			controller->SetCamera(camera);
			controller->SetDistance(20);
			_camera = controller;
		}
	}

	int count = _Entities.Count();

	static bool torque = false;

	if (keyboard->IsKeyDown(Key_P) && keyboard->IsKeyToggled(Key_P))
	{
		_Paused = !_Paused;
	}

	static real64 lastFired = 0.0;
	lastFired += elapsed;
	if (keyboard->IsKeyDown(Key_Space) && keyboard->IsKeyToggled(Key_Space) && lastFired >= 0.1)
	{
		lastFired = 0.0;

		PhysicsEntity* entity = NULL;
		if (_ThrownShapeType == Physics::ShapeType_Plane)
		{
		}
		else if (_ThrownShapeType == Physics::ShapeType_Sphere)
		{
			entity = GameCore::Instance()->GetWorld()->CreateEntity();
			entity->CreateSphere(_ThrownSphere, _ThrownShapeDensity);
		}
		else if (_ThrownShapeType == Physics::ShapeType_Box)
		{
			entity = GameCore::Instance()->GetWorld()->CreateEntity();
			entity->CreateBox(_ThrownBox, _ThrownShapeDensity);
		}

		if (entity != NULL)
		{
			Matrix4 matInvView = Matrix4::Inverse(_camera->GetCamera()->GetView());
			Vector4 position(0.0, -0.15, 0.0, 1.0);
			Vector4 velocity(0.0, 0.0, -24.0, 0.0);

			entity->GetPhysicsBody()->SetPosition(Vector3::Transform(position, matInvView));
			entity->GetPhysicsBody()->SetLinearVelocity(Vector3::Transform(velocity, matInvView));
			entity->GetPhysicsBody()->SetName("entity" + String::ToString(GetEntityCount()));
			GameCore::Instance()->AddEntity(entity);
		}
	}

	int x, y;
	mouse->GetMousePosition(x, y);
	//Console::WriteLine(String::Format("%d %d", x, y));
	Ray3 ray = ScreenToWorld(Vector2(x, y));
	//Console::WriteLine(ray.Origin.ToString() + " " + ray.Direction.ToString());

	BaseArray<EntityIntersection> intersections;

	for (i=0; i<count; i++)
	{
		PhysicsEntity* entity = _Entities[i];
		Physics::IShape* shape = entity->GetPhysicsBody()->Shapes().GetItem(0).Get();

		bool intersect = false;
		real result;
		switch (shape->GetShapeType())
		{
			case ShapeType_Plane:
				break;

			/*case ShapeType_Box:
				intersect = Intersection::Ray3Box(
					ray, ((IBoxShape*)shape)->Data);
				break;*/

			case ShapeType_Sphere:
				intersect = ray.Intersects(((ISphereShape*)shape)->Data, result);
				break;
		}

		if (intersect)
		{
			EntityIntersection intersection;
			intersection.Entity = entity;
			intersection.Position = Vector3::Zero;
			intersections.Add(intersection);
		}
		else
		{
			entity->SetWireColor(Color32::Black);
		}
	}

	int interCount = intersections.Count();
	for (i=0; i<interCount; i++)
	{
		EntityIntersection intersection = intersections[i];

		intersection.Entity->SetWireColor(Color32::Red);

		if (mouse->IsMouseClicked(MouseButton_Left))
		{
			SetControlledEntity(intersection.Entity);
		}

		if (mouse->IsMouseClicked(MouseButton_Right))
		{
			intersection.Entity->GetPhysicsBody()->AddForce(ray.Direction * _ForceStrength);
		}
	}

	if (count > 0)
	{
		if (keyboard->IsKeyDown(Key_Tab) && keyboard->IsKeyToggled(Key_Tab))
		{
			if (_ControlledEntity == NULL)
			{
				SetControlledEntity(_Entities[0]);
			}
			else
			{
				int index = _Entities.IndexOf(_ControlledEntity);
				index = (index < count-1 ? index+1 : 0);
				SetControlledEntity(_Entities[index]);
			}
		}
	}

	if (_ControlledEntity != NULL)
	{
		Physics::IBody* controlledBody = _ControlledEntity->GetPhysicsBody();

		_ControlledEntity->SetWireColor(Color32::Blue);

		if (_cameraMode == CameraMode_LookAt)
		{
			LookAtCameraController* controller = (LookAtCameraController*)_camera;
			controller->SetTarget(_ControlledEntity->GetNode());
		}

		if (keyboard->IsKeyDown(Key_F))
			torque = !torque;

		_Force = Vector3::Zero;

		if (keyboard->IsKeyDown(Key_I))
			ApplyForce(controlledBody, -_ForceStrength*Vector3::UnitZ, torque);
		if (keyboard->IsKeyDown(Key_K))
			ApplyForce(controlledBody, _ForceStrength*Vector3::UnitZ, torque);
		if (keyboard->IsKeyDown(Key_J))
			ApplyForce(controlledBody, -_ForceStrength*Vector3::UnitX, torque);
		if (keyboard->IsKeyDown(Key_L))
			ApplyForce(controlledBody, _ForceStrength*Vector3::UnitX, torque);
		if (keyboard->IsKeyDown(Key_U))
			ApplyForce(controlledBody, _ForceStrength*Vector3::UnitY, torque);
		if (keyboard->IsKeyDown(Key_M))
			ApplyForce(controlledBody, -_ForceStrength*Vector3::UnitY, torque);
	}

	if (!_Paused)
	{
		// Update the contact list, they are kept visible 100ms (they show up in paused mode)
		for (i=0; i<_Contacts.Count(); i++)
		{
			_Contacts[i].t += elapsed;
			if (_Contacts[i].t > 0.1)
			{
				_Contacts.RemoveAt(i);
				i--;
			}
		}

		PhysicsSystem::Current()->Update(elapsed);
	}

	count = _Entities.Count();
	for (i=0; i<count; i++)
	{
		PhysicsEntity* entity = _Entities[i];
		ModelNode* node = entity->GetNode();
		Physics::IBody* body = entity->GetPhysicsBody();

		node->SetLocalPosition(body->GetPosition());
		node->SetLocalOrientation(body->GetOrientation());
	}

	if (_Scene != NULL)
		_Scene->Update(elapsed);
}

void PhysicsWorld::RenderBounds(Physics::IBody* body)
{
	RenderSystem* renderer = RenderSystem::Current();

	AABB aabb = body->GetWorldBoundingBox();

	struct BoundsMeshGeometry
	{
		Vector3 Position;
		uint32 Color;
	};

	uint32 color = Color32::White.ToARGB();

	BoundsMeshGeometry vertices[] =
	{
		{ Vector3(aabb.Min.X, aabb.Min.Y, aabb.Min.Z), color },
		{ Vector3(aabb.Min.X, aabb.Min.Y, aabb.Max.Z), color },
		{ Vector3(aabb.Min.X, aabb.Max.Y, aabb.Min.Z), color },
		{ Vector3(aabb.Min.X, aabb.Max.Y, aabb.Max.Z), color },
		{ Vector3(aabb.Max.X, aabb.Min.Y, aabb.Min.Z), color },
		{ Vector3(aabb.Max.X, aabb.Min.Y, aabb.Max.Z), color },
		{ Vector3(aabb.Max.X, aabb.Max.Y, aabb.Min.Z), color },
		{ Vector3(aabb.Max.X, aabb.Max.Y, aabb.Max.Z), color }
	};

	uint32 vertexCount = 8;
	HardwareBuffer* vertexBuffer = _BoundsMesh->GetMeshPart()->GetVertexBuffer().GetBuffer();
	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);
	Memory::Copy(vbData, vertices, vertexCount*sizeof(BoundsMeshGeometry));
	vertexBuffer->Unmap();

	renderer->SetWorldTransform(Matrix4::Identity);
	renderer->RenderMesh(_BoundsMesh);
}

void PhysicsWorld::RenderContact(const ContactInfo& info)
{
	RenderSystem* renderer = RenderSystem::Current();

	struct ContactMeshGeometry
	{
		Vector3 Position;
		real32 PSize;
		uint32 Color;
	};

	ContactMeshGeometry vertex;
	vertex.Position = info.GetPosition();
	vertex.PSize = 0.020;
	vertex.Color = Color32::Red.ToARGB();

	HardwareBuffer* vertexBuffer = _ContactMesh->GetMeshPart()->GetVertexBuffer().GetBuffer();
	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);
	Memory::Copy(vbData, &vertex, sizeof(ContactMeshGeometry));
	vertexBuffer->Unmap();

	PointState state;
	state.Enable = true;
	state.Size = 1.0f;
	state.ScaleEnable = true;
	state.ConstantScale = 1.0f;
	state.LinearScale = 0.0f;
	state.QuadraticScale = 0.01f;

	renderer->SetWorldTransform(Matrix4::Identity);
	renderer->SetPointState(state);
	renderer->RenderMesh(_ContactMesh);
	renderer->SetPointState(PointState());
}

void PhysicsWorld::RenderArrow(const Vector3& ptA, const Vector3& ptB, const Color32& color)
{
	RenderSystem* renderer = RenderSystem::Current();

	struct NormalMeshGeometry
	{
		Vector3 Position;
		uint32 Color;
	};

	NormalMeshGeometry vertices[2];
	vertices[0].Position = ptA;
	vertices[1].Position = ptB;
	vertices[0].Color = color.ToARGB();
	vertices[1].Color = color.ToARGB();

	HardwareBuffer* vertexBuffer = _NormalMesh->GetMeshPart()->GetVertexBuffer().GetBuffer();
	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);
	Memory::Copy(vbData, vertices, 2*sizeof(NormalMeshGeometry));
	vertexBuffer->Unmap();

	renderer->SetWorldTransform(Matrix4::Identity);
	renderer->RenderMesh(_NormalMesh);
}

void PhysicsWorld::Render()
{
	int i;

	if (_SkyBox != NULL)
		_SkyBox->Render();

	if (_Scene != NULL)
		_Scene->Render();

	int count = _Entities.Count();
	for (i=0; i<count; i++)
	{
		RenderBounds(_Entities[i]->GetPhysicsBody());
	}

	if (_ControlledEntity != NULL && _Force != Vector3::Zero)
	{
		Vector3 pos = _ControlledEntity->GetPhysicsBody()->GetPosition();
		RenderArrow(pos, pos + Vector3::Normalize(_Force) * 0.5, Color32::Yellow);
	}

	for (i=0; i<_Contacts.Count(); i++)
	{
		const ContactInfo& info = _Contacts[i].info;
		RenderContact(info);
		RenderArrow(info.GetPosition(), info.GetPosition() + info.GetNormal() * 0.5, Color32::Green);
	}
}

bool PhysicsWorld::OnPreContact(Object* sender, const Physics::ContactEventArgs* e)
{
	ContactTime ct;
	ct.info = e->Info();
	ct.t = 0.0;
	_Contacts.Add(ct);

	return true;
}
