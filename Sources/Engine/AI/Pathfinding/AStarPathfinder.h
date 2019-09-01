/*=============================================================================
AStarPathfinder.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_ASTARPATHFINDER_H_
#define _SE_AI_ASTARPATHFINDER_H_

#include "AI/Common.h"
#include "AI/Pathfinding/Pathfinder.h"
#include "AI/Pathfinding/HeuristicCost.h"
#include "Core/Containers/PriorityQueue.h"

namespace SonataEngine
{
	namespace AI
	{
		class SE_AI_EXPORT AStarPathfinderNode : public PathfinderNode, public IComparable<AStarPathfinderNode>
		{
		public:
			AStarPathfinderNode();

			virtual int CompareTo(const AStarPathfinderNode value) const
			{
				if (_Fitness < value._Fitness)
					return -1;
				else if (_Fitness > value._Fitness)
					return 1;
				else
					return 0;
			}

			bool operator<(const AStarPathfinderNode& value) const
			{
				return (_Fitness < value._Fitness);
			}

			bool operator>(const AStarPathfinderNode& value) const
			{
				return (_Fitness > value._Fitness);
			}

		public:
			real32 _Goal;
			real32 _Heuristic;
			real32 _Fitness;
		};

		class SE_AI_EXPORT AStarPathfinderMap : public PathfinderMap
		{
		public:
			enum AStarPathfinderMapFlags
			{
				AStarPathfinderMapFlags_Unchecked = 0,
				AStarPathfinderMapFlags_Open = (1<<0),
				AStarPathfinderMapFlags_Closed = (1<<1),
				AStarPathfinderMapFlags_OpenOrClosed =
					AStarPathfinderMapFlags_Open | AStarPathfinderMapFlags_Closed,
				AStarPathfinderMapFlags_NotPassable = (1<<2),
			};

		public:
			virtual void SetAStarFlags(PathfinderNodeID node, uint32 flags) = 0;
			virtual uint32 GetAStarFlags(PathfinderNodeID node) = 0;

			virtual void InitializeNeighbour(AStarPathfinderNode* parent, AStarPathfinderNode* child) {}
		};

		class SE_AI_EXPORT AStarPathfinderGoal : public PathfinderGoal
		{
		public:
			virtual real32 GetHeuristic(AStarPathfinderNode* node) = 0;
			virtual real32 GetCost(AStarPathfinderNode* nodeA, AStarPathfinderNode* nodeB) = 0;
			virtual bool IsSearchFinished(AStarPathfinderNode* node) = 0;
		};

		class SE_AI_EXPORT AStarPathfinderStorage
		{
		public:
			virtual AStarPathfinderNode* CreateNode(PathfinderNodeID node) = 0;
			virtual void DestroyNode(AStarPathfinderNode* node) = 0;

			virtual void Reset() = 0;
			virtual void AddToOpenList(AStarPathfinderNode* node, AStarPathfinderMap* map) = 0;
			virtual void AddToClosedList(AStarPathfinderNode* node, AStarPathfinderMap* map) = 0;
			virtual void RemoveFromOpenList(AStarPathfinderNode* node) = 0;
			virtual void RemoveFromClosedList(AStarPathfinderNode* node) = 0;

			virtual AStarPathfinderNode* FindInOpenList(PathfinderNodeID node) = 0;
			virtual AStarPathfinderNode* FindInClosedList(PathfinderNodeID node) = 0;

			virtual AStarPathfinderNode* RemoveBestOpenNode() = 0;
		};

		/** A* Pathfinder. */
		class SE_AI_EXPORT AStarPathfinder : public Pathfinder
		{
		public:
			AStarPathfinder();
			virtual ~AStarPathfinder();

			AStarPathfinderStorage* GetStorage() const { return _Storage; }
			virtual PathfinderGoal* GetGoal() const { return _Goal; }
			virtual PathfinderMap* GetMap() const { return _Map; }

			virtual PathfinderNodeID GetSourceNode() const { return _SourceNode; }
			virtual void SetSourceNode(PathfinderNodeID value);

			virtual PathfinderNodeID GetDestinationNode() const { return _DestinationNode; }
			virtual void SetDestinationNode(PathfinderNodeID value);

			virtual PathfinderNode* GetCurrentNode() const { return _currentNode; }

			void Initialize(AStarPathfinderStorage* storage, AStarPathfinderGoal* goal, AStarPathfinderMap* map);

			virtual void Run();

		protected:
			AStarPathfinderStorage* _Storage;
			AStarPathfinderGoal* _Goal;
			AStarPathfinderMap* _Map;

			AStarPathfinderNode* _currentNode;

			PathfinderNodeID _SourceNode;
			PathfinderNodeID _DestinationNode;
		};
	}
}

#endif 
