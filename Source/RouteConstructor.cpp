#include "pch.h"
#include "RouteConstructor.h"
#include "Chunk.h"
#include "ChunkHandler.h"

RouteConstructor::RouteConstructor()
{
	routes temp1; m_route.push_back(temp1);
	routes temp2; m_route.push_back(temp2);
	routes temp3; m_route.push_back(temp3);
	routes temp4; m_route.push_back(temp4);
}

RouteConstructor::~RouteConstructor()
{
}

void RouteConstructor::Update()
{
}

//-----------------------------------------------------------------------------------------------------//
//----------------------------------------PATHFINDING--------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

void RouteConstructor::CreatePathfindingMap()
{
	int IdCounter = 0;

	for (int i = 0; i < 32 * 15; i++)
	{
		for (int j = 0; j < 32 * 15; j++)
		{
			Nodes* tempNode = new Nodes(IdCounter, DirectX::XMFLOAT3(i, WorldManipulation::GetHeightmap(i,j), j));
			IdCounter++;

			std::vector<int> StepUpOneConnections;
			std::vector<int> StepUpFiveConnections;

			//-----------------------------------------------------//
			//------------ Step up of One Calculations ------------//
			//-----------------------------------------------------//

			//- Check if the Node Id Is valid -//
			if (i != MAX && j != MIN ) 
				//- If Node is within StepUpRange Add to ConnectedWaypoints -//
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i + 1, j - 1) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i + 1, j - 1))
				{
					//- Save As Connected Waypoint -//
					StepUpOneConnections.push_back((IdCounter + (32 * 15)) - 2);	//Top Left
				}

			if (i != MAX)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i + 1, j) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i + 1, j))
				{
					StepUpOneConnections.push_back((IdCounter + (32 * 15)) - 1);	//Top Mid
				}

			if (i != MAX && j != MAX)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i + 1, j + 1) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i + 1, j + 1))
				{
					StepUpOneConnections.push_back((IdCounter + (32 * 15)) + 0);	//Top RIght
				}

			if (j != MAX)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i, j + 1) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i, j + 1))
				{
					StepUpOneConnections.push_back(IdCounter + 1);					//Mid right
				}

			if (j != MIN)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i, j - 1) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i, j - 1))
				{
					StepUpOneConnections.push_back(IdCounter - 1);					//Mid Left
				}

			if (i != MIN && j != MIN)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i - 1, j - 1) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i - 1, j - 1))
				{
					StepUpOneConnections.push_back((IdCounter - (32 * 15)) - 2);	//Bottom Left
				}

			if (i != MIN)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i - 1,j) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i - 1, j))
				{
					StepUpOneConnections.push_back((IdCounter - (32 * 15)) - 1);	//Botom Mid
				}

			if (i != MIN && j != MAX)
				if (tempNode->GetPosition().y + 1 >= WorldManipulation::GetHeightmap(i - 1, j + 1) && tempNode->GetPosition().y - 1 <= WorldManipulation::GetHeightmap(i - 1, j + 1))
				{
					StepUpOneConnections.push_back((IdCounter - (32 * 15)) + 0);	//Botom Right
				}


			//------------------------------------------------------//
			//------------ Step up of Five Calculations ------------//
			//------------------------------------------------------//

			//- Check if the Node Id Is valid -//
			if (i != MAX && j != MIN)
				//- If Node is within StepUpRange Add to ConnectedWaypoints -//
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i + 1, j - 1) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i + 1, j - 1))
				{
					//- Save As Connected Waypoint -//
					StepUpFiveConnections.push_back((IdCounter + (32 * 15)) - 2);	//Top Left
				}

			if (i != MAX)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i + 1, j) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i + 1, j))
				{
					StepUpFiveConnections.push_back((IdCounter + (32 * 15)) - 1);	//Top Mid
				}

			if (i != MAX && j != MAX)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i + 1, j + 1) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i + 1, j + 1))
				{
					StepUpFiveConnections.push_back((IdCounter + (32 * 15)) + 0);	//Top RIght
				}

			if (j != MAX)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i, j + 1) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i, j + 1))
				{
					StepUpFiveConnections.push_back(IdCounter + 1);					//Mid right
				}

			if (j != MIN)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i, j - 1) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i, j - 1))
				{
					StepUpFiveConnections.push_back(IdCounter - 1);					//Mid Left
				}

			if (i != MIN && j != MIN)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i - 1, j - 1) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i - 1, j - 1))
				{
					StepUpFiveConnections.push_back((IdCounter - (32 * 15)) - 2);	//Bottom Left
				}

			if (i != MIN)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i - 1, j) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i - 1, j))
				{
					StepUpFiveConnections.push_back((IdCounter - (32 * 15)) - 1);	//Botom Mid
				}

			if (i != MIN && j != MAX)
				if (tempNode->GetPosition().y + 5 >= WorldManipulation::GetHeightmap(i - 1, j + 1) && tempNode->GetPosition().y - 5 <= WorldManipulation::GetHeightmap(i - 1, j + 1))
				{
					StepUpFiveConnections.push_back((IdCounter - (32 * 15)) + 0);	//Botom Right
				}

			//- Pushing Both Node Connectors StepUpOneConnections and StepUpFiveConnections -//
			tempNode->m_connectedWaypointIDs.push_back(StepUpOneConnections);
			tempNode->m_connectedWaypointIDs.push_back(StepUpFiveConnections);

			//- Pushing created Node to Pathing map vector to be used in calculations -//
 			m_createdPathingMap.push_back(tempNode);
		}
	}

}

//- CORE FUNCTIONS -//
void RouteConstructor::A_star(STEP_UP_AMOUNT stepUpAmmount, int StartingLocation)
{
	std::vector<Nodes*> _waypoints = GetCreatedPathingMap();
	std::vector<Nodes*> _nodes;

	Nodes* _closesToTank = nullptr;
	Nodes* _closesToDestination = nullptr;
	float distanceToTank = 9999999;
	float distanceToDestination = 99999999;

	float tempish = 0.0f; float temp2ish = 0.0f;

	//- loops though all the waypoints -//
	for (int i = 0; i < _waypoints.size(); i++)
	{
		tempish = ReturnDistance(m_route[StartingLocation].startPos, _waypoints[i]->GetPosition());
		//- checking if the waypoint is the closes to current position -//
		if (distanceToTank > tempish)
		{
			//- seting it as the lowest target -//
			distanceToTank = ReturnDistance(m_route[StartingLocation].startPos, _waypoints[i]->GetPosition());
			_closesToTank = _waypoints[i];
		}
		temp2ish = ReturnDistance(m_route[StartingLocation].endPos, _waypoints[i]->GetPosition());
		//- checking if the waypoint is the closes to the target desinations -//
		if (distanceToDestination > temp2ish)
		{
			//- seting to lowest value -//
			distanceToDestination = ReturnDistance(m_route[StartingLocation].endPos, _waypoints[i]->GetPosition());
			_closesToDestination = _waypoints[i];
		}
		//- turning waypoints into custom struct to easy use -//
		_nodes.push_back(_waypoints[i]);

	}

	if (m_route[StartingLocation].m_route.size() == 0 && FinalDestinationChanged == true)
	{
		GetPath(_closesToTank, _closesToDestination, _nodes, stepUpAmmount, StartingLocation);
		FinalDestinationChanged = false;
	}

	if (m_route[StartingLocation].m_route.size() >= 1)
	{
		m_nodeToTravelTo = m_route[StartingLocation].m_route[m_route.size() - 1];
		if (m_route[StartingLocation].startPos.x > m_nodeToTravelTo->GetPosition().x && m_route[StartingLocation].startPos.x < m_nodeToTravelTo->GetPosition().x &&
			m_route[StartingLocation].startPos.y > m_nodeToTravelTo->GetPosition().y && m_route[StartingLocation].startPos.y < m_nodeToTravelTo->GetPosition().y &&
			m_route[StartingLocation].startPos.z > m_nodeToTravelTo->GetPosition().z && m_route[StartingLocation].startPos.z < m_nodeToTravelTo->GetPosition().z)
		{
			m_route[StartingLocation].m_route.erase(m_route[StartingLocation].m_route.begin() + (m_route[StartingLocation].m_route.size() - 1));
		}
	}
	else
	{
		//cout << "ARRIVED AT CLOSEST NODE" << endl;
	}

}

std::vector<Nodes*> RouteConstructor::GetPath(Nodes* starting, Nodes* ending, std::vector<Nodes*> AllNodes, STEP_UP_AMOUNT stepUpAmmount, int startingLocation)
{
	//- Decliration of vectors -//
	Nodes* CurrentLowestNode;

	m_openNodes.push_back(starting);

	CurrentLowestNode = GetNextLowestNode(m_openNodes);

	//- Running though all open Nodes -//
	while (IsNodeClosed(ending, m_closedNodes) == false)
	{
		if (IsNodeOpen(ending, m_closedNodes))
		{
			break;
		}

		//- opening new nodes  -//
		for (int j = 0; j < CurrentLowestNode->GetConnectedWaypointIDs(stepUpAmmount).size(); j++)
		{
			//- if the node is Open dont re-Open it -//
			if (IsNodeOpen(AllNodes[CurrentLowestNode->GetConnectedWaypointIDs(stepUpAmmount)[j]], m_openNodes) == false && IsNodeClosed(AllNodes[CurrentLowestNode->GetConnectedWaypointIDs(stepUpAmmount)[j]], m_closedNodes) == false)
			{
				AllNodes[CurrentLowestNode->GetConnectedWaypointIDs(stepUpAmmount)[j]]->SetParentWaypoint(CurrentLowestNode);
				CalculateWeighting(AllNodes[CurrentLowestNode->GetConnectedWaypointIDs(stepUpAmmount)[j]], CurrentLowestNode->GetPosition(), ending->GetPosition());
				m_openNodes.push_back(AllNodes[CurrentLowestNode->GetConnectedWaypointIDs(stepUpAmmount)[j]]);
			}
		}

		//- closing the current node so it cant select it self in chosing next node to move to -//
		CloseNode(CurrentLowestNode);

		//- finding the next lowest node -//
		CurrentLowestNode = GetNextLowestNode(m_openNodes);
	}

	for (int i = 0; i < m_closedNodes.size(); i++)
	{
		if (ending == m_closedNodes[i])
		{
			if (ending != starting)
			{
				GetPathResults(m_closedNodes[i], starting, startingLocation);
			}
			break;
		}
	}
	//- Retrace Steps to the Starting Node -//

	return m_route[startingLocation].m_route;

}

void RouteConstructor::GetPathResults(Nodes* parentNode, Nodes* startingNode, int startingLocation)
{
	m_route[startingLocation].m_route.push_back(parentNode);
	if (parentNode->GetParentWayPoint() != nullptr)
	{
		GetPathResults(parentNode->GetParentWayPoint(), startingNode, startingLocation);
	}
}

//- Resused Code -//
Nodes* RouteConstructor::GetNextLowestNode(std::vector<Nodes*> openList)
{
	int lowest_fCost = 9999999999999;
	Nodes* currentLowest = nullptr;
	//- go though open nodes and see whats the lowest and set it an the next node -//
	//- looping though this method gives a prefrance to later squares if they have the same _fCost -//
	for (int i = 0; i < openList.size(); i++)
	{
		//- if the node is lower then save its values-//
		if (openList[i]->GetFCost() <= lowest_fCost)
		{
			lowest_fCost = openList[i]->GetFCost();
			currentLowest = openList[i];
		}
	}

	//- return the lowest -//
	return currentLowest;
}

void RouteConstructor::CalculateWeighting(Nodes* node, DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos)
{
	node->SetGCost(node->GetParentWayPoint()->GetGCost() + 10);
	node->SetHCost(ReturnDistance(startPos, endPos));
	node->SetFCost(node->GetGCost() + node->GetHCost());
}

float RouteConstructor::ReturnDistance(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return ((abs(v1.x - v2.x) + abs(v1.z - v2.z)) / 2);
}

//- Checks For Status Of Code-//
bool RouteConstructor::IsNodeOpen(Nodes* NodeToCheck, std::vector<Nodes*> open)
{
	for (int i = 0; i < open.size(); i++)
	{
		if (NodeToCheck == open[i])
		{
			return true;
		}
	}
	return false;
}

bool RouteConstructor::IsNodeClosed(Nodes* NodeToCheck, std::vector<Nodes*> closed)
{
	for (int i = 0; i < closed.size(); i++)
	{
		if (NodeToCheck == closed[i])
		{
			return true;
		}
	}
	return false;
}

void RouteConstructor::OpenNode(Nodes* NodeToMove)
{
	for (int i = 0; i < m_closedNodes.size(); i++)
	{
		if (NodeToMove == m_closedNodes[i])
		{
			m_openNodes.push_back(NodeToMove);
			m_closedNodes.erase(m_closedNodes.begin() + i);
			break;
		}
	}
}

void RouteConstructor::CloseNode(Nodes* NodeToMove)
{
	for (int i = 0; i < m_openNodes.size(); i++)
	{
		if (NodeToMove == m_openNodes[i])
		{
			m_closedNodes.push_back(NodeToMove);
			m_openNodes.erase(m_openNodes.begin() + i);
			break;
		}
	}
}

void RouteConstructor::SetStarting(DirectX::XMFLOAT3 pos, int startingPosition)
{
	m_route[startingPosition].startPos = pos;
}

void RouteConstructor::SetEnding(DirectX::XMFLOAT3 pos, int startingPosition)
{
	m_route[startingPosition].endPos = pos;
}

//-----------------------------------------//
//------------ Acsess Function ------------//
//-----------------------------------------//

std::vector<Nodes*> RouteConstructor::GetCreatedPathingMap()
{
	return m_createdPathingMap;
}

std::vector<Nodes*> RouteConstructor::GetRoute(int startingLocation)
{
	return m_route[startingLocation].m_route;
}
