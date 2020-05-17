#include "pch.h"
#include "ChunkHandler.h"

ChunkHandler::ChunkHandler() {
	//Create chunks
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int z = 0; z < MAP_SIZE; z++) {
			ChunkHandler::s_chunks[x][z] = Chunk(x, z);
		}
	}
}

Chunk ChunkHandler::GetChunk(int cx, int cz) {
	//Clamp input
	int iCx = std::max(0, std::min(cx, CHUNK_WIDTH-1));
	int iCz = std::max(0, std::min(cx, CHUNK_DEPTH-1));

	return ChunkHandler::s_chunks[iCx][iCz];
}

Chunk ChunkHandler::GetChunk(DirectX::SimpleMath::Vector3Int worldPos) {
	int cx = floor(worldPos.x / MAP_SIZE);
	int cz = floor(worldPos.z / MAP_SIZE);

	return GetChunk(cx, cz);
}
