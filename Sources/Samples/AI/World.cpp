/*=============================================================================
World.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "World.h"
#include "SampleAI.h"
#include "Utils.h"
#include "UnitPathManager.h"

String MakeAWPath(const String& fileName)
{
	return Path::Combine("AI\\AW", fileName);
}

PathNode::PathNode()
{
	_X = 0;
	_Y = 0;
}


UnitPath::UnitPath()
{
	_currentNode = 0;
}

int UnitPath::GetCurrentPathNodeIndex() const
{
	return _currentNode;
}

PathNode* UnitPath::GetCurrentPathNode()
{
	return (_currentNode < _PathNodes.Count()) ? &(_PathNodes[_currentNode]) : NULL;
}

PathNode* UnitPath::GetPathNode(int index)
{
	return (index < _PathNodes.Count() ) ? &(_PathNodes[index]) : NULL;
}

int UnitPath::GetPathLength() const
{
	return _PathNodes.Count();
}

bool UnitPath::IsPathComplete() const
{
	return (_currentNode >= _PathNodes.Count());
}

void UnitPath::ClearPath()
{
	_PathNodes.Clear();
	_currentNode = 0;
}

void UnitPath::AddPathNode(const PathNode& node)
{
	//FIX
	if (_PathNodes.Count() == 0)
		_PathNodes.Add(node);
	else
		_PathNodes.Insert(0, node);
}

void UnitPath::AdvancePathNode()
{
	if (_currentNode < GetPathLength())
	{
		_currentNode++;
	}
}


MovementController::MovementController()
{
	_Status = MovementControllerStatus_Inactive;
	_Speed = 400.0f;
	_Position = PointInt::Empty;
	_WorldPosition = Vector2::Zero;
}

void MovementController::Update(real64 elapsed)
{
	static real64 lastMove = 0.0;
	lastMove += elapsed;

	if (_Status == MovementControllerStatus_Active)
	{
		if (lastMove >= 0.01)
		{
			lastMove = 0.0;

			Map* map = GameCore::Instance()->GetWorld()->GetMap();
			if (_Path.IsPathComplete())
			{
				_Status = MovementControllerStatus_Complete;
				_WorldPosition.X = _Position.X * map->_CellWidth;
				_WorldPosition.Y = _Position.Y * map->_CellHeight;
			}
			else
			{
				PathNode* node = _Path.GetCurrentPathNode();
				if (node->_X > _Position.X && _WorldPosition.X < node->_X * map->_CellWidth)
				{
					_WorldPosition.X += elapsed * _Speed;
				}
				else if (node->_X < _Position.X && _WorldPosition.X > node->_X * map->_CellWidth)
				{
					_WorldPosition.X -= elapsed * _Speed;
				}
				else if (node->_Y > _Position.Y && _WorldPosition.Y < node->_Y * map->_CellHeight)
				{
					_WorldPosition.Y += elapsed * _Speed;
				}
				else if (node->_Y < _Position.Y && _WorldPosition.Y > node->_Y * map->_CellHeight)
				{
					_WorldPosition.Y -= elapsed * _Speed;
				}
				else // scrolling complete
				{
					_Position.X = node->_X;
					_Position.Y = node->_Y;
					_Path.AdvancePathNode();
				}
			}
		}
	}
}


SE_IMPLEMENT_ABSTRACT(MapEntity);

SE_IMPLEMENT_CLASS(Tile);
SE_IMPLEMENT_REFLECTION(Tile);

Tile::Tile() :
	RefObject()
{
	_Sprite = NULL;
}

void Tile::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);
}

void Tile::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	_Sprite = CreateSprite(MakeAWPath(_fileName));
}

SE_IMPLEMENT_CLASS(UnitDef);
SE_IMPLEMENT_REFLECTION(UnitDef);

UnitDef::UnitDef() :
	RefObject()
{
	_Sprite = NULL;
}

void UnitDef::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);
}

void UnitDef::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	_Sprite = CreateSprite(MakeAWPath(_fileName));

	if (_Sprite != NULL)
	{
		SpriteFrame* frame = _Sprite->GetSpriteFrame(0);
		int32 height = frame->GetSourceRect().Height;
		frame->SetSourceRect(RectangleInt(0, 0, height, height));

		AlphaState alphaState;
		alphaState.BlendEnable[0] = true;
		alphaState.SourceBlend = BlendMode_SourceAlpha;
		alphaState.DestinationBlend = BlendMode_InvSourceAlpha;
		_Sprite->SetAlphaState(alphaState);
	}
}

SE_IMPLEMENT_CLASS(Tileset);
SE_IMPLEMENT_REFLECTION(Tileset);

Tileset::Tileset() :
	RefObject()
{
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
		XMLElement* array = document->CreateElement("UnitDefs");
		int count = _UnitDefs.Count();
		array->SetAttribute("Count", String::ToString(count));
		for (int i=0; i<count; i++)
		{
			XMLElement* item = document->CreateElement("UnitDef");
			context->Serialize(document, item, _UnitDefs.GetItem(i), _UnitDefs.GetItem(i)->GetType());
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
		XMLElement* array = (XMLElement*)element->GetItem("UnitDefs");
		int count = array->GetAttribute("Count").ToInt32();
		XMLElement* item = (XMLElement*)array->GetItem("UnitDef");
		for (int i=0; i<count; i++)
		{
			XMLElement* type = (XMLElement*)item->GetFirstChild();
			UnitDef* unitdef = (UnitDef*)context->Deserialize(item->GetOwnerDocument(), type);
			if (unitdef != NULL)
				_UnitDefs.Add(unitdef);
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

SE_IMPLEMENT_CLASS(Unit);
SE_IMPLEMENT_REFLECTION(Unit);

Unit::Unit() :
	MapEntity()
{
	_UnitState = UnitState_Idle;

	_UnitDef = NULL;
	_X = 0;
	_Y = 0;
	_Direction = UnitDirection_West;

	_Destination = NULL;
}

void Unit::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();

	XMLElement* unitdef = document->CreateElement("UnitDef");
	int unitdef_idx = GameCore::Instance()->GetWorld()->GetTileset()->_UnitDefs.IndexOf(_UnitDef);
	XMLText* unitdef_value = document->CreateTextNode(String::ToString(unitdef_idx));
	unitdef->AppendChild(unitdef_value);
	element->AppendChild(unitdef);
}

void Unit::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	XMLElement* unitdef = (XMLElement*)element->GetItem("UnitDef");
	XMLText* unitdef_value = (XMLText*)unitdef->GetFirstChild();
	int unitdef_idx = unitdef_value->GetValue().ToInt32();
	_UnitDef = GameCore::Instance()->GetWorld()->GetTileset()->_UnitDefs[unitdef_idx];
}

void Unit::SetPosition(int32 x, int32 y)
{
	_X = x;
	_Y = y;
}

void Unit::Update()
{
	real64 elapsed = Application::Instance()->GetElapsedTime();

	switch (_UnitState)
	{
	case UnitState_Idle:
		break;

	case UnitState_Moving:
		_MovementController.Update(elapsed);
		SetPosition(_MovementController._Position.X,
			_MovementController._Position.Y);

		if (_MovementController._Status == MovementControllerStatus_Complete)
		{
			_UnitState = UnitState_Idle;
		}
		else
		{
			PathNode* node = _MovementController._Path.GetCurrentPathNode();
			if (node != NULL)
			{
				if (node->_X > _MovementController._Position.X)
				{
					_Direction = UnitDirection_Est;
				}
				else if (node->_X < _MovementController._Position.X)
				{
					_Direction = UnitDirection_West;
				}
				else if (node->_Y > _MovementController._Position.Y)
				{
					_Direction = UnitDirection_South;
				}
				else if (node->_Y < _MovementController._Position.Y)
				{
					_Direction = UnitDirection_North;
				}
			}
			else
			{
				_Direction = UnitDirection_West;
			}
		}
		break;
	}
}

void Unit::SetDestination(Cell* cell)
{
	_Destination = cell;
}

void Unit::MoveToDestination()
{
	if (_Destination != NULL)
	{
		_MovementController._Path.ClearPath();

		Map* map = GameCore::Instance()->GetWorld()->GetMap();
		UnitPathManager mgr;
		mgr.Initialize(PathfinderType_AStar);
		if (mgr.FindPath(this, map->GetCellAt(_X, _Y), _Destination, &_MovementController._Path))
		{
			_MovementController._Status = MovementControllerStatus_Active;
			_MovementController._Position = PointInt(_X, _Y);
			_MovementController._WorldPosition = Vector2(_X * map->_CellWidth, _Y * map->_CellHeight);
			_UnitState = UnitState_Moving;
		}
	}
}

SE_IMPLEMENT_CLASS(Map);
SE_IMPLEMENT_REFLECTION(Map);

Map::Map() :
	RefObject()
{
	_CellWidth = 64;
	_CellHeight = 64;
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
		XMLElement* array = document->CreateElement("Units");
		int count = _Units.Count();
		array->SetAttribute("Count", String::ToString(count));
		for (int i=0; i<count; i++)
		{
			XMLElement* item = document->CreateElement("Unit");
			context->Serialize(document, item, _Units.GetItem(i), _Units.GetItem(i)->GetType());
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

	{
		XMLElement* array = (XMLElement*)element->GetItem("Units");
		int count = array->GetAttribute("Count").ToInt32();
		XMLElement* item = (XMLElement*)array->GetItem("Unit");
		for (int i=0; i<count; i++)
		{
			XMLElement* type = (XMLElement*)item->GetFirstChild();
			Unit* unit = (Unit*)context->Deserialize(item->GetOwnerDocument(), type);
			if (unit != NULL)
				_Units.Add(unit);
			item = (XMLElement*)item->GetNextSibling();
		}
	}

	Validate();
}

void Map::Validate()
{
	int unitIndex;
	int unitCount = _Units.Count();

	for (unitIndex=0; unitIndex<unitCount; unitIndex++)
	{
		Unit* unit = _Units[unitIndex];
		Cell* cell = GetCellAt(unit->_X, unit->_Y);

		int8 cost = TileUnitCosts[cell->_Tile->_TileType][unit->_UnitDef->_UnitType];
		if (cost == 0)
		{
			// The unit can't exist on this tile
			// Remove it
			_Units.RemoveAt(unitIndex);
			delete unit;
			unitCount--;
			unitIndex--;
		}
	}
}

void Map::SetCellTileAt(Tile* tile, int x, int y)
{
	int index = GetCellIndex(x, y);
	_Cells[index]->_Tile = tile;

	Validate();
}

bool Map::HasUnitAt(int x, int y) const
{
	int index = GetUnitIndex(x, y);
	return (index != -1);
}

Unit* Map::GetUnitAt(int x, int y) const
{
	int index = GetUnitIndex(x, y);
	if (index != -1)
		return _Units[index];
	else
		return NULL;
}

int Map::GetUnitIndex(int x, int y) const
{
	int count = _Units.Count();
	for (int i=0; i<count; i++)
	{
		Unit* unit = _Units[i];
		if (unit->_X == x && unit->_Y == y)
		{
			return i;
		}
	}

	return -1;
}

void Map::AddUnit(Unit* unit)
{
	int index = GetCellIndex(unit->_X, unit->_Y);

	// Check if the unit already exists
	if (_Units.Contains(unit))
		return;

	// Check if there is already a unit at that location
	int count = _Units.Count();
	for (int i=0; i<count; i++)
	{
		Unit* curUnit = _Units[i];
		if (curUnit->_X == unit->_X && curUnit->_Y == unit->_Y)
		{
			// Remove it
			_Units.RemoveAt(i);
			delete curUnit;
			break;
		}
	}

	// Add the new unit
	_Units.Add(unit);

	Validate();
}

void Map::Update()
{
	int count = _Units.Count();
	for (int i=0; i<count; i++)
	{
		_Units[i]->Update();
	}
}

void Map::Draw()
{
	int i;

	int cellCount = _Cells.Count();
	for (i=0; i<cellCount; i++)
	{
		Cell* cell = _Cells[i];
		cell->_Tile->_Sprite->SetPosition(
			Vector2(cell->_X*_CellWidth, cell->_Y*_CellHeight));
		cell->_Tile->_Sprite->SetSize(Vector2(_CellWidth, -_CellHeight));
		cell->_Tile->_Sprite->Render();
	}

	int unitCount = _Units.Count();
	for (i=0; i<unitCount; i++)
	{
		Unit* unit = _Units[i];
		Vector2 position = Vector2(unit->_X*_CellWidth, unit->_Y*_CellHeight);
		if (unit->_MovementController._Status == MovementControllerStatus_Active)
		{
			position = unit->_MovementController._WorldPosition;
		}

		if (unit->_Direction == UnitDirection_West || unit->_Direction == UnitDirection_North)
		{
			unit->_UnitDef->_Sprite->SetSize(Vector2(_CellWidth, -_CellHeight));
		}
		else if (unit->_Direction == UnitDirection_Est || unit->_Direction == UnitDirection_South)
		{
			position.X += _CellWidth;
			unit->_UnitDef->_Sprite->SetSize(Vector2(-_CellWidth, -_CellHeight));
		}
		unit->_UnitDef->_Sprite->SetPosition(position);
		unit->_UnitDef->_Sprite->Render();
	}
}
