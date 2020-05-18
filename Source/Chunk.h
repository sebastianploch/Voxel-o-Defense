#pragma once
#include "ChunkObject.h"

class Chunk {
private:
	char* m_blocks;
	int m_cx, m_cz;	//Chunk X and Z index

	std::shared_ptr<ChunkObject> m_chunkGameObject;
	bool m_meshNeedsRegenerating = false;

public:
	Chunk();
	~Chunk(); 

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* context);

	const char GetBlock(int x, int y, int z);
	const char GetBlock(DirectX::SimpleMath::Vector3Int pos);
	void SetBlock(char c, int x, int y, int z);
	void SetBlock(char c, DirectX::SimpleMath::Vector3Int pos);

	const int GetWidth();
	const int GetHeight();
	const int GetDepth();

	inline void SetXIndex(int x) { m_cx = x; }
	inline void SetZIndex(int z) { m_cz = z; }
	inline const int GetXIndex() { return m_cx; }
	inline const int GetZIndex() { return m_cz; }
};
