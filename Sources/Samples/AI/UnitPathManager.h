/*=============================================================================
UnitPathManager.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_UNITPATHMANAGER_H_
#define _SE_UNITPATHMANAGER_H_

#include "Common.h"
#include "World.h"

extern int8 TileUnitCosts[TileType_Count][UnitType_Count];

enum PathfinderType
{
	PathfinderType_AStar
};

class UnitPathMap : public AI::AStarPathfinderMap
{
public:
	UnitPathMap();

	virtual int GetNeighbourCount(AI::PathfinderNode* node);
	virtual AI::PathfinderNodeID GetNeighbour(AI::PathfinderNode* node, int neighbour);

	virtual void SetAStarFlags(AI::PathfinderNodeID node, uint32 flags);
	virtual uint32 GetAStarFlags(AI::PathfinderNodeID node);

	virtual void InitializeNeighbour(AI::AStarPathfinderNode* parent, AI::AStarPathfinderNode* child);

	AI::PathfinderNodeID CellToPathfinderNodeID(Cell* cell);
	Cell* PathfinderNodeIDToCell(AI::PathfinderNodeID node);

	void Initialize(Map* map);

	Map* GetMap() const { return _Map; }

protected:
	Map* _Map;
};

class UnitPathGoal : public AI::AStarPathfinderGoal
{
public:
	UnitPathGoal();

	virtual void SetDestinationNode(AI::PathfinderNodeID node);
	virtual bool IsNodeValid(AI::PathfinderNodeID node);

	virtual real32 GetHeuristic(AI::AStarPathfinderNode* node);
	virtual real32 GetCost(AI::AStarPathfinderNode* nodeA, AI::AStarPathfinderNode* nodeB);
	virtual bool IsSearchFinished(AI::AStarPathfinderNode* node);

	void Initialize(UnitPathMap* map, Unit* unit);

protected:
	AI::PathfinderNodeID _DestinationNode;
	UnitPathMap* _Map;
	Unit* _Unit;
};

template <class T>
class RefComparer : public IComparer<T>
{
public:
	virtual int Compare(const T& x, const T& y) const
	{
		if (*x < *y)
			return -1;
		else if (*x > *y)
			return 1;
		else
			return 0;
	}
};

class PriorityQueueStorage : public AI::AStarPathfinderStorage
{
public:
	PriorityQueueStorage();
	virtual ~PriorityQueueStorage();

	virtual void Reset();
	virtual void AddToOpenList(AI::AStarPathfinderNode* node, AI::AStarPathfinderMap* map);
	virtual void AddToClosedList(AI::AStarPathfinderNode* node, AI::AStarPathfinderMap* map);
	virtual void RemoveFromOpenList(AI::AStarPathfinderNode* node);
	virtual void RemoveFromClosedList(AI::AStarPathfinderNode* node);

	virtual AI::AStarPathfinderNode* FindInOpenList(AI::PathfinderNodeID node);
	virtual AI::AStarPathfinderNode* FindInClosedList(AI::PathfinderNodeID node);

	virtual AI::AStarPathfinderNode* RemoveBestOpenNode();

protected:
	typedef PriorityQueue< AI::AStarPathfinderNode*, RefComparer<AI::AStarPathfinderNode*> > OpenList;
	OpenList _OpenList;
	typedef Array<AI::AStarPathfinderNode*> ClosedList;
	ClosedList _ClosedList;
};

class UnitPathStorage : public PriorityQueueStorage
{
public:
	UnitPathStorage();

	virtual AI::AStarPathfinderNode* CreateNode(AI::PathfinderNodeID node);
	virtual void DestroyNode(AI::AStarPathfinderNode* node);
};

class UnitPathManager
{
public:
	UnitPathManager();
	virtual ~UnitPathManager();

	void Initialize(PathfinderType type);

	bool HasPath(Unit* unit, Cell* source, Cell* destination);
	bool FindPath(Unit* unit, Cell* source, Cell* destination, UnitPath* path);

protected:
	AI::PathfinderNode* FindPath(Unit* unit, Cell* source, Cell* destination);

protected:
	AI::AStarPathfinder* _Pathfinder;
	PriorityQueueStorage* _Storage;
	UnitPathGoal* _Goal;
	UnitPathMap* _Map;
};

#endif 
