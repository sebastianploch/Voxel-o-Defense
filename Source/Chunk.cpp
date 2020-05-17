#include "pch.h"
#include "Chunk.h"

using namespace DirectX;

Chunk::Chunk(short cx, short cz) : 
			m_cx(cx), 
			m_cz(cz), 
			m_blocks(new char[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH]),
			m_chunkGameObject(std::make_shared<ChunkObject>()) {

	//TODO: Generate mesh for m_chunkGameObject
}

Chunk::~Chunk() {
	delete m_blocks;
}

void Chunk::Update(float deltaTime) {
	if (m_meshNeedsRegenerating) {
		//TODO: Regenerate mesh for m_chunkGameObject
	}
}

void Chunk::Draw(ID3D11DeviceContext* context) {
	m_chunkGameObject->Draw(context);
}

void Chunk::SetBlock(char c, int x, int y, int z) {
	m_blocks[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)] = c;
	m_meshNeedsRegenerating = true;
}

void Chunk::SetBlock(char c, SimpleMath::Vector3Int pos) {
	SetBlock(c, pos.x, pos.y, pos.z);
}