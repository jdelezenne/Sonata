/*=============================================================================
World.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _WORLD_H_
#define _WORLD_H_

#include "Common.h"

class PathNode
{
public:
	PathNode();

public:
	int32 _X;
	int32 _Y;
};

class UnitPath
{
public:
	UnitPath();

	int GetCurrentPathNodeIndex() const;
	PathNode* GetCurrentPathNode();
	PathNode* GetPathNode(int index);
	int GetPathLength() const;
	bool IsPathComplete() const;

	void ClearPath();
	void AddPathNode(const PathNode& node);
	void AdvancePathNode();

protected:
	typedef BaseArray<PathNode> PathNodeList;
	PathNodeList _PathNodes;
	int _currentNode;
};

enum MovementControllerStatus
{
	MovementControllerStatus_Inactive,
	MovementControllerStatus_Active,
	MovementControllerStatus_Complete
};

class MovementController
{
public:
	MovementController();
	void Update(real64 elapsed);

public:
	MovementControllerStatus _Status;
	real32 _Speed;
	UnitPath _Path;
	PointInt _Position;
	Vector2 _WorldPosition;
};

class UnitPathNode : public AI::AStarPathfinderNode
{
public:
	UnitPathNode();

public:
	BaseArray<AI::PathfinderNodeID> _Children;
	int32 _X;
	int32 _Y;
};

enum TileType
{
	TileType_Plain,
	TileType_Wood,
	TileType_River,
	TileType_Road,
	TileType_Mountain,
	TileType_Sea,
	TileType_Count
};

SE_BEGIN_ENUM(TileType);
	SE_Enum(Plain);
	SE_Enum(Wood);
	SE_Enum(River);
	SE_Enum(Road);
	SE_Enum(Mountain);
	SE_Enum(Sea);
SE_END_ENUM(TileType);

enum UnitType
{
	UnitType_Infantry,
	UnitType_Tires,
	UnitType_Tread,
	UnitType_Sea,
	UnitType_Air,
	UnitType_Count
};

SE_BEGIN_ENUM(UnitType);
	SE_Enum(Infantry);
	SE_Enum(Tires);
	SE_Enum(Tread);
	SE_Enum(Sea);
	SE_Enum(Air);
SE_END_ENUM(UnitType);

enum UnitDirection
{
	UnitDirection_North,
	UnitDirection_South,
	UnitDirection_West,
	UnitDirection_Est,
};

enum UnitState
{
	UnitState_Idle,
	UnitState_Moving
};

class MapEntity : public RefObject
{
	SE_DECLARE_ABSTRACT(MapEntity, RefObject);

public:
	virtual void Update() = 0;

public:
	UnitState _UnitState;
};

class Tile : public RefObject
{
	SE_DECLARE_CLASS(Tile, RefObject);

	SE_BEGIN_REFLECTION(Tile);
		SE_Field(_TileType, TileType, Public);
		SE_Field(_fileName, String, Public);
	SE_END_REFLECTION(Tile);

public:
	Tile();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	TileType _TileType;
	String _fileName;

	SpritePtr _Sprite;
};

class UnitDef : public RefObject
{
	SE_DECLARE_CLASS(UnitDef, Object);

	SE_BEGIN_REFLECTION(UnitDef);
		SE_Field(_UnitType, UnitType, Public);
		SE_Field(_fileName, String, Public);
	SE_END_REFLECTION(UnitDef);

public:
	UnitDef();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	UnitType _UnitType;
	String _fileName;

	SpritePtr _Sprite;
};

class Tileset : public RefObject
{
	SE_DECLARE_CLASS(Tileset , Object);

	SE_BEGIN_REFLECTION(Tileset);
	SE_END_REFLECTION(Tileset);

public:
	Tileset();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	typedef Array<Tile*> TileList;
	TileList _Tiles;
	typedef Array<UnitDef*> UnitDefList;
	UnitDefList _UnitDefs;
};

class Cell : public RefObject
{
	SE_DECLARE_CLASS(Cell, Object);

	SE_BEGIN_REFLECTION(Cell);
		SE_Field(_X, int32, Public);
		SE_Field(_Y, int32, Public);
	SE_END_REFLECTION(Cell);

public:
	Cell();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	Tile* _Tile;
	int32 _X;
	int32 _Y;
};

class Unit : public MapEntity
{
	SE_DECLARE_CLASS(Unit, MapEntity);

	SE_BEGIN_REFLECTION(Unit);
		SE_Field(_X, int32, Public);
		SE_Field(_Y, int32, Public);
	SE_END_REFLECTION(Unit);

public:
	Unit();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	virtual void Update();

	void SetPosition(int32 x, int32 y);

	void SetDestination(Cell* cell);
	void MoveToDestination();

public:
	UnitDef* _UnitDef;
	int32 _X;
	int32 _Y;
	UnitDirection _Direction;
	Cell* _Destination;
	MovementController _MovementController;
};

class Map : public RefObject
{
	SE_DECLARE_CLASS(Map, Object);

	SE_BEGIN_REFLECTION(Map);
		SE_Field(_name, String, Public);
		SE_Field(_CellWidth, int32, Public);
		SE_Field(_CellHeight, int32, Public);
		SE_Field(_Width, int32, Public);
		SE_Field(_Height, int32, Public);
	SE_END_REFLECTION(Map);

public:
	Map();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	int GetCellIndex(int x, int y) const
	{
		return (y * _Width + x);
	}

	Cell* GetCellAt(int x, int y) const
	{
		int index = GetCellIndex(x, y);
		return (index != -1 ? _Cells[index] : NULL);
	}

	void SetCellTileAt(Tile* tile, int x, int y);

	bool HasUnitAt(int x, int y) const;
	Unit* GetUnitAt(int x, int y) const;

	int GetUnitIndex(int x, int y) const;

	void AddUnit(Unit* unit);

	void Update();
	void Draw();

protected:
	void Validate();

public:
	String _name;
	Tileset* _Tileset;
	int32 _CellWidth;
	int32 _CellHeight;
	int32 _Width;
	int32 _Height;
	typedef Array<Cell*> CellList;
	CellList _Cells;
	typedef Array<Unit*> UnitList;
	UnitList _Units;
};

#endif 
