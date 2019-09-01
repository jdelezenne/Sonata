/*=============================================================================
AIController.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "AIController.h"
#include "SampleStealth.h"
#include "States.h"

SE_IMPLEMENT_CLASS(AIController);
SE_IMPLEMENT_REFLECTION(AIController);

AIController::AIController() :
	super()
{
	_TargetVisible = false;
	_TargetLastVisiblePosition = Vector3::Zero;
	_TargetLostTime = 0.0;

	_CharacterActorState = NULL;
	_NextActorState = NULL;
	SetCharacterActorState("Idle");

	_LOSMesh = NULL;
	_LastLOSCheck = 0.0;
}

AIController::~AIController()
{
}

void AIController::SetCharacterActorState(const String& value)
{
	_NextActorStateName = value;

	_NextActorState = (CharacterActorState*)TypeFactory::Instance()->CreateInstance("CharacterActorState" + value);
}

void AIController::ChangeActorState()
{
	SE_DELETE(_CharacterActorState);

	if (_NextActorState != NULL)
	{
		_CharacterActorState = _NextActorState;
		if (_Character != NULL)
		{
			Logger::Current()->Log(LogLevel::Information, _T("AIController.ChangeActorState"),
				_T("Character <") + _Character->_name +
				_T("> is switching to state <") + _NextActorStateName + _T(">"));
		}

		_CharacterActorState->Initialize(this);

		_NextActorStateName = String::Empty;
		_NextActorState = NULL;
	}
}

Mesh* AIController::CreateLOSMesh(real32 viewConeAngle, real32 viewDistance)
{
	real32 smoothness = 1.0f;
	uint32 color0 = Color32(0.0f, 0.0f, 1.0f, 0.5f).ToARGB();
	uint32 color1 = Color32(0.0f, 0.0f, 1.0f, 0.0f).ToARGB();

	RenderSystem* renderer = RenderSystem::Current();

	VertexLayout* vertexLayout;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		return NULL;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	uint32 vertexCount = 7;
	HardwareBuffer* vertexBuffer;
	if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		return NULL;
	}

	struct LOSMeshVertex
	{
		Vector3 Position;
		uint32 Color;
	};

	LOSMeshVertex vertices[] =
	{
		{ Vector3(0.0, 0.0, 0.0), color0 },
		{ Matrix3::RotationZ(-viewConeAngle/2)*Vector3(0.0, viewDistance, 0.0), color1 },
		{ Matrix3::RotationZ(-viewConeAngle/4)*Vector3(0.0, viewDistance, 0.0), color1 },
		{ Vector3(0.0, viewDistance, 0.0), color1 },
		{ Matrix3::RotationZ(viewConeAngle/4)*Vector3(0.0, viewDistance, 0.0), color1 },
		{ Matrix3::RotationZ(viewConeAngle/2)*Vector3(0.0, viewDistance, 0.0), color1 },
		{ Vector3(0.0, 0.0, 0.0), color0 }
	};

	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);
	memcpy(vbData, vertices, vertexCount*sizeof(LOSMeshVertex));
	vertexBuffer->Unmap();

	MeshPart* meshPart = new MeshPart();
	meshPart->SetVertexLayout(vertexLayout);
	meshPart->SetPrimitiveType(PrimitiveType_TriangleFan);
	meshPart->SetVertexBuffer(VertexBufferDescription(vertexCount, vertexBuffer));

	Mesh* mesh = new Mesh();
	mesh->SetMeshPart(meshPart);

	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_Solid;
	pass0->RasterizerState.CullMode = CullMode_None;
	pass0->AlphaState.BlendEnable[0] = true;
	pass0->AlphaState.SourceBlend = BlendMode_SourceAlpha;
	pass0->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
	technique->AddPass(pass0);
	shader->AddTechnique(technique);

	mesh->SetShader(shader);
	return mesh;
}

void AIController::SetLOSMeshColor(const Color32& color)
{
	uint32 color0 = Color32(color.r, color.g, color.b, 0.5f).ToARGB();
	uint32 color1 = Color32(color.r, color.g, color.b, 0.0f).ToARGB();

	RenderSystem* renderer = RenderSystem::Current();

	MeshPart* meshPart = _LOSMesh->GetMeshPart();
	if (meshPart == NULL)
	{
		return;
	}

	uint32 vertexCount = meshPart->GetVertexBuffer().GetVertexCount();
	HardwareBuffer* vertexBuffer = meshPart->GetVertexBuffer().GetBuffer();

	struct LOSMeshVertex
	{
		Vector3 Position;
		uint32 Color;
	};

	LOSMeshVertex* vertices;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vertices);
	vertices[0].Color = color0;
	vertices[1].Color = color1;
	vertices[2].Color = color1;
	vertices[3].Color = color1;
	vertices[4].Color = color1;
	vertices[5].Color = color1;
	vertices[6].Color = color0;
	vertexBuffer->Unmap();
}

bool AIController::CheckLineOfSight()
{
	int i;

	GameWorld* world = GameCore::Instance()->GetWorld();
	Map* map = world->GetMap();
	Player* player = world->GetPlayer();
	if (player == NULL || player->GetCharacter() == NULL)
		return false;

	Vector3 playerPos = player->GetCharacter()->_Position;
	Vector3 aiDir = _Character->_Orientation.Rotate(Vector3::UnitY);

	real distance = Vector2(_Character->_Position.X, _Character->_Position.Y).Distance(
		Vector2(playerPos.X, playerPos.Y));
	if (distance > _Character->_ViewDistance)
		return false;

	Vector2 v1 = Vector2(aiDir.X, aiDir.Y);
	Vector2 v2 = Vector2(
		playerPos.X - _Character->_Position.X,
		playerPos.Y - _Character->_Position.Y);

	real angle = Vector2::Angle(v1, v2);
	if (angle > Math::ToRadians(_Character->_ViewConeAngle) / 2.0f)
		return false;

	Segment line(_Character->_Position, player->GetCharacter()->_Position);
	line.p0.Z += map->_CellSize / 2.0;
	line.p1.Z += map->_CellSize / 2.0;

	// World intersections
	int cellCount = map->_Cells.Count();
	for (i=0; i<cellCount; i++)
	{
		Cell* cell = map->_Cells[i];

		for (int j=0; j<cell->_Depth; j++)
		{
			Matrix4 m = Matrix4(Matrix3::Identity,
				Vector3(0.5+cell->_X, 0.5+cell->_Y, 0.5+j));
			m.Scale(Vector3::One * map->_CellSize);

			AABB aabb = AABB(-Vector3(0.5), Vector3(0.5));
			aabb.Transform(m);

			//TOFIX
			/*if (Intersection::Segment3Box(line, Box(aabb, Matrix3::Identity)))
			{
				return false;
			}*/
		}
	}

	// Actors intersections
	int actorCount = map->_Actors.Count();
	for (i=0; i<actorCount; i++)
	{
		Actor* actor = map->_Actors[i];
		if (actor->_ActorDef->_ActorType != ActorType_Character &&
			actor->_ActorDef->_ActorType != ActorType_Placeable)
		{
			continue;
		}
		if (actor == _Character || actor == player->GetCharacter())
			continue;

		Matrix4 m = Matrix4(Matrix3::CreateFromQuaternion(actor->_Orientation), actor->_Position);
		AABB aabb = actor->_ActorDef->_Model->GetBoundingBox();
		aabb.Transform(m);

		//TOFIX
		/*if (Intersection::Segment3Box(line, Box(aabb, Matrix3::Identity)))
		{
			return false;
		}*/
	}

	_TargetLastVisiblePosition = playerPos;

	return true;
}

void AIController::Update(real64 elapsed)
{
	super::Update(elapsed);

	if (_Character == NULL)
		return;

	if (_LOSMesh == NULL)
	{
		_LOSMesh = CreateLOSMesh(Math::ToRadians(_Character->_ViewConeAngle),
			_Character->_ViewDistance);
	}

	if (_CharacterActorState != NULL)
	{
		_CharacterActorState->PreUpdate(elapsed);
		_CharacterActorState->Update(elapsed);
		_CharacterActorState->PostUpdate(elapsed);
	}

	// Do the LOS check
	real64 t = GameCore::GetTime();
	if (t - _LastLOSCheck > 0.2)
	{
		_LastLOSCheck = t;
		bool wasVisible = _TargetVisible;
		_TargetVisible = CheckLineOfSight();

		if (_TargetVisible && !wasVisible)
		{
			SetLOSMeshColor(Color32::Red);
		}
		if (!_TargetVisible && wasVisible)
		{
			_TargetLostTime = GameCore::GetTime();
			SetLOSMeshColor(Color32::Blue);
		}
	}

	// Do the delayed state change
	if (!_NextActorStateName.IsEmpty())
	{
		ChangeActorState();
	}
}

void AIController::Draw()
{
	if (_Character == NULL)
		return;

	if (_LOSMesh == NULL)
		return;

	RenderSystem* renderer = RenderSystem::Current();
	renderer->SetWorldTransform(Matrix4(
		Matrix3::CreateFromQuaternion(_Character->_Orientation), _Character->_Position));
	renderer->RenderMesh(_LOSMesh);
}

void AIController::OnPlayerKilled()
{
	if (_TargetVisible)
	{
		_TargetVisible = false;
		_TargetLostTime = GameCore::GetTime();
		SetLOSMeshColor(Color32::Blue);
	}
}