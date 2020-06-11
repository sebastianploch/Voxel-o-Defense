#pragma once
#include <Importer.hpp>
#include <config.h>
#include <cimport.h>
#include <postprocess.h>
#include <scene.h>
class Mesh
{
public:
	Mesh() {};
	Mesh(ID3D11Device1* device, aiMesh& mesh, aiMaterial& material, std::string pathToFile);
	void Draw(ID3D11Buffer* pConstantBuffer, ConstantBuffer& cb,ID3D11DeviceContext1 &context);
	~Mesh();
	Geometry m_MeshGeometry;
	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_Texture;
	void ReleaseObjects();
	DirectX::XMFLOAT3 GetMinCoord() { return m_MinCoord; }
	DirectX::XMFLOAT3 GetMaxCoord() { return m_MaxCoord; }
private:
	ID3D11ShaderResourceView* CreateTextureFromFile(std::string filePath, ID3D11Device1* device);
	DirectX::XMFLOAT3 m_MinCoord;
	DirectX::XMFLOAT3 m_MaxCoord;
	std::string m_PathToFile;
};