#pragma once
#include "IGameObject.h"
#include "Mesh.h"
// Assimp Library
#include <assimp/Importer.hpp>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model : public IGameObject
{
public:
	Model() {}
	bool Initialise(const std::string& filePath, Microsoft::WRL::ComPtr <ID3D11Device1> device);
	void Update(float deltaTime) override;
	void Draw(ID3D11Buffer* pConstantBuffer, ConstantBuffer& cb,ID3D11DeviceContext1 &context);
	void Draw(ID3D11DeviceContext* context) override;

	void SetScale(DirectX::XMFLOAT3 scale) { this->m_Scale = scale; m_Bounds.Extents = DirectX::XMFLOAT3((m_BoxDist.x / 2) * scale.x, (m_BoxDist.y / 2) * scale.y, (m_BoxDist.z / 2) * scale.z); }
	void SetRotation(DirectX::XMFLOAT3 rotation) { this->m_Rotation = rotation; }
	void SetTranslation(DirectX::XMFLOAT3 translation) { this->m_Translation = translation; m_Bounds.Center = this->m_Translation; }

	float GetFeetPos() { return this->m_Translation.y - (m_Bounds.Extents.y / 2); }
	DirectX::XMFLOAT3 GetScale() { return this->m_Scale; }
	DirectX::XMFLOAT3 GetRotation() { return this->m_Rotation; }
	DirectX::XMFLOAT3 GetTranslation() { return this->m_Translation; }

	DirectX::BoundingBox GetBounds() { return this->m_Bounds; }
	~Model();

private:
	bool LoadModel(const std::string& filePath, ID3D11Device1* device);
	void ProcessNode(aiNode* node, const aiScene* scene, std::string pathToFile,ID3D11Device1* device);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string pathToFile,ID3D11Device1* device);
	bool GetPathToFile();

	std::vector<Mesh>m_Meshes;
	std::string m_Path;
	std::string m_PathToFile;

	DirectX::XMFLOAT3 m_Translation;
	DirectX::XMFLOAT3 m_Rotation;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_BoxDist;

	DirectX::BoundingBox m_Bounds;
};