/*=============================================================================
World.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "World.h"
#include "SampleStealth.h"

String MakeMGSPath(const String& fileName)
{
	return Path::Combine("AI\\MGS", fileName);
}

Quaternion MakeOrientation(const Vector3& v)
{
	return (Quaternion::FromAxisAngle(Vector3::UnitX, Math::ToRadians(v.X)) *
		Quaternion::FromAxisAngle(Vector3::UnitY, Math::ToRadians(v.Y)) *
		Quaternion::FromAxisAngle(Vector3::UnitZ, Math::ToRadians(v.Z)));
}

ShaderMaterial* CreateTextureShader(Texture* texture, const Color32& color)
{
	ShaderMaterial* shader = new Shader();
	ShaderTechnique* technique = new ShaderTechnique();

	ShaderPass* pass0 = new ShaderPass();
	pass0->RasterizerState.FillMode = FillMode_Solid;
	//pass0->LightState.Lighting = true;

	SamplerState* sampler0 = new SamplerState();
	pass0->AddSamplerState(sampler0);
	TextureState* texture0 = new TextureState();
	texture0->SetTexture(texture);

	texture0->ColorOperation = TextureOperation_Modulate;
	texture0->ColorArgument1 = TextureArgument_textureColor;
	texture0->ColorArgument2 = TextureArgument_Constant;
	texture0->ConstantColor = color;

	texture0->AlphaOperation = TextureOperation_Modulate;
	texture0->AlphaArgument1 = TextureArgument_textureColor;
	texture0->AlphaArgument2 = TextureArgument_Constant;

	pass0->AddTextureState(texture0);
	technique->AddPass(pass0);

	shader->AddTechnique(technique);

	return shader;
}

PathNode::PathNode()
{
	_Position = Vector2::Zero;
}


MapNode::MapNode()
{
	_Position = Vector2::Zero;
}


ActorPath::ActorPath()
{
	_currentNode = 0;
}

int ActorPath::GetCurrentPathNodeIndex() const
{
	return _currentNode;
}

PathNode* ActorPath::GetCurrentPathNode()
{
	return (_currentNode < _PathNodes.Count()) ? &(_PathNodes[_currentNode]) : NULL;
}

PathNode* ActorPath::GetPathNode(int index)
{
	return (index < _PathNodes.Count() ) ? &(_PathNodes[index]) : NULL;
}

int ActorPath::GetPathLength() const
{
	return _PathNodes.Count();
}

bool ActorPath::IsPathComplete() const
{
	return (_currentNode >= _PathNodes.Count());
}

void ActorPath::ClearPath()
{
	_PathNodes.Clear();
	_currentNode = 0;
}

void ActorPath::AddPathNode(const PathNode& node)
{
	//FIX
	if (_PathNodes.Count() == 0)
		_PathNodes.Add(node);
	else
		_PathNodes.Insert(0, node);
}

void ActorPath::AdvancePathNode()
{
	if (_currentNode < GetPathLength())
	{
		_currentNode++;
	}
}


SE_IMPLEMENT_ABSTRACT(GameEntity);
SE_IMPLEMENT_REFLECTION(GameEntity);

GameEntity::GameEntity() :
	super()
{
	_Design = false;
}


SE_IMPLEMENT_CLASS(Tile);
SE_IMPLEMENT_REFLECTION(Tile);

Tile::Tile() :
	RefObject()
{
	_mesh = NULL;
}

void Tile::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);
}

void Tile::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	Texture* texture = CreateTexture(MakeMGSPath(_fileName));

	ShapeDescription shapeDesc;
	shapeDesc.TextureCoordinates = true;

	BoxShape* boxShape = new BoxShape(BoundingBox(Vector3(-0.5), Vector3(0.5)));
	boxShape->SetDivisions(1);
	boxShape->CreateMesh(&shapeDesc);
	_mesh = boxShape->GetMesh();
	_mesh->SetShader(CreateTextureShader(texture, _Color));
	//delete boxShape;
}

SE_IMPLEMENT_CLASS(ActorDef);
SE_IMPLEMENT_REFLECTION(ActorDef);

ActorDef::ActorDef() :
	RefObject()
{
	_Model = NULL;
	_Offset = Vector3::Zero;
	_Scale = 1.0f;
	_Orientation = Vector3::Zero;
}

void ActorDef::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);
}

void ActorDef::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	_Model = CreateModel(MakeMGSPath(_fileName));

	if (_Model != NULL && _Model->GetMeshFrameCount() > 0)
	{
		Transform t(_Offset, _Scale, MakeOrientation(_Orientation));
		_Model->GetMeshFrameByIndex(0)->SetTransform(t);
		_Model->GetMeshFrameByIndex(0)->UpdateBoundingBox();
		_Model->UpdateBoundingBox();
	}
}

SE_IMPLEMENT_CLASS(Tileset);
SE_IMPLEMENT_REFLECTION(Tileset);

Tileset::Tileset() :
	RefObject()
{
}

Tile* Tileset::GetTileOfType(TileType type) const
{
	Tile* tile;
	foreach (tile, _Tiles, TileList)
	{
		if (tile->_TileType == type)
			return tile;
	}

	return NULL;
}

void Tileset::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();

	{
		XMLElement* array = document->CreateElement("Tiles");
		int count = _Tiles.Count();
		array->SetAttribute("Count", String::ToString(count));
		for (int i=0; i<count; i++)
		{
			XMLElement* item = document->CreateElement("Tile");
			context->Serialize(document, item, _Tiles.GetItem(i), _Tiles.GetItem(i)->GetType());
			array->AppendChild(item);
		}
		element->AppendChild(array);
	}

	{
		XMLElement* array = document->CreateElement("ActorDefs");
		int count = _ActorDefs.Count();
		array->SetAttribute("Count", String::ToString(count));
		for (int i=0; i<count; i++)
		{
			XMLElement* item = document->CreateElement("ActorDef");
			context->Serialize(document, item, _ActorDefs.GetItem(i), _ActorDefs.GetItem(i)->GetType());
			array->AppendChild(item);
		}
		element->AppendChild(array);
	}
}

void Tileset::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	{
		XMLElement* array = (XMLElement*)element->GetItem("Tiles");
		int count = array->GetAttribute("Count").ToInt32();
		XMLElement* item = (XMLElement*)array->GetItem("Tile");
		for (int i=0; i<count; i++)
		{
			XMLElement* type = (XMLElement*)item->GetFirstChild();
			Tile* tile = (Tile*)context->Deserialize(item->GetOwnerDocument(), type);
			if (tile != NULL)
				_Tiles.Add(tile);
			item = (XMLElement*)item->GetNextSibling();
		}
	}

	{
		XMLElement* array = (XMLElement*)element->GetItem("ActorDefs");
		int count = array->GetAttribute("Count").ToInt32();
		XMLElement* item = (XMLElement*)array->GetItem("ActorDef");
		for (int i=0; i<count; i++)
		{
			XMLElement* type = (XMLElement*)item->GetFirstChild();
			ActorDef* actordef = (ActorDef*)context->Deserialize(item->GetOwnerDocument(), type);
			if (actordef != NULL)
				_ActorDefs.Add(actordef);
			item = (XMLElement*)item->GetNextSibling();
		}
	}
}

SE_IMPLEMENT_CLASS(Cell);
SE_IMPLEMENT_REFLECTION(Cell);

Cell::Cell() :
	RefObject()
{
	_Tile = NULL;
	_X = 0;
	_Y = 0;
	_Depth = 1;
}

void Cell::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();

	XMLElement* tile = document->CreateElement("Tile");
	int tile_idx = GameCore::Instance()->GetWorld()->GetTileset()->_Tiles.IndexOf(_Tile);
	XMLText* tile_value = document->CreateTextNode(String::ToString(tile_idx));
	tile->AppendChild(tile_value);
	element->AppendChild(tile);
}

void Cell::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	XMLElement* tile = (XMLElement*)element->GetItem("Tile");
	XMLText* tile_value = (XMLText*)tile->GetFirstChild();
	int tile_idx = tile_value->GetValue().ToInt32();
	_Tile = GameCore::Instance()->GetWorld()->GetTileset()->_Tiles[tile_idx];
}


SE_IMPLEMENT_CLASS(Actor);
SE_IMPLEMENT_REFLECTION(Actor);

Actor::Actor() :
	super()
{
	_ActorDef = NULL;
	_RotationSpeed = 2.0f;
	_Position = Vector3::Zero;
	_Orientation = Quaternion::Identity;

	_Moving = false;
	_MovePosition = Vector3::Zero;

	_Rotating = false;
	_StartRotation = Quaternion::Identity;
	_TargetRotation = Quaternion::Identity;
}

void Actor::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();

	XMLElement* actordef = document->CreateElement("ActorDef");
	int actordef_idx = GameCore::Instance()->GetWorld()->GetTileset()->_ActorDefs.IndexOf(_ActorDef);
	XMLText* actordef_value = document->CreateTextNode(String::ToString(actordef_idx));
	actordef->AppendChild(actordef_value);
	element->AppendChild(actordef);
}

void Actor::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	XMLElement* actordef = (XMLElement*)element->GetItem("ActorDef");
	XMLText* actordef_value = (XMLText*)actordef->GetFirstChild();
	int actordef_idx = actordef_value->GetValue().ToInt32();
	_ActorDef = GameCore::Instance()->GetWorld()->GetTileset()->_ActorDefs[actordef_idx];
}

void Actor::Move(const Vector3& pos)
{
	_MovePosition = pos;
	_Moving = true;
}

void Actor::LookAt(const Vector3& pos)
{
	Vector3 dir = Vector3::Normalize(pos - _Position);
	if (Math::Equals(dir.Length(), 0.0f))
	{
		// Facing the same position
		return;
	}

	real32 angle = Math::Atan2(dir.X, dir.Y);
	Vector3 forward = _Orientation.Rotate(Vector3::UnitY);
	real32 angle2 = Math::Atan2(forward.X, forward.Y);

	if (!Math::Equals(angle2 - angle, 0.0f))
	{
		_StartRotation = _Orientation;
		_TargetRotation = _Orientation * Quaternion::FromAxisAngle(Vector3::UnitZ, angle2 - angle);
		_Rotating = true;
	}

	/*Vector3 forward = _Orientation * Vector3::YAxis;
	real32 angle = Vector3::Angle(dir, forward);
	if (!Math::Equals(angle, 0.0f))
	{
		_StartRotation = _Orientation;
		_TargetRotation = _Orientation * Quaternion::FromAxisAngle(Vector3::ZAxis, angle);
		_Rotating = true;
	}*/
}

void Actor::Update(real64 elapsed)
{
	if (_Moving)
	{
		_Position = _MovePosition;
		_Moving = false;
	}

	if (_Rotating)
	{
		/*if (Math::Equals((_TargetRotation - _Orientation).Length(), 0.0f))
		{
			_Orientation = _TargetRotation;
			_Rotating = false;
		}
		else
		{
			//_Orientation.Slerp(_RotationSpeed * elapsed, _StartRotation, _TargetRotation);
			real32 angle = 2.0f * Math::PI / _RotationSpeed * elapsed;
			_Orientation *= Quaternion::FromAxisAngle(Vector3::ZAxis, angle);
		}*/

		//FIX
		_Orientation = _TargetRotation;
		_Rotating = false;
	}
}

SE_IMPLEMENT_CLASS(Map);
SE_IMPLEMENT_REFLECTION(Map);

Map::Map() :
	RefObject()
{
	_CellSize = 64;
	_Divisions = DefaultMapDivisions;
	_Width = 0;
	_Height = 0;
}

void Map::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();

	{
		XMLElement* array = document->CreateElement("Cells");
		int count = _Cells.Count();
		array->SetAttribute("Count", String::ToString(count));
		for (int i=0; i<count; i++)
		{
			XMLElement* item = document->CreateElement("Cell");
			context->Serialize(document, item, _Cells.GetItem(i), _Cells.GetItem(i)->GetType());
			array->AppendChild(item);
		}
		element->AppendChild(array);
	}

	{
		XMLElement* array = document->CreateElement("Actors");
		int count = _Actors.Count();
		array->SetAttribute("Count", String::ToString(count));
		for (int i=0; i<count; i++)
		{
			XMLElement* item = document->CreateElement("Actor");
			context->Serialize(document, item, _Actors.GetItem(i), _Actors.GetItem(i)->GetType());
			array->AppendChild(item);
		}
		element->AppendChild(array);
	}
}

void Map::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	{
		XMLElement* array = (XMLElement*)element->GetItem("Cells");
		if (array != NULL)
		{
			int count = array->GetAttribute("Count").ToInt32();
			XMLElement* item = (XMLElement*)array->GetItem("Cell");
			for (int i=0; i<count; i++)
			{
				XMLElement* type = (XMLElement*)item->GetFirstChild();
				Cell* cell = (Cell*)context->Deserialize(item->GetOwnerDocument(), type);
				if (cell != NULL)
					_Cells.Add(cell);
				item = (XMLElement*)item->GetNextSibling();
			}
		}
	}

	{
		XMLElement* array = (XMLElement*)element->GetItem("Actors");
		if (array != NULL)
		{
			int count = array->GetAttribute("Count").ToInt32();
			XMLElement* item = (XMLElement*)array->GetItem("Actor");
			for (int i=0; i<count; i++)
			{
				XMLElement* type = (XMLElement*)item->GetFirstChild();
				Actor* actor = (Actor*)context->Deserialize(item->GetOwnerDocument(), type);
				if (actor != NULL)
					AddActor(actor);
				item = (XMLElement*)item->GetNextSibling();
			}
		}
	}

	Validate();
}

void Map::Validate()
{
	int actorIndex;
	int actorCount = _Actors.Count();

	for (actorIndex=0; actorIndex<actorCount; actorIndex++)
	{
		Actor* actor = _Actors[actorIndex];
	}
}

int Map::GetCellIndex(const Vector2& value) const
{
	return ((int32)value.Y * _Width + (int32)value.X);
}

void Map::SetCellTileAt(Tile* tile, const Vector2& value)
{
	int index = GetCellIndex(value);
	_Cells[index]->_Tile = tile;
}

Cell* Map::GetActorCell(Actor* actor) const
{
	int index = GetCellIndex(WorldToCell(Vector2(actor->_Position.X, actor->_Position.Y)));
	return _Cells[index];
}

int Map::GetActorDepth(Actor* actor) const
{
	int index = GetCellIndex(WorldToCell(Vector2(actor->_Position.X, actor->_Position.Y)));
	return _Cells[index]->_Depth;
}

bool Map::HasActorAt(const Vector2& value) const
{
	int index = GetActorIndex(value);
	return (index != -1);
}

Actor* Map::GetActorOfType(ActorType type) const
{
	int count = _Actors.Count();
	for (int i=0; i<count; i++)
	{
		Actor* actor = _Actors[i];
		if (actor->_ActorDef->_ActorType == type)
		{
			return actor;
		}
	}

	return NULL;
}

Actor* Map::GetActorAt(const Vector2& value) const
{
	int index = GetActorIndex(value);
	if (index != -1)
		return _Actors[index];
	else
		return NULL;
}

Actor* Map::GetActorByName(const String& name) const
{
	int count = _Actors.Count();
	for (int i=0; i<count; i++)
	{
		Actor* actor = _Actors[i];
		if (actor->_name == name)
			return actor;
	}

	return NULL;
}

int Map::GetActorIndex(const Vector2& value) const
{
	int count = _Actors.Count();
	for (int i=0; i<count; i++)
	{
		Actor* actor = _Actors[i];
		Vector3 pos = WorldToCell(actor->_Position);
		if (pos.X == value.X && pos.Y == value.Y)
		{
			return i;
		}
	}

	return -1;
}

String Map::GetUniqueName(ActorType type) const
{
	String typeName = EnumObject::GetName(typeof(ActorType), type);
	String name;
	int count = _Actors.Count();
	int j = 0;
	bool found = true;
	while (found)
	{
		found = false;
		name = typeName + String::ToString(j);
		for (int i=0; i<count; i++)
		{
			Actor* actor = _Actors[i];

			if (actor->_name == name)
			{
				j++;
				found = true;
			}
		}
	}

	return name;
}

void Map::AddActor(Actor* actor)
{
	// Check if the actor already exists
	if (_Actors.Contains(actor))
		return;

	if (actor->_ActorDef->_ActorType == ActorType_Spawn)
	{
		Actor* prev = GetActorOfType(ActorType_Spawn);
		if (prev != NULL)
		{
			_Actors.Remove(prev);
			GameCore::Instance()->GetGameUI()->RemoveEntity(prev);
			delete prev;
		}
	}

	if (actor->_ActorDef->_ActorType == ActorType_Goal)
	{
		Actor* prev = GetActorOfType(ActorType_Goal);
		if (prev != NULL)
		{
			_Actors.Remove(prev);
			GameCore::Instance()->GetGameUI()->RemoveEntity(prev);
			delete prev;
		}
	}

	// Add the new actor
	_Actors.Add(actor);

	Validate();
}

void Map::SetActorPosition(Actor* actor, const Vector2& pos)
{
	Vector2 p = WorldToCell(pos);
	if (p.X < 0 || p.Y < 0 || p.X > _Width || p.Y > _Height)
		return;

	// If the depth of the new cell is higher than the current one, can't move there
	Cell* actorCell = GetActorCell(actor);
	Cell* cell = GetCellAt(p);

	bool valid = true;
	if (actorCell->_Tile->_TileType != TileType_Tunnel && cell->_Tile->_TileType != TileType_Tunnel)
	{
		// Common case
		if (cell->_Depth > actorCell->_Depth)
			valid = false;
	}
	else if (actorCell->_Tile->_TileType == TileType_Tunnel && cell->_Tile->_TileType == TileType_Tunnel)
	{
		// Moving in a tunnel
		if (cell->_Depth != actorCell->_Depth)
			valid = false;
	}
	else if (cell->_Tile->_TileType == TileType_Tunnel)
	{
		// Going in a tunnel
		if ((cell->_Depth-2) != actorCell->_Depth)
			valid = false;
	}
	else if (actorCell->_Tile->_TileType == TileType_Tunnel)
	{
		// Going out a tunnel
		if (cell->_Depth != (actorCell->_Depth-2))
			valid = false;
	}

	if (valid)
	{
		actor->_Position.X = pos.X;
		actor->_Position.Y = pos.Y;

		if (cell->_Tile->_TileType == TileType_Tunnel)
			actor->_Position.Z = Math::Max(1, (cell->_Depth-2)) * _CellSize;
		else
			actor->_Position.Z = cell->_Depth * _CellSize;
	}
}

Vector2 Map::WorldToCell(const Vector2& value) const
{
	return Vector2(value.X / _CellSize, value.Y / _CellSize);
}

Vector2 Map::CellToWorld(const Vector2& value) const
{
	return Vector2(value.X * _CellSize + _CellSize/2, value.Y * _CellSize + _CellSize/2);
}

Vector3 Map::WorldToCell(const Vector3& value) const
{
	return Vector3(value.X / _CellSize, value.Y / _CellSize, value.Z / _CellSize);
}

Vector3 Map::CellToWorld(const Vector3& value) const
{
	return Vector3(value.X * _CellSize + _CellSize/2, value.Y * _CellSize + _CellSize/2, value.Z * _CellSize);
}

int Map::GetMapNodeIndexAt(const Vector2& value) const
{
	if (_MapNodes.IsEmpty())
		return -1;

	real32 divSize = _CellSize / _Divisions;
	int32 divWidth = _Width * _Divisions;
	int32 divHeight = _Height * _Divisions;
	return ((int32)(value.Y / divSize) * divWidth) + (int32)(value.X / divSize);
}

MapNode* Map::GetMapNodeAt(const Vector2& value) const
{
	int index = GetMapNodeIndexAt(value);
	if (index < -1 || index >= _MapNodes.Count())
		return NULL;
	else
		return _MapNodes[index];
}

void Map::Initialize()
{
	int count;
	int i;
	real32 x, y;

	count = _MapNodes.Count();
	for (i=0; i<count; i++)
	{
		delete _MapNodes[i];
	}
	_MapNodes.Clear();

	// Discretize
	real32 divWidth = _Width * _Divisions;
	real32 divHeight = _Height * _Divisions;
	real32 divSize = _CellSize / _Divisions;
	real32 halfDivSize = divSize / 2.0f;
	Vector2 pos;

	for (y=0; y<divHeight; y++)
	{
		for (x=0; x<divWidth; x++)
		{
			MapNode* node = new MapNode();
			node->_Position = Vector2(x*divSize, y*divSize);
			_MapNodes.Add(node);
		}
	}
}

void Map::Update(real64 elapsed)
{
	int count = _Actors.Count();
	for (int i=0; i<count; i++)
	{
		_Actors[i]->Update(elapsed);
	}
}

void Map::Draw()
{
	int i, j;

	RenderSystem* renderer = RenderSystem::Current();
	Tileset* tileset = GameCore::Instance()->GetWorld()->GetTileset();

	// Draw tiles
	int cellCount = _Cells.Count();
	for (i=0; i<cellCount; i++)
	{
		Cell* cell = _Cells[i];

		for (j=0; j<cell->_Depth; j++)
		{
			Mesh* mesh = cell->_Tile->_mesh;
			ShaderPass* pass = (ShaderPass*)mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(0);
			pass->GetTextureStateByIndex(0)->ConstantColor.a = 1.0f;
			pass->AlphaState.BlendEnable[0] = false;

			if (cell->_Tile->_TileType == TileType_Tunnel)
			{
				if (j == cell->_Depth - 1 || j == cell->_Depth - 2)
					continue;
				else if (j == cell->_Depth - 3)
					mesh = tileset->GetTileOfType(TileType_Floor)->_mesh;
			}

			Matrix4 m = Matrix4(Matrix3::Identity,
				Vector3(0.5+cell->_X, 0.5+cell->_Y, 0.5+j));
			m.Scale(Vector3::One * _CellSize);
			renderer->SetWorldTransform(m);
			renderer->RenderMesh(mesh);
		}
	}

	static ModelNode* node = new ModelNode();

	// Draw actors
	int actorCount = _Actors.Count();
	for (i=0; i<actorCount; i++)
	{
		Actor* actor = _Actors[i];

		if (actor->_Design && !GameCore::Instance()->GetWorld()->GetEditor())
			continue;

		node->SetLocalPosition(actor->_Position);
		node->SetLocalOrientation(actor->_Orientation);

		node->SetModel(actor->_ActorDef->_Model);
		node->Render();
	}

	// Draw transparent tiles
	cellCount = _Cells.Count();
	for (i=0; i<cellCount; i++)
	{
		Cell* cell = _Cells[i];

		for (j=0; j<cell->_Depth; j++)
		{
			Mesh* mesh = cell->_Tile->_mesh;
			ShaderPass* pass = (ShaderPass*)mesh->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(0);
			pass->GetTextureStateByIndex(0)->ConstantColor.a = 1.0f;
			pass->AlphaState.BlendEnable[0] = false;

			if (cell->_Tile->_TileType == TileType_Tunnel&& j == cell->_Depth - 1)
			{
				if (GameCore::Instance()->GetWorld()->GetPlayer() != NULL &&
					cell == GetActorCell(GameCore::Instance()->GetWorld()->GetPlayer()->GetCharacter()))
				{
					pass->GetTextureStateByIndex(0)->ConstantColor.a = 0.5f;
					pass->AlphaState.BlendEnable[0] = true;
					pass->AlphaState.SourceBlend = BlendMode_SourceAlpha;
					pass->AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
				}

				Matrix4 m = Matrix4(Matrix3::Identity,
					Vector3(0.5+cell->_X, 0.5+cell->_Y, 0.5+j));
				m.Scale(Vector3::One * _CellSize);
				renderer->SetWorldTransform(m);
				renderer->RenderMesh(mesh);
			}
		}
	}
}
