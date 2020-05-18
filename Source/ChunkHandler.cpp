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
	int cx = static_cast<int>(floor(worldPos.x / MAP_SIZE));
	int cz = static_cast<int>(floor(worldPos.z / MAP_SIZE));

	return GetChunk(cx, cz);
}
