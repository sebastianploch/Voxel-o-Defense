#include "pch.h"
#include "Node.h"

Nodes::Nodes()
{
}

Nodes::Nodes(Nodes* itSelf, Nodes* parent)
{

}

Nodes::Nodes(int ID, DirectX::XMFLOAT3 position)
{
	m_ID = ID;
	m_position = position;
}

Nodes::~Nodes()
{
}

//---------------------------------------------------//
//------------ Access Variable Functions ------------//
//---------------------------------------------------//

std::vector<int> Nodes::GetConnectedWaypointIDs(STEP_UP_AMOUNT stepUpAmmount)
{
	return m_connectedWaypointIDs[stepUpAmmount];
}

bool Nodes::IsConnectedTo(STEP_UP_AMOUNT stepUpAmount ,int waypointIDToCheck)
{
	for (int i = 0; i < m_connectedWaypointIDs[stepUpAmount].size(); i++)
	{
		if (m_connectedWaypointIDs[stepUpAmount][i] == m_ID)
		{
			return true;
		}
	}

	return false;
}

int Nodes::GetID()
{
	return m_ID;
}

DirectX::XMFLOAT3 Nodes::GetPosition()
{
	return m_position;
}

Nodes* Nodes::GetParentWayPoint()
{
	return _parentWaypoint;
}

void Nodes::SetParentWaypoint(Nodes* parent)
{
	_parentWaypoint = parent;
}

float Nodes::GetGCost()
{
	return _gCost;
}

void Nodes::SetGCost(float passIn)
{
	_gCost = passIn;
}

float Nodes::GetHCost()
{
	return _hCost;
}

void Nodes::SetHCost(float passIn)
{
	_hCost = passIn;
}

float Nodes::GetFCost()
{
	return _fCost;
}

void Nodes::SetFCost(float passIn)
{
	_fCost = passIn;
}
