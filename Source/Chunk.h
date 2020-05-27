#pragma once
#include "ChunkObject.h"

class Chunk {
private:
	char* m_blocks;
	int m_cx = 0, m_cz = 0;	//Chunk X and Z index

	std::shared_ptr<ChunkObject> m_chunkGameObject = std::make_shared<ChunkObject>();
	bool m_meshNeedsRegenerating = true;

public:
	Chunk();
	~Chunk(); 

	void UpdateMesh(ID3D11Device* device);
	void Draw(ID3D11DeviceContext1* context);

	const char GetVoxel(int x, int y, int z);
	const char GetVoxel(DirectX::SimpleMath::Vector3Int pos);
	const char GetNeighbourVoxel(int dir, int x, int y, int z);
	const char GetNeighbourVoxel(int dir, DirectX::SimpleMath::Vector3Int pos);
	Chunk* GetNeighbour(int dir);
	void SetVoxel(char v, int x, int y, int z);
	void SetVoxel(char v, DirectX::SimpleMath::Vector3Int pos);

	const int GetWidth();
	const int GetHeight();
	const int GetDepth();

	inline void SetXIndex(int x) { m_cx = x; }
	inline void SetZIndex(int z) { m_cz = z; }
	inline const int GetXIndex() { return m_cx; }
	inline const int GetZIndex() { return m_cz; }

	std::shared_ptr<ChunkObject> GetChunkGameObject() { return m_chunkGameObject; }
};
