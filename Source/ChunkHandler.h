#pragma once
#include "Chunk.h"
#include "WorldManipulation.h"
#include "ShaderManager.h"
#include "PerlinNoise.h"

#define MAP_SIZE 15

class ChunkHandler {
private:
	static Chunk s_chunks[MAP_SIZE][MAP_SIZE];

public:
	static Chunk* GetChunk(int cx, int cz);
	static Chunk* GetChunk(DirectX::SimpleMath::Vector3Int worldPos);

	static void UpdateChunkMeshes(ID3D11Device* device, ID3D11DeviceContext* context);
	static void DrawChunks(ID3D11DeviceContext1* context, ShaderManager* shaderManager);

	static const int GetMapSize();

	//Used to initialise without needing an instance
	static class _init {
	public:
		_init() {
			PerlinNoise::RandomiseSeed();

			//Set Chunk Indices
			for (int x = 0; x < MAP_SIZE; x++) {
				for (int z = 0; z < MAP_SIZE; z++) {
					ChunkHandler::s_chunks[x][z].SetXIndex(x);
					ChunkHandler::s_chunks[x][z].SetZIndex(z);
					WorldManipulation::GenerateTerrainData(&s_chunks[x][z]);
				}
			}
		}
	} _initialiser;
};