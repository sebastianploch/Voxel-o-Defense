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
//-- Enums --//
//-----------//

enum STEP_UP_AMOUNT
{
	ONE = 0,
	FIVE = 1
};

//-----------//
//-- Class --//
//-----------//

class Nodes {
public:
	Nodes();
	Nodes(Nodes* itSelf, Nodes* parent);
	//- Node Constructor when first creating -//
	Nodes(int ID, DirectX::XMFLOAT3 position);
	~Nodes();

	//- Core -//
	std::vector<int> GetConnectedWaypointIDs(STEP_UP_AMOUNT stepUpAmmount);
	bool IsConnectedTo(STEP_UP_AMOUNT stepUpAmount, int waypointIDToCheckint);
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

	std::vector<std::vector<int>> m_connectedWaypointIDs;


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
