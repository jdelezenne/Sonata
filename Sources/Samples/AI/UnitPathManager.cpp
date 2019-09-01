/*=============================================================================
UnitPathManager.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "UnitPathManager.h"
#include "SampleAI.h"

int8 TileUnitCosts[TileType_Count][UnitType_Count] = 
{
	{ 1, 2, 1, 0, 1 },
	{ 1, 3, 1, 0, 1 },
	{ 2, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 1 },
	{ 2, 1, 0, 0, 1 },
	{ 0, 0, 0, 1, 1 }
};

UnitPathNode::UnitPathNode() :
	AI::AStarPathfinderNode()
{
	_X = 0;
	_Y = 0;
}


UnitPathMap::UnitPathMap() :
	AI::AStarPathfinderMap()
{
	_Map = NULL;
}

void UnitPathMap::Initialize(Map* map)
{
	_Map = map;
}

int UnitPathMap::GetNeighbourCount(AI::PathfinderNode* node)
{
	UnitPathNode* unitNode = (UnitPathNode*)node;
	return unitNode->_Children.Count();
}

AI::PathfinderNodeID UnitPathMap::GetNeighbour(AI::PathfinderNode* node, int index)
{
	UnitPathNode* unitNode = (UnitPathNode*)node;
	return unitNode->_Children[index];
}

void UnitPathMap::SetAStarFlags(AI::PathfinderNodeID node, uint32 flags)
{
	//TODO
}

uint32 UnitPathMap::GetAStarFlags(AI::PathfinderNodeID node)
{
	//TODO
	return 0;
}

void UnitPathMap::InitializeNeighbour(AI::AStarPathfinderNode* parent, AI::AStarPathfinderNode* child)
{
	UnitPathNode* unitChild = (UnitPathNode*)child;
	unitChild->_Children.Clear();

	Cell* cell = PathfinderNodeIDToCell(child->_NodeID);

	// No diagonals
	if (cell->_X > 0)
		unitChild->_Children.Add(CellToPathfinderNodeID(_Map->GetCellAt(cell->_X-1, cell->_Y)));
	if (cell->_X < _Map->_Width-1)
		unitChild->_Children.Add(CellToPathfinderNodeID(_Map->GetCellAt(cell->_X+1, cell->_Y)));
	if (cell->_Y > 0)
		unitChild->_Children.Add(CellToPathfinderNodeID(_Map->GetCellAt(cell->_X, cell->_Y-1)));
	if (cell->_Y < _Map->_Height-1)
		unitChild->_Children.Add(CellToPathfinderNodeID(_Map->GetCellAt(cell->_X, cell->_Y+1)));
}

AI::PathfinderNodeID UnitPathMap::CellToPathfinderNodeID(Cell* cell)
{
	int index = _Map->GetCellIndex(cell->_X, cell->_Y);
	return (AI::PathfinderNodeID)index;
}

Cell* UnitPathMap::PathfinderNodeIDToCell(AI::PathfinderNodeID node)
{
	return _Map->_Cells[(int)node];
}


UnitPathGoal::UnitPathGoal() :
	AI::AStarPathfinderGoal()
{
	_DestinationNode = AI::PathfinderNodeID_Invalid;
	_Map = NULL;
	_Unit = NULL;
}

void UnitPathGoal::Initialize(UnitPathMap* map, Unit* unit)
{
	_Map = map;
	_Unit = unit;
}

void UnitPathGoal::SetDestinationNode(AI::PathfinderNodeID node)
{
	_DestinationNode = node;
}

bool UnitPathGoal::IsNodeValid(AI::PathfinderNodeID node)
{
	Cell* cell = _Map->PathfinderNodeIDToCell(node);

	// The tile cost must be higher than 0
	int8 cost = TileUnitCosts[cell->_Tile->_TileType][_Unit->_UnitDef->_UnitType];
	if (cost == 0)
		return false;

	// The tile must be free
	return !_Map->GetMap()->HasUnitAt(cell->_X, cell->_Y);
}

real32 UnitPathGoal::GetHeuristic(AI::AStarPathfinderNode* node)
{
	Cell* source = _Map->PathfinderNodeIDToCell(node->_NodeID);
	Cell* dest = _Map->PathfinderNodeIDToCell(_DestinationNode);

	return AI::ChebyshevDistance2::Compute(
		Vector2(source->_X, source->_Y), Vector2(dest->_X, dest->_Y));
}

real32 UnitPathGoal::GetCost(AI::AStarPathfinderNode* nodeA, AI::AStarPathfinderNode* nodeB)
{
	UnitPathNode* unitNodeA = (UnitPathNode*)nodeA;
	UnitPathNode* unitNodeB = (UnitPathNode*)nodeB;

	Cell* source = _Map->PathfinderNodeIDToCell(unitNodeA->_NodeID);
	Cell* dest = _Map->PathfinderNodeIDToCell(unitNodeB->_NodeID);

	// The cost is given by the value at the destination cell
	int8 cost = TileUnitCosts[dest->_Tile->_TileType][_Unit->_UnitDef->_UnitType];
	return (real32)cost;
}

bool UnitPathGoal::IsSearchFinished(AI::AStarPathfinderNode* node)
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


UnitPathStorage::UnitPathStorage() :
	PriorityQueueStorage()
{
}

AI::AStarPathfinderNode* UnitPathStorage::CreateNode(AI::PathfinderNodeID node)
{
	UnitPathNode* unitNode = new UnitPathNode();
	unitNode->_NodeID = node;
	return unitNode;
}

void UnitPathStorage::DestroyNode(AI::AStarPathfinderNode* node)
{
	SE_DELETE(node);
}


UnitPathManager::UnitPathManager()
{
	_Pathfinder = NULL;
	_Storage = NULL;
	_Goal = NULL;
	_Map = NULL;
}

UnitPathManager::~UnitPathManager()
{
	SE_DELETE(_Pathfinder);
	SE_DELETE(_Storage);
	SE_DELETE(_Goal);
	SE_DELETE(_Map);
}

void UnitPathManager::Initialize(PathfinderType type)
{
	_Pathfinder = new AI::AStarPathfinder();
	_Storage = new UnitPathStorage();
	_Goal = new UnitPathGoal();
	_Map = new UnitPathMap();
}

AI::PathfinderNode* UnitPathManager::FindPath(Unit* unit, Cell* source, Cell* destination)
{
	if (unit == NULL)
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

	_Goal->Initialize(_Map, unit);
	_Map->Initialize(GameCore::Instance()->GetWorld()->GetMap());

	_Pathfinder->Initialize(_Storage, _Goal, _Map);
	_Pathfinder->SetSourceNode(_Map->CellToPathfinderNodeID(source));
	_Pathfinder->SetDestinationNode(_Map->CellToPathfinderNodeID(destination));
	_Pathfinder->Run();

	return _Pathfinder->GetCurrentNode();
	// Allows uncomplete paths
	//if (_Pathfinder->GetCurrentNode() != NULL)
	//	return _Pathfinder->GetCurrentNode();
	//else
	//	return _Pathfinder->GetPreviousNode();
}

bool UnitPathManager::HasPath(Unit* unit, Cell* source, Cell* destination)
{
	AI::PathfinderNode* node;
	node = FindPath(unit, source, destination);

	return (node != NULL);
}

bool UnitPathManager::FindPath(Unit* unit, Cell* source, Cell* destination, UnitPath* path)
{
	AI::PathfinderNode* node;
	node = FindPath(unit, source, destination);

	if (node == NULL)
		return false;

	while (node != NULL)
	{
		Cell* cell = _Map->PathfinderNodeIDToCell(node->_NodeID);
		PathNode pathNode;
		pathNode._X = cell->_X;
		pathNode._Y = cell->_Y;
		path->AddPathNode(pathNode);
		node = node->_Parent;
	}

	return true;
}
