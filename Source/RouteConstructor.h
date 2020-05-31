#pragma once
#ifndef ROUTE_CONSTRUCTOR
#define ROUTE_CONSTRUCTOR

//-----------//
//- Defines -//
//-----------//



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

	void SetStarting(DirectX::XMFLOAT3 pos);
	void SetEnding(DirectX::XMFLOAT3 pos);
	
	void A_star();

protected:

	//- Core Pathfinding Functions -//
	std::vector<Nodes*> GetPath(Nodes* starting, Nodes* ending, std::vector<Nodes*> AllNodes);
	void GetPathResults(Nodes* parentNode, Nodes* startingNode);

	//- Code That is Resused multiple times in functions -//
	void CalculateWeighting(Nodes* node, DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos);
	Nodes* GetNextLowestNode(std::vector<Nodes*> openList);
	float ReturnDistance(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	//- Functions To Check If Node is checked or not -//
	bool IsNodeOpen(Nodes* NodeToCheck, std::vector<Nodes*> open);
	bool IsNodeClosed(Nodes* NodeToCheck, std::vector<Nodes*> closed);
	void OpenNode(Nodes* NodeToMove);
	void CloseNode(Nodes* NodeToMove);
	void NewTarget(DirectX::XMFLOAT3 target);
	void StopMoving();

	std::vector<Nodes*> GetCreatedPathingMap();

private:

	//- Pathfinding Core Veriables -//
	std::vector<Nodes*> m_route;
	std::vector<Nodes*> m_openNodes;
	std::vector<Nodes*> m_closedNodes;
	Nodes* m_nodeToTravelTo = nullptr;

	bool FinalDestinationChanged = true;

	DirectX::XMFLOAT3 m_targetPos;
	DirectX::XMFLOAT3 m_StartingPos;


	std::vector<Nodes*> m_createdPathingMap;

};
#endif // !ROUTE_CONSTRUCTOR
