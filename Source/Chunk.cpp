#include "pch.h"
#include "Chunk.h"
#include "GreedyVoxelMeshGeneration.h"

using namespace DirectX;

#define CHUNK_W	32
#define CHUNK_H	150
#define	CHUNK_D	32

Chunk::Chunk() : m_blocks(new char[CHUNK_W * CHUNK_H * CHUNK_D]) {
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
	int index = x + CHUNK_W * (y + CHUNK_H * z);
	if (index >= (CHUNK_W * CHUNK_H * CHUNK_D))
		return 0x00;
	if (index < 0)
		return 0x00;

	return m_blocks[index]; 
}

const char Chunk::GetVoxel(DirectX::SimpleMath::Vector3Int pos) { 
	return GetVoxel(pos.x, pos.y, pos.z); 
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