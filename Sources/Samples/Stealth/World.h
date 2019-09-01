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
	Vector2 _Position;
};

class ActorPath
{
public:
	ActorPath();

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

const int32 DefaultMapDivisions = 4;

class MapNode
{
public:
	MapNode();

public:
	Vector2 _Position;
};

class ActorPathNode : public AI::AStarPathfinderNode
{
public:
	ActorPathNode();

public:
	BaseArray<AI::PathfinderNodeID> _Children;
	int32 _X;
	int32 _Y;
};

enum TileType
{
	TileType_Floor,
	TileType_Top,
	TileType_Tunnel,
	TileType_Snow,
	TileType_Trap
};

SE_BEGIN_ENUM(TileType);
	SE_Enum(Floor);
	SE_Enum(Top);
	SE_Enum(Tunnel);
	SE_Enum(Snow);
	SE_Enum(Trap);
SE_END_ENUM(TileType);

enum ActorType
{
	ActorType_Character,
	ActorType_Placeable,
	ActorType_Spawn,
	ActorType_Goal,
	ActorType_WayPoint
};

SE_BEGIN_ENUM(ActorType);
	SE_Enum(Character);
	SE_Enum(Placeable);
	SE_Enum(Spawn);
	SE_Enum(Goal);
	SE_Enum(WayPoint);
SE_END_ENUM(ActorType);

class GameEntity : public RefObject
{
	SE_DECLARE_ABSTRACT(GameEntity, RefObject);

	SE_BEGIN_REFLECTION(GameEntity);
		SE_Field(_name, String, Public);
		SE_Field(_Design, bool, Public);
	SE_END_REFLECTION(GameEntity);

public:
	GameEntity();

	virtual void Update(real64 elapsed) = 0;

public:
	String _name;
	bool _Design;
};

class Tile : public RefObject
{
	SE_DECLARE_CLASS(Tile, RefObject);

	SE_BEGIN_REFLECTION(Tile);
		SE_Field(_TileType, TileType, Public);
		SE_Field(_fileName, String, Public);
		SE_Field(_Color, Color32, Public);
	SE_END_REFLECTION(Tile);

public:
	Tile();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	TileType _TileType;
	String _fileName;
	Color32 _Color;

	MeshPtr _mesh;
};

class ActorDef : public RefObject
{
	SE_DECLARE_CLASS(ActorDef, Object);

	SE_BEGIN_REFLECTION(ActorDef);
		SE_Field(_name, String, Public);
		SE_Field(_ActorType, ActorType, Public);
		SE_Field(_fileName, String, Public);
		SE_Field(_Offset, Vector3, Public);
		SE_Field(_Orientation, Vector3, Public);
		SE_Field(_Scale, real32, Public);
	SE_END_REFLECTION(ActorDef);

public:
	ActorDef();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	String _name;
	ActorType _ActorType;
	String _fileName;
	Vector3 _Offset;
	Vector3 _Orientation;
	real32 _Scale;

	ModelPtr _Model;
};

class Tileset : public RefObject
{
	SE_DECLARE_CLASS(Tileset , Object);

	SE_BEGIN_REFLECTION(Tileset);
		SE_Field(_PlayerActor, String, Public);
	SE_END_REFLECTION(Tileset);

public:
	Tileset();

	Tile* GetTileOfType(TileType type) const;

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	String _PlayerActor;

	typedef Array<Tile*> TileList;
	TileList _Tiles;
	typedef Array<ActorDef*> ActorDefList;
	ActorDefList _ActorDefs;
};

class Cell : public RefObject
{
	SE_DECLARE_CLASS(Cell, Object);

	SE_BEGIN_REFLECTION(Cell);
		SE_Field(_X, int32, Public);
		SE_Field(_Y, int32, Public);
		SE_Field(_Depth, int32, Public);
	SE_END_REFLECTION(Cell);

public:
	Cell();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

public:
	Tile* _Tile;
	int32 _X;
	int32 _Y;
	int32 _Depth;
};

class Actor : public GameEntity
{
	SE_DECLARE_CLASS(Actor, GameEntity);

	SE_BEGIN_REFLECTION(Actor);
		SE_Field(_RotationSpeed, real32, Public);
		SE_Field(_Position, Vector3, Public);
		SE_Field(_Orientation, Quaternion, Public);
	SE_END_REFLECTION(Actor);

public:
	Actor();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	void Move(const Vector3& pos);
	void LookAt(const Vector3& pos);

	virtual void Update(real64 elapsed);

public:
	ActorDef* _ActorDef;
	real32 _RotationSpeed;
	Vector3 _Position;
	Quaternion _Orientation;

	bool _Moving;
	Vector3 _MovePosition;

	bool _Rotating;
	Quaternion _StartRotation;
	Quaternion _TargetRotation;
};

class Map : public RefObject
{
	SE_DECLARE_CLASS(Map, Object);

	SE_BEGIN_REFLECTION(Map);
		SE_Field(_name, String, Public);
		SE_Field(_CellSize, int32, Public);
		SE_Field(_Width, int32, Public);
		SE_Field(_Height, int32, Public);
	SE_END_REFLECTION(Map);

public:
	Map();

	virtual void OnSerialized(XMLSerializer* context, XMLElement* element);
	virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

	int GetCellIndex(const Vector2& value) const;

	Cell* GetCellAt(const Vector2& value) const
	{
		int index = GetCellIndex(value);
		return (index != -1 ? _Cells[index] : NULL);
	}

	void SetCellTileAt(Tile* tile, const Vector2& value);

	Cell* GetActorCell(Actor* actor) const;

	int GetActorDepth(Actor* actor) const;

	bool HasActorAt(const Vector2& value) const;
	Actor* GetActorOfType(ActorType type) const;
	Actor* GetActorAt(const Vector2& value) const;
	Actor* GetActorByName(const String& name) const;

	int GetActorIndex(const Vector2& value) const;

	String GetUniqueName(ActorType type) const;
	void AddActor(Actor* actor);

	void SetActorPosition(Actor* actor, const Vector2& pos);

	int GetMapNodeIndexAt(const Vector2& value) const;
	MapNode* GetMapNodeAt(const Vector2& value) const;

	void Initialize();
	void Update(real64 elapsed);
	void Draw();

	Vector2 WorldToCell(const Vector2& value) const;
	Vector2 CellToWorld(const Vector2& value) const;
	Vector3 WorldToCell(const Vector3& value) const;
	Vector3 CellToWorld(const Vector3& value) const;

protected:
	void Validate();

public:
	String _name;
	int32 _CellSize;
	int32 _Divisions;
	int32 _Width;
	int32 _Height;
	typedef Array<Cell*> CellList;
	CellList _Cells;
	typedef Array<Actor*> ActorList;
	ActorList _Actors;

	MeshPtr _mesh;
	typedef Array<MapNode*> MapNodeList;
	MapNodeList _MapNodes;
};

#endif 
