#pragma once
#include "Chunk.h"
class GreedyVoxelMeshGeneration {
public:
	static void GenerateMesh(Chunk* chunk, ID3D11Device* device, ID3D11DeviceContext* context, VoxelMesh* mesh);


private:
	static void CreateBuffers(ID3D11Device* device, ID3D11DeviceContext* context, VoxelMesh* mesh,
								std::vector<DirectX::SimpleMath::Vector3>* vertices, 
								std::vector<int>* indices, 
								std::vector<DirectX::SimpleMath::Vector3>* normals, 
								std::vector<DirectX::SimpleMath::Vector2>* uvs, 
								std::vector<DirectX::SimpleMath::Vector2>* uvs1);

	static void GenerateQuad(DirectX::SimpleMath::Vector3 v1, 
							DirectX::SimpleMath::Vector3 v2, 
							DirectX::SimpleMath::Vector3 v3, 
							DirectX::SimpleMath::Vector3 v4, 
							DirectX::SimpleMath::Vector3 normal,
							std::vector<DirectX::SimpleMath::Vector3>* vertices, 
							std::vector<int>* indices, 
							std::vector<DirectX::SimpleMath::Vector3>* normals, 
							std::vector<DirectX::SimpleMath::Vector2>* uvs, 
							std::vector<DirectX::SimpleMath::Vector2>* uvs1,
							int w, int h, int vox);

	static std::vector<DirectX::SimpleMath::Vector2> GenerateCorrectUVs(DirectX::SimpleMath::Vector3 normal, int w, int h);

	static DirectX::SimpleMath::Vector3 GetNormal(bool isBackFace, int axis);
};