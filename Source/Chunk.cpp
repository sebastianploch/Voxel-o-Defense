#include "pch.h"
#include "Chunk.h"

using namespace DirectX;

#define CHUNK_WIDTH		32
#define CHUNK_HEIGHT	150
#define	CHUNK_DEPTH		32

Chunk::Chunk() : 
			m_cx(0),
			m_cz(0),
			m_blocks(new char[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH]),
			m_chunkGameObject(std::make_shared<ChunkObject>()) {
	//Default constructor should always be followed up with SetIndex() functions.
}

Chunk::~Chunk() {
	delete[] m_blocks;
}

void Chunk::Update(float deltaTime) {
	if (m_meshNeedsRegenerating) {
		//TODO: Regenerate mesh for m_chunkGameObject
	}
}

void Chunk::Draw(ID3D11DeviceContext* context) {
	m_chunkGameObject->Draw(context);
}

#pragma region Block Getters/Setters
const char Chunk::GetBlock(int x, int y, int z) { 
	return m_blocks[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)]; 
}

const char Chunk::GetBlock(DirectX::SimpleMath::Vector3Int pos) { 
	return GetBlock(pos.x, pos.y, pos.z); 
}

void Chunk::SetBlock(char c, int x, int y, int z) {
	m_blocks[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)] = c;
	m_meshNeedsRegenerating = true;
}

void Chunk::SetBlock(char c, SimpleMath::Vector3Int pos) {
	SetBlock(c, pos.x, pos.y, pos.z);
}
const int Chunk::GetWidth() {
	return CHUNK_WIDTH;
}
const int Chunk::GetHeight() {
	return CHUNK_HEIGHT;
}
const int Chunk::GetDepth() {
	return CHUNK_DEPTH;
}
#pragma endregion