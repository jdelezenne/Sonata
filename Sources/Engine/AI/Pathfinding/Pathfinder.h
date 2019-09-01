/*=============================================================================
Pathfinder.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_PATHFINDER_H_
#define _SE_AI_PATHFINDER_H_

#include "AI/Common.h"
#include "Core/IProcess.h"

namespace SonataEngine
{
	namespace AI
	{
		typedef int32 PathfinderNodeID;
		const PathfinderNodeID PathfinderNodeID_Invalid = -1;

		class SE_AI_EXPORT PathfinderNode
		{
		public:
			PathfinderNode()
			{
				_NodeID = PathfinderNodeID_Invalid;
				_Parent = NULL;
			}

		public:
			PathfinderNodeID _NodeID;
			PathfinderNode* _Parent;
		};

		class SE_AI_EXPORT PathfinderMap
		{
		public:
			virtual int GetNeighbourCount(PathfinderNode* node) = 0;
			virtual PathfinderNodeID GetNeighbour(PathfinderNode* node, int index) = 0;
		};

		class SE_AI_EXPORT PathfinderGoal
		{
		public:
			virtual void SetDestinationNode(PathfinderNodeID node) = 0;
			virtual bool IsNodeValid(PathfinderNodeID node) { return true; }
		};

		/** Base class for the Pathfinders. */
		class SE_AI_EXPORT Pathfinder
		{
		public:
			enum Status
			{
				Status_Idle,
				Status_Searching,
				Status_Succeeded,
				Status_Failed
			};

			virtual ~Pathfinder();

			Status GetStatus() const { return _Status; }

			virtual PathfinderGoal* GetGoal() const = 0;
			virtual PathfinderMap* GetMap() const = 0;

			virtual PathfinderNodeID GetSourceNode() const = 0;
			virtual void SetSourceNode(PathfinderNodeID value) = 0;

			virtual PathfinderNodeID GetDestinationNode() const = 0;
			virtual void SetDestinationNode(PathfinderNodeID value) = 0;

			virtual PathfinderNode* GetCurrentNode() const = 0;

			virtual void Run() = 0;

		protected:
			Pathfinder();

		protected:
			Status _Status;
		};
	}
}

#endif 
