#pragma once
#ifndef ROUTE_CONSTRUCTOR
#define ROUTE_CONSTRUCTOR

//-----------//
//- Defines -//
//-----------//

#define SizeOfVoxel 1
#define MAX 479
#define MIN 0


//------------//
//- Includes -//
//------------//

//#include "AiManager.h"
#include "AiAgent.h"
#include "Node.h"
#include <DirectXMath.h>

//-----------//
//-- Enums --//
//-----------//

//------------//
//-- Struct --//
//------------//

struct routes
{
	DirectX::SimpleMath::Vector3 startPos;
	DirectX::SimpleMath::Vector3 endPos;
	std::vector<std::vector<Nodes*>> m_route;
	bool FinalDestinationChanged = true;
};

//-----------//
//-- Class --//
//-----------//
class  RouteConstructor
{
public:
	RouteConstructor();
	~RouteConstructor();

	void Update();
	void CreatePathfindingMap();
	void UpdatePathfindingMap();

	void SetStarting(DirectX::XMFLOAT3 pos, int startingPosition);
	void SetEnding(DirectX::XMFLOAT3 pos, int endingPosition);
	
	void A_star(int startingLocation);

	std::vector<Nodes*> GetCreatedPathingMap();

	std::vector<Nodes*> GetRoute(int startingLocation, STEP_UP_AMOUNT stepup);

protected:

	//- Core Pathfinding Functions -//
	void GetPath(Nodes* starting, Nodes* ending, std::vector<Nodes*> AllNodes, STEP_UP_AMOUNT stepUpAmmount, int startingLocation);
	void GetPathResults(Nodes* parentNode, Nodes* startingNode, int startingLocation, STEP_UP_AMOUNT stepup);

	//- Code That is Resused multiple times in functions -//
	void CalculateWeighting(Nodes* node, DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos);
	Nodes* GetNextLowestNode(std::vector<Nodes*> openList);
	float ReturnDistance(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	//- Functions To Check If Node is checked or not -//
	bool IsNodeOpen(Nodes* NodeToCheck, std::vector<Nodes*> open);
	bool IsNodeClosed(Nodes* NodeToCheck, std::vector<Nodes*> closed);
	void OpenNode(Nodes* NodeToMove);
	void CloseNode(Nodes* NodeToMove);

private:

	//- Pathfinding Core Veriables -//
	std::vector<routes> m_route;
	std::vector<Nodes*> m_openNodes;
	std::vector<Nodes*> m_closedNodes;
	Nodes* m_nodeToTravelTo = nullptr;

	std::vector<Nodes*> m_createdPathingMap;

};
#endif // !ROUTE_CONSTRUCTOR
