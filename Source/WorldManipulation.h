#pragma once
#include "Chunk.h"

class WorldManipulation {
public:
	static const char GetVoxel(int x, int y, int z);
	static const char GetVoxel(DirectX::SimpleMath::Vector3Int worldPos);

	static void SetVoxel(char v, int x, int y, int z);
	static void SetVoxel(char v, DirectX::SimpleMath::Vector3Int worldPos);
	static void SetVoxelArea(char v, int x, int y, int z, int w, int h, int d);
	static void SetVoxelArea(char v, DirectX::SimpleMath::Vector3Int worldPos, DirectX::SimpleMath::Vector3Int size);
	static void SetVoxelSphere(char v, int x, int y, int z, int r);
	static void SetVoxelSphere(char v, DirectX::SimpleMath::Vector3Int worldPos, int r);

	static void GenerateTerrainData(Chunk* c);
	
private:
	static char GetVoxelType(DirectX::SimpleMath::Vector3Int pos, int terrainHeight);
	static DirectX::SimpleMath::Vector3Int GetRelativePos(Chunk* c, DirectX::SimpleMath::Vector3Int worldPos);
};