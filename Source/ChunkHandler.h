#pragma once
#include "Chunk.h"

#define MAP_SIZE	15

class ChunkHandler {
private:
	static Chunk s_chunks[15][15];

public:
	ChunkHandler();
	~ChunkHandler() = default;

	static Chunk GetChunk(int cx, int cz);
	static Chunk GetChunk(DirectX::SimpleMath::Vector3Int worldPos);
};