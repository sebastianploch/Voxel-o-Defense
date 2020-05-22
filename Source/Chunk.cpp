#include "pch.h"
#include "Chunk.h"
#include "GreedyVoxelMeshGeneration.h"
#include "ChunkHandler.h"

using namespace DirectX;

#define CHUNK_W	32
#define CHUNK_H	150
#define	CHUNK_D	32

Chunk::Chunk() : m_blocks(new char[CHUNK_W * CHUNK_H * CHUNK_D]()) {
	//Default constructor should always be followed up with Set_Index() functions.
}

Chunk::~Chunk() {
	delete[] m_blocks;
}

void Chunk::UpdateMesh(ID3D11Device* device) {
	if (m_meshNeedsRegenerating) {
		m_chunkGameObject->UpdateMesh(GreedyVoxelMeshGeneration::GenerateMesh(this, device));
		m_meshNeedsRegenerating = false;
	}
}

void Chunk::Draw(ID3D11DeviceContext* context) {
	m_chunkGameObject->Draw(context);
}

#pragma region Block Getters/Setters
const char Chunk::GetVoxel(int x, int y, int z) { 
	if (x < 0) {
		return GetNeighbourVoxel(3, x + GetWidth(), y, z);
	} else if (x >= GetWidth()) {
		return GetNeighbourVoxel(1, x - GetWidth(), y, z);
	}

	if (y < 0 || y >= GetHeight()) {
		return (char)0;
	}

	if (z < 0) {
		return GetNeighbourVoxel(2, x, y, z + GetDepth());
	} else if (z >= GetDepth()) {
		return GetNeighbourVoxel(0, x, y, z - GetDepth());
	}

	int index = x + CHUNK_W * (y + CHUNK_H * z);
	return m_blocks[index]; 
}

const char Chunk::GetVoxel(DirectX::SimpleMath::Vector3Int pos) { 
	return GetVoxel(pos.x, pos.y, pos.z); 
}

const char Chunk::GetNeighbourVoxel(int dir, int x, int y, int z) {
	Chunk* chunk = GetNeighbour(dir);

	if (chunk != nullptr) {
		return chunk->GetVoxel(x, y, z);
	} else {
		return (char)0;
	}
}

const char Chunk::GetNeighbourVoxel(int dir, DirectX::SimpleMath::Vector3Int pos) {
	return GetNeighbourVoxel(dir, pos.x, pos.y, pos.z);
}

Chunk* Chunk::GetNeighbour(int dir) {
	switch (dir) {
	case 0:
		if (m_cz == ChunkHandler::GetMapSize() - 1)
			return nullptr;
		return ChunkHandler::GetChunk(m_cx, m_cz + 1);  //North

	case 1:
		if (m_cx == ChunkHandler::GetMapSize() - 1)
			return nullptr;
		return ChunkHandler::GetChunk(m_cx + 1, m_cz);  //East

	case 2:
		if (m_cz == 0)
			return nullptr;
		return ChunkHandler::GetChunk(m_cx, m_cz - 1);  //South

	case 3:
		if (m_cx == 0)
			return nullptr;
		return ChunkHandler::GetChunk(m_cx - 1, m_cz);  //West

	default:
		return nullptr;
	}
}

void Chunk::SetVoxel(char c, int x, int y, int z) {
	int index = x + CHUNK_W * (y + CHUNK_H * z);
	if (index >= (CHUNK_W * CHUNK_H * CHUNK_D))
		return;
	if (index < 0)
		return;

	m_blocks[index] = c;
	m_meshNeedsRegenerating = true;
}

void Chunk::SetVoxel(char c, SimpleMath::Vector3Int pos) {
	SetVoxel(c, pos.x, pos.y, pos.z);
}
const int Chunk::GetWidth() {
	return CHUNK_W;
}
const int Chunk::GetHeight() {
	return CHUNK_H;
}
const int Chunk::GetDepth() {
	return CHUNK_D;
}
#pragma endregion