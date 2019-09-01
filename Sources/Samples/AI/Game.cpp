/*=============================================================================
Game.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "SampleAI.h"
#include "Game.h"
#include "Utils.h"

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

ShaderMaterial* CreateMapShader()
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
	pass0->AddTextureState(texture0);
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

SE_IMPLEMENT_CLASS(GameEntity);
SE_IMPLEMENT_REFLECTION(GameEntity);

GameEntity::GameEntity() :
	RefObject()
{
}

GameEntity::GameEntity(GameWorld* world) :
	RefObject(),
	_GameWorld(world)
{
}

void GameEntity::Create(Mesh* mesh)
{
	_mesh = mesh;

	MeshFrame* frame = new MeshFrame();
	frame->AddMeshLOD(0.0f, _mesh);

	Model* model = new Model();
	model->AddMeshFrame(frame);

	_Node = new ModelNode();
	_Node->SetModel(model);
}

GameWorld::GameWorld() :
	_Scene(NULL)
{
	_Paused = false;
	_ControlledEntity = NULL;

	_Map = NULL;
	_Tileset = NULL;
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
	}
}

void GameWorld::NewMap(int32 width, int32 height)
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
			_Map->_Cells.Add(cell);
		}
	}
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

int GameWorld::GetEntityCount() const
{
	return _Entities.Count();
}

void GameWorld::AddEntity(GameEntity* entity)
{
	_Scene->GetRoot()->AddChild(entity->GetNode());
	_Entities.Add(entity);

	if (_ControlledEntity == NULL)
		_ControlledEntity = entity;
}

void GameWorld::RemoveEntity(GameEntity* entity)
{
	_Scene->GetRoot()->RemoveChild(entity->GetNode());
	_Entities.Remove(entity);
}

void GameWorld::Create()
{
	Camera* camera = new Camera();
	camera->SetPerspective(45.0f, (real32)
		GameCore::Instance()->GetMainWindow()->GetClientWidth() /
		GameCore::Instance()->GetMainWindow()->GetClientHeight(),
		1.0f, 5000.0f);
	camera->SetLocalPosition(Vector3(0, 0, 512));
	camera->LookAt(Vector3(0, 0, 0));

	_camera = camera;
	RenderSystem::Current()->SetCamera(_camera);

	_Scene = new Scene();
	_Scene->SetAmbientColor(Color32::Black);
	_Scene->SetRoot(new SceneNode());

	_Editor = false;
	_Cursor = PointInt::Empty;

	_SelectedUnit = NULL;
	_ActionRequested = false;
}

void GameWorld::SetCursor(int32 x, int32 y)
{
	_Cursor.X = Math::Clamp(x, 0, _Map->_Width-1);
	_Cursor.Y = Math::Clamp(y, 0, _Map->_Height-1);

	Vector3 position = _camera->GetLocalPosition();
	position.X = _Cursor.X * _Map->_CellWidth;
	position.Y = _Cursor.Y * _Map->_CellHeight;
	_camera->SetLocalPosition(position);
}

void GameWorld::SetEditor(bool value)
{
	_Editor = value;

	GameCore::Instance()->GetGameUI()->SetEditor(_Editor);
}

void GameWorld::UpdateCamera()
{
	InputKeyboard* keyboard = (InputKeyboard*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Keyboard);
	InputMouse* mouse = (InputMouse*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Mouse);

	if (keyboard->IsKeyDown(Key_PageUp))
		_camera->Translate(Vector3::UnitZ*10);

	if (keyboard->IsKeyDown(Key_PageDown))
		_camera->Translate(-Vector3::UnitZ*10);

	/*if (keyboard->IsKeyDown(Key_Up))
		_camera->Translate(Vector3::UnitY*10);

	if (keyboard->IsKeyDown(Key_Down))
		_camera->Translate(-Vector3::UnitY*10);

	if (keyboard->IsKeyDown(Key_Left))
		_camera->Translate(-Vector3::UnitX*10);

	if (keyboard->IsKeyDown(Key_Right))
		_camera->Translate(Vector3::UnitX*10);*/
}

void GameWorld::SetControlledEntity(GameEntity* entity)
{
	if (_ControlledEntity != NULL)
	{
		Mesh* mesh = _ControlledEntity->GetMesh();
		mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(1)->MaterialState.DiffuseColor = Color32::Black;
	}

	_ControlledEntity = entity;

	if (_ControlledEntity != NULL)
	{
		Mesh* mesh = _ControlledEntity->GetMesh();
		mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(1)->MaterialState.DiffuseColor = Color32::Blue;
	}
}

void GameWorld::Update(real64 elapsed)
{
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
		if (keyboard->IsKeyDown(Key_Up))
			SetCursor(_Cursor.X, _Cursor.Y+1);

		if (keyboard->IsKeyDown(Key_Down))
			SetCursor(_Cursor.X, _Cursor.Y-1);

		if (keyboard->IsKeyDown(Key_Left))
			SetCursor(_Cursor.X-1, _Cursor.Y);

		if (keyboard->IsKeyDown(Key_Right))
			SetCursor(_Cursor.X+1, _Cursor.Y);
	}

	if (keyboard->IsKeyDown(Key_E) && keyboard->IsKeyToggled(Key_E))
	{
		SetEditor(!_Editor);
	}

	if (_Editor)
	{
		if (keyboard->IsKeyDown(Key_Delete))
		{
			// Check if there is a unit at that location
			int unit_idx = _Map->GetUnitIndex(_Cursor.X, _Cursor.Y);
			if (unit_idx != -1)
			{
				// Remove it
				Unit* unit = _Map->_Units[unit_idx];
				_Map->_Units.RemoveAt(unit_idx);
				delete unit;
			}
		}
	}
	else
	{
		if (keyboard->IsKeyDown(Key_Z) && keyboard->IsKeyToggled(Key_Z))
		{
			// Check if there is a unit at that location
			int unit_idx = _Map->GetUnitIndex(_Cursor.X, _Cursor.Y);

			_ActionRequested = true;
			if (!_ActionRequested)
			{
				if (unit_idx != -1)
				{
					if (_SelectedUnit != NULL)
					{
						_SelectedUnit->SetDestination(NULL);
					}
					_SelectedUnit = _Map->_Units[unit_idx];
				}
				else
				{
					if (_SelectedUnit != NULL)
					{
						Cell* cell = _Map->_Cells[_Map->GetCellIndex(_Cursor.X, _Cursor.Y)];
						_SelectedUnit->SetDestination(cell);
						_ActionRequested = true;
					}
				}
			}
			else
			{
				if (unit_idx != -1)
				{
					if (_SelectedUnit != NULL)
					{
						_SelectedUnit->SetDestination(NULL);
					}
					_SelectedUnit = _Map->_Units[unit_idx];
				}
				else
				{
					if (_SelectedUnit != NULL)
					{
						Cell* cell = _Map->_Cells[_Map->GetCellIndex(_Cursor.X, _Cursor.Y)];
						_SelectedUnit->SetDestination(cell);
						_SelectedUnit->MoveToDestination();
					}
				}
			}
		}

		if (keyboard->IsKeyDown(Key_X) && keyboard->IsKeyToggled(Key_X))
		{
			if (_ActionRequested)
			{
				_ActionRequested = false;
			}
		}
	}

	if (keyboard->IsKeyDown(Key_P) && keyboard->IsKeyToggled(Key_P))
	{
		_Paused = !_Paused;
	}

	if (!_Paused)
	{
	}
/*
	count = _Entities.Count();
	for (i=0; i<count; i++)
	{
		GameEntity* entity = _Entities[i];
		ModelNode* node = entity->GetNode();
		Physics::IBody* body = entity->GetPhysicsBody();

		node->SetLocalPosition(body->GetPosition());
		node->SetLocalOrientation(body->GetOrientation());
	}
*/
	if (_Scene != NULL)
		_Scene->Update(elapsed);

	_Map->Update();
}

void GameWorld::Render()
{
	if (_Scene != NULL)
		_Scene->Render();

	if (_Map != NULL)
	{
		_Map->Draw();
	}

	InputMouse* mouse = (InputMouse*)InputSystem::Current()->
		GetInputDeviceByType(InputDeviceType_Mouse);
	int32 mouseX, mouseY;
	mouse->GetMousePosition(mouseX, mouseY);

	Vector3 cameraPos = _camera->GetWorldPosition();

	Pen pen;
	pen.Color = Color32::Red;
	pen.Width = 2;
	RenderSystem::Current()->DrawRectangle(&pen,
		_Cursor.X * _Map->_CellWidth, _Cursor.Y * _Map->_CellHeight,
		(_Cursor.X+1) * _Map->_CellWidth, (_Cursor.Y-1) * _Map->_CellHeight);

	if (_SelectedUnit != NULL)
	{
		pen.Color = Color32::Blue;
		pen.Width = 2;
		RenderSystem::Current()->DrawRectangle(&pen,
			_SelectedUnit->_X * _Map->_CellWidth-2, _SelectedUnit->_Y * _Map->_CellHeight-2,
			(_SelectedUnit->_X+1) * _Map->_CellWidth+2, (_SelectedUnit->_Y-1) * _Map->_CellHeight+2);
	}
}
