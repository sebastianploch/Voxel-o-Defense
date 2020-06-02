#include "pch.h"
#include "Model.h"

bool Model::Initialise(const std::string& filePath, Microsoft::WRL::ComPtr <ID3D11Device1> device)
{
	this->m_Translation = DirectX::XMFLOAT3(0.0f, 2.0f, -4.0f);
	this->m_Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); //set our initial scaling, rotation and translation values to 0,0,0
	this->m_Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->m_shaderType = Shaders::BASIC_SHADER;

	if (!this->LoadModel(filePath, device.Get()))
		return false;

	return true;
}

void Model::Update(float deltaTime)
{
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationX(this->m_Rotation.x) * DirectX::XMMatrixRotationY(this->m_Rotation.y) * DirectX::XMMatrixRotationZ(this->m_Rotation.z);
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(this->m_Translation.x, this->m_Translation.y, this->m_Translation.z);

	//calculate the world matrix
	DirectX::XMStoreFloat4x4(&m_worldMat, scale * rotation * translate);
}

void Model::Draw(ID3D11Buffer* pConstantBuffer, ConstantBuffer& cb,ID3D11DeviceContext1 &context)
{
	cb.world = this->GetWorldMatrix(); //transpose the matrix to the position in the world
	for (int i = 0; i < m_Meshes.size(); i++)
	{
		m_Meshes[i].Draw(pConstantBuffer, cb,context);
	}
}

void Model::Draw(ID3D11DeviceContext* context)
{

}

bool Model::LoadModel(const std::string& filePath,ID3D11Device1 * device)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_RemoveRedundantMaterials | aiProcess_ImproveCacheLocality);

	if (pScene == NULL)
	{
		OutputDebugStringA("Failed to import assimp file!");
		return false;
	}

	m_Path = filePath;
	if (GetPathToFile())
		this->ProcessNode(pScene->mRootNode, pScene, m_PathToFile,device); //Get our file location for the model which will be inserted before the texture file information
	else
		OutputDebugStringA("Failed to get path to file!");

	DirectX::XMFLOAT3 minBoundingCoord = m_Meshes[0].GetMinCoord();
	DirectX::XMFLOAT3 maxBoundingCoord = m_Meshes[0].GetMaxCoord();
	for (int i = 1; i < m_Meshes.size(); i++) //calculate the overall minimum and maximum bounding coordinates
	{
		if (m_Meshes[i].GetMinCoord().x < minBoundingCoord.x)
			minBoundingCoord.x = m_Meshes[i].GetMinCoord().x;

		if (m_Meshes[i].GetMinCoord().y < minBoundingCoord.y)
			minBoundingCoord.y = m_Meshes[i].GetMinCoord().y;

		if (m_Meshes[i].GetMinCoord().z < minBoundingCoord.z)
			minBoundingCoord.z = m_Meshes[i].GetMinCoord().z;

		if (m_Meshes[i].GetMaxCoord().x > maxBoundingCoord.x)
			maxBoundingCoord.x = m_Meshes[i].GetMaxCoord().x;

		if (m_Meshes[i].GetMaxCoord().y > maxBoundingCoord.y)
			maxBoundingCoord.y = m_Meshes[i].GetMaxCoord().y;

		if (m_Meshes[i].GetMaxCoord().z > maxBoundingCoord.z)
			maxBoundingCoord.z = m_Meshes[i].GetMaxCoord().z;
	}

	m_BoxDist = DirectX::XMFLOAT3(maxBoundingCoord.x - minBoundingCoord.x, maxBoundingCoord.y - minBoundingCoord.y, maxBoundingCoord.z - minBoundingCoord.z); //here we get the vector for the x, y and z distances
	this->m_Bounds = DirectX::BoundingBox(DirectX::XMFLOAT3(this->m_Translation.x, this->m_Translation.y, this->m_Translation.z), DirectX::XMFLOAT3(m_BoxDist.x / 2, m_BoxDist.y / 2, m_BoxDist.z / 2));

	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, std::string pathToFile,ID3D11Device1 * device)
{
	for (UINT i = 0; i < node->mNumMeshes; i++) //We take our meshes within the scene (model) loaded in
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; //set our mesh to the current mesh index
		m_Meshes.push_back(this->ProcessMesh(mesh, scene, pathToFile,device)); //push back a new created mesh to the meshes container
	}
	for (UINT j = 0; j < node->mNumChildren; j++)
	{
		this->ProcessNode(node->mChildren[j], scene, pathToFile,device); //Each node can have a child node, we want to process all nodes, think for example a tank and the barrel being a child node
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string pathToFile,ID3D11Device1 *device)
{
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex]; //Pass our meshes material through with it
	return Mesh(device, *mesh, *mat, pathToFile);
}

bool Model::GetPathToFile()
{
	int index = 0;

	for (int i = 0; i < m_Path.size(); i++) //we check each index to see if there are /'s. we get to the final / which is what we need for texturing
	{
		if (m_Path[i] == '/')
			index = i;
	}
	for (int i = 0; i < index + 1; i++)
		m_PathToFile = m_PathToFile + m_Path[i]; //for each character up until the final / we append the indexed character to our path to the file

	if (m_PathToFile.size() > 0) //simple check here to see if we received the file path correctly, if not we output to the debug string that we havent created it correctly
		return true;
	else
		return false;
}


Model::~Model()
{
	for (int i = 0; i < m_Meshes.size(); i++) //finally for cleanup here we simply just release the info for the meshes
	{
		m_Meshes[i].ReleaseObjects();
		m_Meshes[i].~Mesh();
	}
	m_Meshes.clear();
}