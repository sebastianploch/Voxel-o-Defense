#include "pch.h"
#include "Chunk.h"
#include "GreedyVoxelMeshGeneration.h"
#include "ChunkHandler.h"

using namespace DirectX;

#define CHUNK_W	32
#define CHUNK_H	150
#define	CHUNK_D	32

Chunk::Chunk() : m_voxels(new char[CHUNK_W * CHUNK_H * CHUNK_D]()), m_heightmap(new int[CHUNK_W * CHUNK_D]()) {
	//Default constructor should always be followed up with Set_Index() functions.
}

Chunk::~Chunk() {
	delete[] m_voxels;
	delete[] m_heightmap;
}

void Chunk::UpdateMesh(ID3D11Device* device) {
	if (m_meshNeedsRegenerating) {
		GreedyVoxelMeshGeneration::GenerateMesh(this, device, m_chunkGameObject->GetMesh());
		m_meshNeedsRegenerating = false;
	}
}

void Chunk::Draw(ID3D11DeviceContext1* context,
				 ConstantBuffer& cb,
				 ID3D11Buffer* constantBuffer)
{
	m_chunkGameObject->Draw(context, cb, constantBuffer);
}

#pragma region Voxel Getters/Setters
const char Chunk::GetVoxel(int x, int y, int z) { 
	if (x < 0) {
		return GetNeighbourVoxel(3, x + GetWidth(), y, z);
	} else if (x >= GetWidth()) {
		return GetNeighbourVoxel(1, x - GetWidth(), y, z);
	}

	if (y < 0 || y >= GetHeight()) {
		return 0x00;
	}

	if (z < 0) {
		return GetNeighbourVoxel(2, x, y, z + GetDepth());
	} else if (z >= GetDepth()) {
		return GetNeighbourVoxel(0, x, y, z - GetDepth());
	}

	int index = x + CHUNK_W * (y + CHUNK_H * z);
	if (index >= CHUNK_W * CHUNK_H * CHUNK_D)
		return 0x00;

	return m_voxels[index]; 
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
	if (!m_voxels)
		return;

	m_voxels[index] = c;

	m_meshNeedsRegenerating = true;

	for (int i = 0; i < CHUNK_H - 1; i++) {
		//Check for two blocks of air to determine walkable space
		if (GetVoxel(x, i, z)	  == VOXEL_TYPE::AIR && 
			GetVoxel(x, i + 1, z) == VOXEL_TYPE::AIR) {
			SetHeightmap(i, x, z);
			break;
		}
	}
}

void Chunk::SetVoxel(char c, SimpleMath::Vector3Int pos) {
	SetVoxel(c, pos.x, pos.y, pos.z);
}

const int Chunk::GetHeightmap(int x, int z) {
	int index = (z * CHUNK_W) + x;
	if (index >= (CHUNK_W * CHUNK_D))
		return 0;
	if (index < 0)
		return 0;

	return m_heightmap[index];
}
const int Chunk::GetHeightmap(DirectX::SimpleMath::Vector2Int pos) {
	return GetHeightmap(pos.x, pos.y);
}

void Chunk::SetHeightmap(int val, int x, int z) {
	int index = (z * CHUNK_W) + x;
	if (index >= (CHUNK_W * CHUNK_D))
		return;
	if (index < 0)
		return;

	m_heightmap[index] = val;
}
void Chunk::SetHeightmap(int val, DirectX::SimpleMath::Vector2Int pos) {
	SetHeightmap(val, pos.x, pos.y);
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