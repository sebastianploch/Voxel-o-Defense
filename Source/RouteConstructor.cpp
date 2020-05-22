#include "pch.h"
#include "RouteConstructor.h"

RouteConstructor::RouteConstructor()
{
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
}

//- CORE FUNCTIONS -//
void RouteConstructor::A_star(AiAgent* agent, DirectX::XMFLOAT3 Destination)
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
		tempish = ReturnDistance(agent->GetPosition(), _waypoints[i]->GetPosition());
		//- checking if the waypoint is the closes to current position -//
		if (distanceToTank > tempish)
		{
			//- seting it as the lowest target -//
			distanceToTank = ReturnDistance(agent->GetPosition(), _waypoints[i]->GetPosition());
			_closesToTank = _waypoints[i];
		}
		temp2ish = ReturnDistance(Destination, _waypoints[i]->GetPosition());
		//- checking if the waypoint is the closes to the target desinations -//
		if (distanceToDestination > temp2ish)
		{
			//- seting to lowest value -//
			distanceToDestination = ReturnDistance(Destination, _waypoints[i]->GetPosition());
			_closesToDestination = _waypoints[i];
		}
		//- turning waypoints into custom struct to easy use -//
		_nodes.push_back(_waypoints[i]);

	}

	if (m_route.size() == 0 && agent->HasFinalDestinationChanged() == true)
	{
		GetPath(agent, _closesToTank, _closesToDestination, _nodes);
		agent->endingDestinationChanged(false);
	}

	if (m_route.size() >= 1)
	{
		m_nodeToTravelTo = m_route[m_route.size() - 1];
		if (agent->GetPosition().x > m_nodeToTravelTo->GetPosition().x && agent->GetPosition().x < m_nodeToTravelTo->GetPosition().x &&
			agent->GetPosition().y > m_nodeToTravelTo->GetPosition().y && agent->GetPosition().y < m_nodeToTravelTo->GetPosition().y &&
			agent->GetPosition().z > m_nodeToTravelTo->GetPosition().z && agent->GetPosition().z < m_nodeToTravelTo->GetPosition().z)
		{
			m_route.erase(m_route.begin() + (m_route.size() - 1));
		}
	}
	else
	{
		//cout << "ARRIVED AT CLOSEST NODE" << endl;
	}

}

std::vector<Nodes*> RouteConstructor::GetPath(AiAgent* agent, Nodes* starting, Nodes* ending, std::vector<Nodes*> AllNodes)
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
		for (int j = 0; j < CurrentLowestNode->GetConnectedWaypointIDs().size(); j++)
		{
			//- if the node is Open dont re-Open it -//
			if (IsNodeOpen(AllNodes[CurrentLowestNode->GetConnectedWaypointIDs()[j]], m_openNodes) == false)
			{
				Nodes* temp = new Nodes(AllNodes[CurrentLowestNode->GetConnectedWaypointIDs()[j]], CurrentLowestNode);
				CalculateWeighting(agent, temp, starting->GetPosition(), ending->GetPosition());
				m_openNodes.push_back(temp);
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
				GetPathResults(m_closedNodes[i], starting);
			}
			break;
		}
	}
	//- Retrace Steps to the Starting Node -//

	return m_route;

}

void RouteConstructor::GetPathResults(Nodes* parentNode, Nodes* startingNode)
{
	m_route.push_back(parentNode);
	if (parentNode->GetParentWayPoint() != startingNode)
	{
		GetPathResults(parentNode->GetParentWayPoint(), startingNode);
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

void RouteConstructor::CalculateWeighting(AiAgent* agent,Nodes* node, DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos)
{
	node->SetGCost(node->GetParentWayPoint()->GetGCost() + 10);
	node->SetHCost(ReturnDistance(agent->GetPosition(), endPos));
	node->SetFCost(node->GetGCost() + node->GetHCost());
}

float RouteConstructor::ReturnDistance(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return ((abs(v1.x - v2.x) + abs(v1.y - v2.y)) / 2);
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

void RouteConstructor::NewTarget(AiAgent* agent, DirectX::XMFLOAT3 target)
{
	m_targetPos = target;
	agent->endingDestinationChanged(true);
	m_route.clear();
	m_closedNodes.clear();
	m_openNodes.clear();
}

void RouteConstructor::StopMoving(AiAgent* agent)
{
	NewTarget(agent, agent->GetPosition());
	agent->CompleatlyStopAgent(true);
}

//-----------------------------------------//
//------------ Acsess Function ------------//
//-----------------------------------------//

std::vector<Nodes*> RouteConstructor::GetCreatedPathingMap()
{
	return m_createdPathingMap;
}
