#include "pch.h"
#include "ChunkHandler.h"

Chunk ChunkHandler::s_chunks[MAP_SIZE][MAP_SIZE] = { Chunk() };
ChunkHandler::_init ChunkHandler::_initialiser;

Chunk* ChunkHandler::GetChunk(int cx, int cz) {
	//Clamp input
	int iCx = std::max(0, std::min(cx, s_chunks[0][0].GetWidth() - 1));
	int iCz = std::max(0, std::min(cz, s_chunks[0][0].GetDepth() - 1));

	return &ChunkHandler::s_chunks[iCx][iCz];
}

Chunk* ChunkHandler::GetChunk(DirectX::SimpleMath::Vector3Int worldPos) {
	int cx = (int)floorf(worldPos.x / s_chunks[0][0].GetWidth());
	int cz = (int)floorf(worldPos.z / s_chunks[0][0].GetDepth());

	return GetChunk(cx, cz);
}

void ChunkHandler::UpdateChunkMeshes(ID3D11Device* device) {
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int z = 0; z < MAP_SIZE; z++) {
			s_chunks[x][z].UpdateMesh(device);
		}
	}
}

void ChunkHandler::DrawChunks(ID3D11DeviceContext* context) {
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int z = 0; z < MAP_SIZE; z++) {
			s_chunks[x][z].Draw(context);
		}
	}
}

const int ChunkHandler::GetMapSize() {
	return MAP_SIZE;
}
