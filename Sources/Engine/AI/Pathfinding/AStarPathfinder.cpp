/*=============================================================================
AStarPathfinder.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AStarPathfinder.h"

namespace SonataEngine
{
	namespace AI
	{
		AStarPathfinderNode::AStarPathfinderNode() :
			PathfinderNode()
		{
			_Goal = 0.0f;
			_Heuristic = 0.0f;
			_Fitness = SE_MAX_R32;
		}

		AStarPathfinder::AStarPathfinder() :
			Pathfinder(),
			_Storage(NULL),
			_Goal(NULL),
			_Map(NULL)
		{
		}

		AStarPathfinder::~AStarPathfinder()
		{
		}

		void AStarPathfinder::SetSourceNode(PathfinderNodeID value)
		{
			_SourceNode = value;
			if (_Storage != NULL)
				_Storage->Reset();
		}

		void AStarPathfinder::SetDestinationNode(PathfinderNodeID value)
		{
			_DestinationNode = value;
			if (_Goal != NULL)
				_Goal->SetDestinationNode(value);
		}

		void AStarPathfinder::Initialize(AStarPathfinderStorage* storage, AStarPathfinderGoal* goal, AStarPathfinderMap* map)
		{
			_Storage = storage;
			_Goal = goal;
			_Map = map;

			_currentNode = NULL;

			_SourceNode = PathfinderNodeID_Invalid;
			_DestinationNode = PathfinderNodeID_Invalid;
		}

		void AStarPathfinder::Run()
		{
			real32 g, h, f;
			int neighbour;
			PathfinderNodeID neighbourID;
			AStarPathfinderNode* neighbourNode;
			bool inClosed;

			// Initialize the source node
			_currentNode = _Storage->CreateNode(_SourceNode);
			if (_currentNode == NULL)
			{
				// Failed to create the source node
				return;
			}

			_Map->InitializeNeighbour(NULL, _currentNode);

			// Set f, g, h for the source node
			h = _Goal->GetHeuristic(_currentNode);
			_currentNode->_Goal = 0.0f;
			_currentNode->_Heuristic = h;
			_currentNode->_Fitness = h;

			// Add the source node to the Open list
			_Storage->AddToOpenList(_currentNode, _Map);

			// Infinite loop
			while (true)
			{
				// Remove the best node (lowest fitness) from the Open list
				_currentNode = _Storage->RemoveBestOpenNode();

				if (_currentNode != NULL)
				{
					// Add the current node to the Closed list
					_Storage->AddToClosedList(_currentNode, _Map);
				}

				// Check if the search is finished:
				// The current node is the destination node or there are no more nodes to explore
				if (_currentNode == NULL || _Goal->IsSearchFinished(_currentNode))
				{
					// Stop the loop
					break;
				}

				// Iterate over the neighbours of the current node
				int neighbourCount = _Map->GetNeighbourCount(_currentNode);
				for (neighbour = 0; neighbour < neighbourCount; ++neighbour)
				{
					// Get the neighbour at the current index
					neighbourID = _Map->GetNeighbour(_currentNode, neighbour);

					// Check the neighbour ID
					if (neighbourID == PathfinderNodeID_Invalid)
					{
						// The neighbour ID is invalid
						continue;
					}

					// TODO: use flags to prevent the searches
					inClosed = false;

					// Check if the node is in the Open list
					neighbourNode = _Storage->FindInOpenList(neighbourID);

					// The node is not in the Open list
					if (neighbourNode == NULL)
					{
						// Check if the node is in the Closed list
						neighbourNode = _Storage->FindInClosedList(neighbourID);
					}

					// The node is not in the Closed list
					if (neighbourNode == NULL)
					{
						// The node is not in the lists
						// Check if the node is valid
						if (_Goal->IsNodeValid(neighbourID))
						{
							// Create a new node for this neighbour
							neighbourNode = _Storage->CreateNode(neighbourID);

							if (neighbourNode == NULL)
							{
								// Failed to create the neighbour node
								continue;
							}
						}
						else
						{
							// The neighbour is invalid
							continue;
						}
					}
					else
					{
						inClosed = true;
					}

					_Map->InitializeNeighbour(_currentNode, neighbourNode);

					// Check if the neighbour is in the current path
					if (neighbourNode->_Parent == _currentNode)
					{
						continue;
					}

					// Calculate the f, g, h values for the neighbour node
					g = _currentNode->_Goal;
					g += _Goal->GetCost(_currentNode, neighbourNode);
					h = _Goal->GetHeuristic(neighbourNode);
					f = g + h;

					// Check if it is more expensive to get to this neighbour from
					// the current node than from its previous parent
					if (f >= neighbourNode->_Fitness * (1.0f - (real32)Math::Epsilon))
					{
						continue;
					}

					// Set the node values
					neighbourNode->_Goal = g;
					neighbourNode->_Heuristic = h;
					neighbourNode->_Fitness = f;

					if (inClosed)
					{
						// Remove the neighbour node from the Closed list
						_Storage->RemoveFromClosedList(neighbourNode);
					}

					// Add the node to the Open list
					_Storage->AddToOpenList(neighbourNode, _Map);

					// Set the neighbour node's parent as the current node
					neighbourNode->_Parent = _currentNode;
				}
			}
		}
	}
}
