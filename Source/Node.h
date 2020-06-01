#pragma once
#ifndef NODES
#define NODES

//-----------//
//- Defines -//
//-----------//



//------------//
//- Includes -//
//------------//
#include <DirectXMath.h>
#include <Vector>

//-----------//
//-- Class --//
//-----------//

class Nodes {
public:
	Nodes();
	Nodes(Nodes* itSelf, Nodes* parent);
	//- Node Constructor when first creating -//
	Nodes(int ID, DirectX::XMFLOAT3 position);
	void SecondaryNodeCreation(std::vector<int> connections, DirectX::XMFLOAT3 position);
	~Nodes();

	//- Core -//
	std::vector<int> GetConnectedWaypointIDs();
	bool IsConnectedTo(int waypointIDToCheck);
	int GetID();
	DirectX::XMFLOAT3 GetPosition();
	Nodes* GetParentWayPoint();
	void SetParentWaypoint(Nodes* parent);

	//- Get Set Costs -//
	float GetGCost();
	void SetGCost(float passIn);
	float GetHCost();
	void SetHCost(float passIn);
	float GetFCost();
	void SetFCost(float passIn);

	std::vector<int> m_connectedWaypointIDs;


private:
	//- Pointers to Given Waypoint struct and parentNode -//
	Nodes* _parentWaypoint = nullptr; ////- parentNode -//

	//- Critical Node Information -//
	int m_ID;
	DirectX::XMFLOAT3 m_position;

	//- Cost Of Travel -//
	float _gCost = 0; ////- ditance to startPoint -//
	float _hCost = 0; ////- distance to destination -//
	float _fCost = 0; ////- total cost of the values -//
};
#endif // !NODES
