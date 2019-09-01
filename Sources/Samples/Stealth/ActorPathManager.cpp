/*=============================================================================
ActorPathManager.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "ActorPathManager.h"
#include "SampleStealth.h"

#define PROFILING 0

ActorPathNode::ActorPathNode() :
	AI::AStarPathfinderNode()
{
	_X = 0;
	_Y = 0;
}


ActorPathMap::ActorPathMap() :
	AI::AStarPathfinderMap()
{
	_Map = NULL;
}

void ActorPathMap::Initialize(Map* map)
{
	_Map = map;
}

int ActorPathMap::GetNeighbourCount(AI::PathfinderNode* node)
{
	ActorPathNode* actorNode = (ActorPathNode*)node;
	return actorNode->_Children.Count();
}

AI::PathfinderNodeID ActorPathMap::GetNeighbour(AI::PathfinderNode* node, int index)
{
	ActorPathNode* actorNode = (ActorPathNode*)node;
	return actorNode->_Children[index];
}

void ActorPathMap::SetAStarFlags(AI::PathfinderNodeID node, uint32 flags)
{
	//TODO
}

uint32 ActorPathMap::GetAStarFlags(AI::PathfinderNodeID node)
{
	//TODO
	return 0;
}

void ActorPathMap::InitializeNeighbour(AI::AStarPathfinderNode* parent, AI::AStarPathfinderNode* child)
{
	ActorPathNode* actorChild = (ActorPathNode*)child;
	actorChild->_Children.Clear();

	real32 width = _Map->_Width * _Map->_CellSize;
	real32 height = _Map->_Height * _Map->_CellSize;
	real32 divSize = _Map->_CellSize / _Map->_Divisions;

	MapNode* mapNode = PathfinderNodeIDToMapNode(child->_NodeID);

	if (mapNode->_Position.X >= divSize)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X-divSize, mapNode->_Position.Y))));
	}
	if (mapNode->_Position.X+divSize < width)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X+divSize, mapNode->_Position.Y))));
	}
	if (mapNode->_Position.Y >= divSize)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X, mapNode->_Position.Y-divSize))));
	}
	if (mapNode->_Position.Y+divSize < height)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X, mapNode->_Position.Y+divSize))));
	}

	if (mapNode->_Position.X >= divSize && mapNode->_Position.Y >= divSize)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X-divSize, mapNode->_Position.Y-divSize))));
	}
	if (mapNode->_Position.X >= divSize && mapNode->_Position.Y+divSize < height)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X-divSize, mapNode->_Position.Y+divSize))));
	}
	if (mapNode->_Position.X+divSize < width && mapNode->_Position.Y > divSize)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X+divSize, mapNode->_Position.Y-divSize))));
	}
	if (mapNode->_Position.X+divSize < width && mapNode->_Position.Y+divSize < height)
	{
		actorChild->_Children.Add(MapNodeToPathfinderNodeID(
			_Map->GetMapNodeAt(Vector2(mapNode->_Position.X+divSize, mapNode->_Position.Y+divSize))));
	}
}

AI::PathfinderNodeID ActorPathMap::MapNodeToPathfinderNodeID(MapNode* mapNode)
{
	int index = _Map->GetMapNodeIndexAt(mapNode->_Position);
	return (AI::PathfinderNodeID)index;
}

MapNode* ActorPathMap::PathfinderNodeIDToMapNode(AI::PathfinderNodeID node)
{
	return _Map->_MapNodes[(int)node];
}


ActorPathGoal::ActorPathGoal() :
	AI::AStarPathfinderGoal()
{
	_DestinationNode = AI::PathfinderNodeID_Invalid;
	_Map = NULL;
	_Actor = NULL;
}

void ActorPathGoal::Initialize(ActorPathMap* map, Actor* actor)
{
	_Map = map;
	_Actor = actor;
}

void ActorPathGoal::SetDestinationNode(AI::PathfinderNodeID node)
{
	_DestinationNode = node;
}

bool ActorPathGoal::IsNodeValid(AI::PathfinderNodeID node)
{
	MapNode* mapNode = _Map->PathfinderNodeIDToMapNode(node);

	// The tile must exist
	if (mapNode == NULL)
		return false;

	Vector2 pos = _Map->GetMap()->WorldToCell(mapNode->_Position);
	Cell* cell = _Map->GetMap()->GetCellAt(pos);
	return (cell->_Tile->_TileType != TileType_Top);
}

real32 ActorPathGoal::GetHeuristic(AI::AStarPathfinderNode* node)
{
	MapNode* source = _Map->PathfinderNodeIDToMapNode(node->_NodeID);
	MapNode* dest = _Map->PathfinderNodeIDToMapNode(_DestinationNode);

	return AI::ManhattanDistance2::Compute(source->_Position, dest->_Position);
}

real32 ActorPathGoal::GetCost(AI::AStarPathfinderNode* nodeA, AI::AStarPathfinderNode* nodeB)
{
	ActorPathNode* actorNodeA = (ActorPathNode*)nodeA;
	ActorPathNode* actorNodeB = (ActorPathNode*)nodeB;

	MapNode* source = _Map->PathfinderNodeIDToMapNode(actorNodeA->_NodeID);
	MapNode* dest = _Map->PathfinderNodeIDToMapNode(actorNodeB->_NodeID);

	// Diagonal cost is Sqrt(2)
	if (dest->_Position.X != source->_Position.X || dest->_Position.Y != source->_Position.Y)
		return Math::Sqrt(2.0f);
	else
		return 1.0f;
}

bool ActorPathGoal::IsSearchFinished(AI::AStarPathfinderNode* node)
{
	// The destination has been reached
	return (node->_NodeID == _DestinationNode);
}


PriorityQueueStorage::PriorityQueueStorage() :
	AI::AStarPathfinderStorage()
{
}

PriorityQueueStorage::~PriorityQueueStorage()
{
	Reset();
}

void PriorityQueueStorage::Reset()
{
	OpenList::Iterator itOpen = _OpenList.GetIterator();
	while (itOpen.Next())
	{
		//delete itOpen.Current();
	}

	ClosedList::Iterator itClosed = _ClosedList.GetIterator();
	while (itClosed.Next())
	{
		//delete itOpen.Current();
	}

	_OpenList.Clear();
	_ClosedList.Clear();
}

void PriorityQueueStorage::AddToOpenList(AI::AStarPathfinderNode* node, AI::AStarPathfinderMap* map)
{
	_OpenList.Enqueue(node);
}

void PriorityQueueStorage::AddToClosedList(AI::AStarPathfinderNode* node, AI::AStarPathfinderMap* map)
{
	_ClosedList.Add(node);
}

void PriorityQueueStorage::RemoveFromOpenList(AI::AStarPathfinderNode* node)
{
	if (node == _OpenList.Peek())
		_OpenList.Dequeue();
}

void PriorityQueueStorage::RemoveFromClosedList(AI::AStarPathfinderNode* node)
{
	_ClosedList.Remove(node);
}

AI::AStarPathfinderNode* PriorityQueueStorage::FindInOpenList(AI::PathfinderNodeID node)
{
	OpenList::Iterator it = _OpenList.GetIterator();
	while (it.Next())
	{
		if (it.Current()->_NodeID == node)
			return it.Current();
	}

	return NULL;
}

AI::AStarPathfinderNode* PriorityQueueStorage::FindInClosedList(AI::PathfinderNodeID node)
{
	ClosedList::Iterator it = _ClosedList.GetIterator();
	while (it.Next())
	{
		if (it.Current()->_NodeID == node)
			return it.Current();
	}

	return NULL;
}

AI::AStarPathfinderNode* PriorityQueueStorage::RemoveBestOpenNode()
{
	if (_OpenList.IsEmpty())
	{
		return NULL;
	}

	return _OpenList.Dequeue();
}


ActorPathStorage::ActorPathStorage() :
	PriorityQueueStorage()
{
}

AI::AStarPathfinderNode* ActorPathStorage::CreateNode(AI::PathfinderNodeID node)
{
	ActorPathNode* actorNode = new ActorPathNode();
	actorNode->_NodeID = node;
	return actorNode;
}

void ActorPathStorage::DestroyNode(AI::AStarPathfinderNode* node)
{
	SE_DELETE(node);
}


ActorPathManager::ActorPathManager()
{
	_Pathfinder = NULL;
	_Storage = NULL;
	_Goal = NULL;
	_Map = NULL;
}

ActorPathManager::~ActorPathManager()
{
	SE_DELETE(_Pathfinder);
	SE_DELETE(_Storage);
	SE_DELETE(_Goal);
	SE_DELETE(_Map);
}

void ActorPathManager::Initialize(PathfinderType type)
{
	_Pathfinder = new AI::AStarPathfinder();
	_Storage = new ActorPathStorage();
	_Goal = new ActorPathGoal();
	_Map = new ActorPathMap();
}

AI::PathfinderNode* ActorPathManager::FindPath(Actor* actor, MapNode* source, MapNode* destination)
{
	if (actor == NULL)
	{
		return NULL;
	}

	if (source == NULL || destination == NULL)
	{
		return NULL;
	}

	if (_Pathfinder == NULL || _Storage == NULL || _Goal == NULL || _Map == NULL)
	{
		return NULL;
	}

#if PROFILING
	real64 t = GameCore::GetTime();
#endif

	_Goal->Initialize(_Map, actor);
	_Map->Initialize(GameCore::Instance()->GetWorld()->GetMap());

	_Pathfinder->Initialize(_Storage, _Goal, _Map);
	_Pathfinder->SetSourceNode(_Map->MapNodeToPathfinderNodeID(source));
	_Pathfinder->SetDestinationNode(_Map->MapNodeToPathfinderNodeID(destination));
	_Pathfinder->Run();

#if PROFILING
	Console::WriteLine(String::ToString(GameCore::GetTime() - t));
#endif

	return _Pathfinder->GetCurrentNode();
}

bool ActorPathManager::HasPath(Actor* actor, MapNode* source, MapNode* destination)
{
	AI::PathfinderNode* node;
	node = FindPath(actor, source, destination);

	return (node != NULL);
}

bool ActorPathManager::FindPath(Actor* actor, MapNode* source, MapNode* destination, ActorPath* path)
{
	AI::PathfinderNode* node;
	node = FindPath(actor, source, destination);

	if (node == NULL)
		return false;

	while (node != NULL)
	{
		MapNode* mapNode = _Map->PathfinderNodeIDToMapNode(node->_NodeID);
		PathNode pathNode;
		pathNode._Position = mapNode->_Position;
		path->AddPathNode(pathNode);
		node = node->_Parent;
	}

	return true;
}
