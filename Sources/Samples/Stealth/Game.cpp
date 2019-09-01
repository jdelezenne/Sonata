/*=============================================================================
Game.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SampleStealth.h"
#include "Game.h"

const real32 CameraPositionY = -256.0;

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

ShaderMaterial* CreateCursorShader()
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_WireFrame;
	pass0->DepthState.Enable = false;
	//pass0->MaterialState.DiffuseColor = Color32::Red;
	pass0->LightState.Lighting = false;

	SamplerState* sampler0 = new SamplerState();
	pass0->AddSamplerState(sampler0);
	TextureState* texture0 = new TextureState();
	texture0->ColorOperation = TextureOperation_SelectArg1;
	texture0->ColorArgument1 = TextureArgument_Constant;
	texture0->ConstantColor = Color32::Red;
	pass0->AddTextureState(texture0);
	technique->AddPass(pass0);

	technique->AddPass(pass0);

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

Cursor::Cursor()
{
	_Position = Vector2::Zero;

	BoxShape* shape = new BoxShape(BoundingBox(Vector3(-0.5), Vector3(0.5)));
	shape->SetDivisions(1);
	shape->CreateMesh(NULL);
	_mesh = shape->GetMesh();
	_mesh->SetShader(CreateCursorShader());
	//delete shape;
}

void Cursor::Draw()
{
	GameWorld* world = GameCore::Instance()->GetWorld();
	if (world->GetEditorState() == EditorState_View)
		return;

	if (_mesh != NULL)
	{
		Map* map = world->GetMap();
		Matrix4 m = Matrix4::Identity;

		if (world->GetEditorState() == EditorState_Tiles)
		{
			m.Translate(Vector3(0.5+(int32)(_Position.X / map->_CellSize), 0.5+(int32)(_Position.Y / map->_CellSize), 0.5+world->GetEditorDepth()-1));
			m.Scale(Vector3::One * map->_CellSize);
		}
		else if (world->GetEditorState() == EditorState_Actors)
		{
			m.Translate(Vector3(_Position.X / map->_CellSize, _Position.Y / map->_CellSize, 0.5+world->GetEditorDepth()-1));
			m.Scale(Vector3::One * map->_CellSize);
		}

		RenderSystem* renderer = RenderSystem::Current();
		renderer->SetWorldTransform(m);
		renderer->RenderMesh(_mesh);
	}
}


GameWorld::GameWorld() :
	_Scene(NULL)
{
	_Paused = false;
	_EndGameRequest = false;

	_BoundsMesh = NULL;

	_Map = NULL;
	_Tileset = NULL;

	_Player = NULL;
}

void GameWorld::LoadTileset(const String& fileName)
{
	File* file = new File(fileName);
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
	if (stream != NULL)
	{
		_Tileset = (Tileset*)serializer->Deserialize(stream.Get());
		stream->Close();
	}

	delete serializer;
	delete file;
}

void GameWorld::SaveTileset(const String& fileName)
{
	if (_Tileset != NULL)
	{
		File* file = new File(fileName);
		XMLSerializer* serializer  = new XMLSerializer();

		FileStreamPtr stream = file->Open(FileMode_Create);
		if (stream != NULL)
		{
			serializer->Serialize(stream.Get(), _Tileset);
			stream->Close();
		}

		delete serializer;
		delete file;
	}
}

void GameWorld::NewMap(int32 width, int32 height, int32 depth)
{
	SE_DELETE(_Map);

	_Map = new Map();
	_Map->_name = "Map";
	_Map->_Width = width;
	_Map->_Height = height;

	for (int y=0; y<_Map->_Height; y++)
	{
		for (int x=0; x<_Map->_Width; x++)
		{
			Cell* cell = new Cell();
			cell->_Tile = _Tileset->_Tiles[0];
			cell->_X = x;
			cell->_Y = y;
			cell->_Depth = depth;
			_Map->_Cells.Add(cell);
		}
	}

	SetCursor(0.0f, 0.0f);
	GameCore::Instance()->GetGameUI()->Initialize(_Map);
}

void GameWorld::LoadMap(const String& fileName)
{
	File* file = new File(fileName);
	XMLSerializer* serializer  = new XMLSerializer();

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read);
	if (stream != NULL)
	{
		_Map = (Map*)serializer->Deserialize(stream.Get());
		stream->Close();
	}

	delete serializer;

	SetCursor(0.0f, 0.0f);
	GameCore::Instance()->GetGameUI()->Initialize(_Map);
}

void GameWorld::SaveMap(const String& fileName)
{
	if (_Map != NULL)
	{
		File* file = new File(fileName);
		XMLSerializer* serializer  = new XMLSerializer();

		FileStreamPtr stream = file->Open(FileMode_Create);
		if (stream != NULL)
		{
			serializer->Serialize(stream.Get(), _Map);
			stream->Close();
		}

		delete serializer;
	}
}

void GameWorld::NewGame()
{
	EndGame();

	if (_Map == NULL)
		return;

	int count;
	int i;

	if (_Map->GetActorOfType(ActorType_Spawn) != NULL)
	{
		CharacterActor* playerCharacter = NULL;

		count = _Tileset->_ActorDefs.Count();
		for (i=0; i<count; i++)
		{
			ActorDef* actordef = _Tileset->_ActorDefs[i];
			if (actordef->_ActorType == ActorType_Character &&
				actordef->_name == _Tileset->_PlayerActor)
			{
				CharacterActor* character = new CharacterActor();
				character->_ActorDef = actordef;
				_Map->AddActor(character);
				GameCore::Instance()->GetGameUI()->AddEntity(character);
				playerCharacter = character;
				break;
			}
		}

		if (playerCharacter != NULL)
		{
			_Player = new Player();
			_Player->SetCharacter(playerCharacter);
			_Player->Respawn();
		}
	}

	_Map->Initialize();

	count = _Map->_Actors.Count();
	for (i=0; i<count; i++)
	{
		Actor* actor = _Map->_Actors[i];
		if (actor->_ActorDef->_ActorType == ActorType_Character)
		{
			if (_Player != NULL && _Player->GetCharacter() == actor)
				continue;

			AIController* ai = new AIController();
			CharacterActor* character = (CharacterActor*)actor;
			character->_GuardPosition = character->_Position;
			character->_GuardOrientation = character->_Orientation;
			ai->SetCharacter(character);
			ai->SetCharacterActorState("Idle");
			_AIControllers.Add(ai);
		}
	}
}

void GameWorld::EndGame()
{
	if (!_Editor)
	{
		KillPlayer();

		{
			AIController* ai;
			foreach (ai, _AIControllers, Array<AIController*>)
			{
				SE_DELETE(ai);
			}
			_AIControllers.Clear();
		}

		{
			Actor* actor;
			foreach (actor, _Map->_Actors, Map::ActorList)
			{
				if (actor->_ActorDef->_ActorType == ActorType_Character)
				{
					CharacterActor* character = (CharacterActor*)actor;
					character->_Position = character->_GuardPosition;
					character->_Orientation = character->_GuardOrientation;
				}
			}
		}
	}
}

void GameWorld::KillPlayer()
{
	if (_Player != NULL)
	{
		GameCore::Instance()->GetGameUI()->RemoveEntity(_Player->GetCharacter());
		SE_DELETE(_Player);
	}

	AIController* ai;
	foreach (ai, _AIControllers, Array<AIController*>)
	{
		ai->OnPlayerKilled();
	}
}

void GameWorld::EndGameRequest()
{
	_EndGameRequest = true;
}

void GameWorld::CreateBoundsMesh()
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
	memcpy(ibData, &indices, indexCount * sizeof(uint16));
	indexBuffer->Unmap();

	MeshPart* meshPart = new MeshPart();
	meshPart->SetVertexLayout(vertexLayout);
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

void GameWorld::RenderBounds(const AABB& aabb)
{
	RenderSystem* renderer = RenderSystem::Current();

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
	memcpy(vbData, vertices, vertexCount*sizeof(BoundsMeshGeometry));
	vertexBuffer->Unmap();

	renderer->SetWorldTransform(Matrix4::Identity);
	renderer->RenderMesh(_BoundsMesh);
}

void GameWorld::Create()
{
	Camera* camera = new Camera();
	camera->SetPerspective(45.0f, (real32)
		GameCore::Instance()->GetMainWindow()->GetClientWidth() /
		GameCore::Instance()->GetMainWindow()->GetClientHeight(),
		1.0f, 5000.0f);
	camera->SetLocalPosition(Vector3(0, CameraPositionY, 1024));
	camera->LookAt(Vector3(0, 0, 0));

	_camera = camera;
	RenderSystem::Current()->SetCamera(_camera);

	RenderSystem::Current()->SetClearColor(Color32::Black);

	_Scene = new Scene();
	_Scene->SetAmbientColor(Color32::Black);
	_Scene->SetRoot(new SceneNode());

	_Editor = false;
	_EditorState = EditorState_Tiles;
	_EditorDepth = 1;
	_ShowBoundingBoxes = false;
	_SelectedActor = NULL;

	CreateBoundsMesh();
}

void GameWorld::SetCursor(real32 x, real32 y)
{
	if (_EditorState == EditorState_Tiles)
	{
		_Cursor._Position.X = Math::Clamp(x, 0.0f, (real32)((_Map->_Width-1) * _Map->_CellSize));
		_Cursor._Position.Y = Math::Clamp(y, 0.0f, (real32)((_Map->_Height-1) * _Map->_CellSize));
	}
	else if (_EditorState == EditorState_Actors)
	{
		_Cursor._Position.X = Math::Clamp(x, 0.0f, (real32)(_Map->_Width * _Map->_CellSize - 1));
		_Cursor._Position.Y = Math::Clamp(y, 0.0f, (real32)(_Map->_Height * _Map->_CellSize - 1));
	}

	Vector3 position(_Cursor._Position.X, _Cursor._Position.Y + CameraPositionY,
		_camera->GetLocalPosition().Z);
	_camera->SetLocalPosition(position);
	_camera->LookAt(Vector3(_Cursor._Position.X, _Cursor._Position.Y, _EditorDepth * _Map->_CellSize));
}

void GameWorld::SetCursorOnSelectedActor()
{
	if (_SelectedActor != NULL)
	{
		SetCursor(_SelectedActor->_Position.X, _SelectedActor->_Position.Y);
	}
}

void GameWorld::SetEditor(bool value)
{
	_Editor = value;

	GameCore::Instance()->GetGameUI()->SetEditor(_Editor);

	SetCursor(_Cursor._Position.X, _Cursor._Position.Y);
}

void GameWorld::UpdateCamera()
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);
	InputMouse* mouse = (InputMouse*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Mouse);

	if (keyboard->IsKeyDown(Key_PageUp))
	{
		_camera->Translate(-Vector3::UnitZ*10.0);

		if (_Editor)
		{
			_camera->LookAt(Vector3(_Cursor._Position.X, _Cursor._Position.Y, _EditorDepth * _Map->_CellSize));
		}
	}

	if (keyboard->IsKeyDown(Key_PageDown))
	{
		_camera->Translate(Vector3::UnitZ*10.0);
		if (_Editor)
		{
			_camera->LookAt(Vector3(_Cursor._Position.X, _Cursor._Position.Y, _EditorDepth * _Map->_CellSize));
		}
	}
}

void GameWorld::OnToolBarTile()
{
	Tile* tile = GameCore::Instance()->GetGameUI()->GetSelectedTile();

	_Map->SetCellTileAt(tile, _Map->WorldToCell(_Cursor._Position));
}

void GameWorld::OnToolBarActor()
{
	ActorDef* actordef = GameCore::Instance()->GetGameUI()->GetSelectedActorDef();

	String actorTypeName = EnumObject::GetName(typeof(ActorType), actordef->_ActorType);
	Actor* actor = (Actor*)TypeFactory::Instance()->CreateInstance(actorTypeName + "Actor");
	if (actor != NULL)
	{
		actor->_ActorDef = actordef;
		Cell* cell = _Map->GetCellAt(_Cursor._Position / _Map->_CellSize);
		_Map->SetActorPosition(actor, _Cursor._Position);
		actor->_name = _Map->GetUniqueName(actordef->_ActorType);
		_Map->AddActor(actor);
		SetSelectedActor(actor);

		GameCore::Instance()->GetGameUI()->AddEntity(actor);
		GameCore::Instance()->GetGameUI()->SetSelectedObject(actor);
	}
}

void GameWorld::Update(real64 elapsed)
{
	int i;

	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);

	InputMouse* mouse = (InputMouse*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Mouse);

	UpdateCamera();

	static real64 delta = 0.0;
	delta += elapsed;
	if (delta > 0.1)
	{
		delta = 0.0;

		if (_Editor)
		{
			if (_EditorState == EditorState_Tiles)
			{
				if (keyboard->IsKeyDown(Key_Up))
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y + _Map->_CellSize);

				if (keyboard->IsKeyDown(Key_Down))
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y - _Map->_CellSize);

				if (keyboard->IsKeyDown(Key_Left))
					SetCursor(_Cursor._Position.X - _Map->_CellSize, _Cursor._Position.Y);

				if (keyboard->IsKeyDown(Key_Right))
					SetCursor(_Cursor._Position.X + _Map->_CellSize, _Cursor._Position.Y);
			}
			else if (_EditorState == EditorState_Actors)
			{
				real32 move;
				if (keyboard->IsKeyDown(Key_LeftShift) || keyboard->IsKeyDown(Key_RightShift))
					move = _Map->_CellSize;
				else
					move = 500.0 * elapsed;

				if (keyboard->IsKeyDown(Key_Up))
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y + move);

				if (keyboard->IsKeyDown(Key_Down))
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y - move);

				if (keyboard->IsKeyDown(Key_Left))
					SetCursor(_Cursor._Position.X - move, _Cursor._Position.Y);

				if (keyboard->IsKeyDown(Key_Right))
					SetCursor(_Cursor._Position.X + move, _Cursor._Position.Y);

				if (keyboard->IsKeyDown(Key_LeftShift) || keyboard->IsKeyDown(Key_RightShift))
				{
					_Cursor._Position.X -= (int32)_Cursor._Position.X % _Map->_CellSize / 2.0;
					_Cursor._Position.Y -= (int32)_Cursor._Position.Y % _Map->_CellSize / 2.0;
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y);
				}
			}
		}
		else
		{
			if (_Player != NULL && _Player->GetCharacter() != NULL)
			{
				Vector3 pos = _Player->GetCharacter()->_Position;
				pos.Y += CameraPositionY;
				pos.Z = _camera->GetLocalPosition().Z;
				_camera->SetLocalPosition(pos);
				_camera->LookAt(_Player->GetCharacter()->_Position);
			}
			else
			{
				if (keyboard->IsKeyDown(Key_Up))
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y + _Map->_CellSize);

				if (keyboard->IsKeyDown(Key_Down))
					SetCursor(_Cursor._Position.X, _Cursor._Position.Y - _Map->_CellSize);

				if (keyboard->IsKeyDown(Key_Left))
					SetCursor(_Cursor._Position.X - _Map->_CellSize, _Cursor._Position.Y);

				if (keyboard->IsKeyDown(Key_Right))
					SetCursor(_Cursor._Position.X + _Map->_CellSize, _Cursor._Position.Y);
			}
		}
	}

	if (IsKeyPushed(Key_E))
	{
		SetEditor(!_Editor);
	}

	if (_Editor)
	{
		Cell* cell = _Map->GetCellAt(Vector2(
			_Cursor._Position.X / _Map->_CellSize, _Cursor._Position.Y / _Map->_CellSize));
		_EditorDepth = cell->_Depth;

		if (_EditorState == EditorState_Tiles)
		{
			if (IsKeyPushed(Key_Return))
			{
				OnToolBarTile();
			}

			if (IsKeyPushed(Key_Subtract))
			{
				if (cell->_Depth > 1)
					cell->_Depth--;
			}
			else if (IsKeyPushed(Key_Add))
			{
				if (cell->_Depth < 8)
					cell->_Depth++;
			}

			if (IsKeyPushed(Key_Home))
			{
			}
			else if (IsKeyPushed(Key_End))
			{
			}
		}
		else if (_EditorState == EditorState_Actors)
		{
			if (IsKeyPushed(Key_Return))
			{
				OnToolBarActor();
			}

			if (_SelectedActor != NULL)
			{
				if (IsKeyPushed(Key_Delete))
				{
					_Map->_Actors.Remove(_SelectedActor);
					GameCore::Instance()->GetGameUI()->RemoveEntity(_SelectedActor);
					_SelectedActor = NULL;
				}

				if (IsKeyPushed(Key_Home))
				{
					_SelectedActor->_Orientation *= Quaternion::FromAxisAngle(Vector3::UnitZ, Math::Pi / 4.0);
					_SelectedActor->_Orientation.Normalize();
				}

				if (IsKeyPushed(Key_End))
				{
					_SelectedActor->_Orientation *= Quaternion::FromAxisAngle(Vector3::UnitZ, -Math::Pi / 4.0);
					_SelectedActor->_Orientation.Normalize();
				}
			}
		}
	}
	else
	{
	}

	if (IsKeyPushed(Key_Space))
	{
		if (_Editor)
		{
			NewGame();
			SetEditor(false);
		}
		else
		{
			EndGame();
			SetEditor(true);
		}
	}

	if (IsKeyPushed(Key_P))
	{
		_Paused = !_Paused;
	}

	if (_Paused)
	{
		return;
	}

	if (_Scene != NULL)
	{
		_Scene->Update(elapsed);
	}

	if (_Map != NULL)
	{
		_Map->Update(elapsed);
	}

	if (_Player != NULL)
	{
		_Player->Update(elapsed);
	}

	AIController* ai;
	foreach (ai, _AIControllers, Array<AIController*>)
	{
		ai->Update(elapsed);
	}
	
	if (_EndGameRequest)
	{
		_EndGameRequest = false;
		EndGame();
	}
}

void GameWorld::Render()
{
	int i;

	RenderSystem* renderer = RenderSystem::Current();
	Camera* camera = SceneManager::Instance()->GetCamera();
	camera->SetPerspective(45.0f, (real32)
		GameCore::Instance()->GetMainWindow()->GetClientWidth() /
		GameCore::Instance()->GetMainWindow()->GetClientHeight(),
		1.0f, 5000.0f);
	renderer->SetProjectionTransform(camera->GetProjection());
	renderer->SetViewTransform(camera->GetView());
	renderer->SetWorldTransform(Matrix4::Identity);

	if (_Scene != NULL)
		_Scene->Render();

	if (_Map != NULL)
	{
		_Map->Draw();
	}

	if (_Player != NULL)
	{
		_Player->Draw();
	}

	AIController* ai;
	foreach (ai, _AIControllers, Array<AIController*>)
	{
		ai->Draw();
	}

	if (_Editor)
	{
		if (_SelectedActor != NULL)
		{
			Matrix4 m = Matrix4(Matrix3::CreateFromQuaternion(_SelectedActor->_Orientation), _SelectedActor->_Position);
			AABB aabb = _SelectedActor->_ActorDef->_Model->GetBoundingBox();
			aabb.Transform(m);
			RenderBounds(aabb);
		}

		_Cursor.Draw();
	}

	/*int cellCount = _Map->_Cells.Count();
	for (i=0; i<cellCount; i++)
	{
		Cell* cell = _Map->_Cells[i];

		for (int j=0; j<cell->_Depth; j++)
		{
			Matrix4 m = Matrix4(Matrix3::Identity,
				Vector3(0.5+cell->_X, 0.5+cell->_Y, 0.5+j));
			m.Scale(Vector3::One * _Map->_CellSize);

			AABB aabb = AABB(-Vector3(0.5), Vector3(0.5));
			aabb.Transform(m);
			RenderBounds(aabb);
		}
	}*/

	if (_ShowBoundingBoxes)
	{
		int actorCount = _Map->_Actors.Count();
		for (i=0; i<actorCount; i++)
		{
			Actor* actor = _Map->_Actors[i];

			if (actor == _SelectedActor)
				continue;

			Matrix4 m = Matrix4(Matrix3::CreateFromQuaternion(actor->_Orientation), actor->_Position);
			AABB aabb = actor->_ActorDef->_Model->GetBoundingBox();
			aabb.Transform(m);
			RenderBounds(aabb);
		}
	}
}
