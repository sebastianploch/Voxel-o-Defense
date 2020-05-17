#pragma once
#include "ChunkObject.h"

#define CHUNK_WIDTH		32
#define CHUNK_HEIGHT	150
#define	CHUNK_DEPTH		32

class Chunk {
private:
	char* m_blocks;
	short m_cx, m_cz;	//Chunk X and Z index

	std::shared_ptr<ChunkObject> m_chunkGameObject;
	bool m_meshNeedsRegenerating = false;

public:
	Chunk(short cx, short cz);
	~Chunk(); 

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* context);

	inline const char GetBlock(int x, int y, int z) { return m_blocks[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)]; }
	inline const char GetBlock(DirectX::SimpleMath::Vector3Int pos) { return GetBlock(pos.x, pos.y, pos.z); }

	void SetBlock(char c, int x, int y, int z);
	void SetBlock(char c, DirectX::SimpleMath::Vector3Int pos);
};
