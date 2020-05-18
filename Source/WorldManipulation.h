#pragma once
#include "Chunk.h"

class WorldManipulation {
public:
	static void SetVoxel(char v, int x, int y, int z);
	static void SetVoxel(char v, DirectX::SimpleMath::Vector3Int worldPos);
	static void SetVoxelArea(char v, int x, int y, int z, int w, int h, int d);
	static void SetVoxelArea(char v, DirectX::SimpleMath::Vector3Int worldPos, DirectX::SimpleMath::Vector3Int size);
	static void SetVoxelSphere(char v, int x, int y, int z, int r);
	static void SetVoxelSphere(char v, DirectX::SimpleMath::Vector3Int worldPos, int r);

	static void GenerateTerrainData(Chunk c);
};