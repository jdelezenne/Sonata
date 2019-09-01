/*=============================================================================
ActorPathManager.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_ACTORPATHMANAGER_H_
#define _SE_ACTORPATHMANAGER_H_

#include "Common.h"
#include "World.h"

enum PathfinderType
{
	PathfinderType_AStar
};

class ActorPathMap : public AI::AStarPathfinderMap
{
public:
	ActorPathMap();

	virtual int GetNeighbourCount(AI::PathfinderNode* node);
	virtual AI::PathfinderNodeID GetNeighbour(AI::PathfinderNode* node, int neighbour);

	virtual void SetAStarFlags(AI::PathfinderNodeID node, uint32 flags);
	virtual uint32 GetAStarFlags(AI::PathfinderNodeID node);

	virtual void InitializeNeighbour(AI::AStarPathfinderNode* parent, AI::AStarPathfinderNode* child);

	AI::PathfinderNodeID MapNodeToPathfinderNodeID(MapNode* node);
	MapNode* PathfinderNodeIDToMapNode(AI::PathfinderNodeID node);

	void Initialize(Map* map);

	Map* GetMap() const { return _Map; }

protected:
	Map* _Map;
};

class ActorPathGoal : public AI::AStarPathfinderGoal
{
public:
	ActorPathGoal();

	virtual void SetDestinationNode(AI::PathfinderNodeID node);
	virtual bool IsNodeValid(AI::PathfinderNodeID node);

	virtual real32 GetHeuristic(AI::AStarPathfinderNode* node);
	virtual real32 GetCost(AI::AStarPathfinderNode* nodeA, AI::AStarPathfinderNode* nodeB);
	virtual bool IsSearchFinished(AI::AStarPathfinderNode* node);

	void Initialize(ActorPathMap* map, Actor* actor);

protected:
	AI::PathfinderNodeID _DestinationNode;
	ActorPathMap* _Map;
	Actor* _Actor;
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

class ActorPathStorage : public PriorityQueueStorage
{
public:
	ActorPathStorage();

	virtual AI::AStarPathfinderNode* CreateNode(AI::PathfinderNodeID node);
	virtual void DestroyNode(AI::AStarPathfinderNode* node);
};

class ActorPathManager
{
public:
	ActorPathManager();
	virtual ~ActorPathManager();

	void Initialize(PathfinderType type);

	bool HasPath(Actor* actor, MapNode* source, MapNode* destination);
	bool FindPath(Actor* actor, MapNode* source, MapNode* destination, ActorPath* path);

protected:
	AI::PathfinderNode* FindPath(Actor* actor, MapNode* source, MapNode* destination);

protected:
	AI::AStarPathfinder* _Pathfinder;
	PriorityQueueStorage* _Storage;
	ActorPathGoal* _Goal;
	ActorPathMap* _Map;
};

#endif 
