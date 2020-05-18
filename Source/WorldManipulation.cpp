#include "pch.h"
#include "WorldManipulation.h"
#include "ChunkHandler.h"

using namespace DirectX::SimpleMath;

void WorldManipulation::SetVoxel(char v, int x, int y, int z) {
	SetVoxel(v, Vector3Int(x, y, z));
}

void WorldManipulation::SetVoxel(char v, Vector3Int worldPos) {
	Chunk* c = ChunkHandler::GetChunk(worldPos);

	Vector3Int relativePos = Vector3Int(worldPos.x - (c->GetXIndex() * c->GetWidth()), 
										worldPos.y, 
										worldPos.z - (c->GetZIndex() * c->GetDepth()));

	c->SetVoxel(v, relativePos);
}

void WorldManipulation::SetVoxelArea(char v, int x, int y, int z, int w, int h, int d) {

}

void WorldManipulation::SetVoxelArea(char v, Vector3Int worldPos, Vector3Int size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			for (int z = 0; z < size.z; z++) {
				SetVoxel(v, Vector3Int(worldPos.x + x, worldPos.y + y, worldPos.z + z));
			}
		}
	}

}

void WorldManipulation::SetVoxelSphere(char v, int x, int y, int z, int r) {

}

void WorldManipulation::SetVoxelSphere(char v, Vector3Int worldPos, int r) {

}

void WorldManipulation::GenerateTerrainData(Chunk c) {

}
