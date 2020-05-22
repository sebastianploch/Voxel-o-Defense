#include "pch.h"
#include "Node.h"

Nodes::Nodes()
{
}

Nodes::Nodes(Nodes* itSelf, Nodes* parent)
{

}

Nodes::~Nodes()
{
}

//---------------------------------------------------//
//------------ Access Variable Functions ------------//
//---------------------------------------------------//

std::vector<int> Nodes::GetConnectedWaypointIDs()
{
	return m_connectedWaypointIDs;
}

bool Nodes::IsConnectedTo(int waypointIDToCheck)
{
	for (int i = 0; i < m_connectedWaypointIDs.size(); i++)
	{
		if (m_connectedWaypointIDs[i] == m_ID)
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
	return DirectX::XMFLOAT3();
}

Nodes* Nodes::GetParentWayPoint()
{
	return nullptr;
}

float Nodes::GetGCost()
{
	return 0.0f;
}

void Nodes::SetGCost(float passIn)
{
}

float Nodes::GetHCost()
{
	return 0.0f;
}

void Nodes::SetHCost(float passIn)
{
}

float Nodes::GetFCost()
{
	return 0.0f;
}

void Nodes::SetFCost(float passIn)
{
}
