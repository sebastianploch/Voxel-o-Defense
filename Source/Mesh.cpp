#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ID3D11Device1 * device, aiMesh& mesh, aiMaterial& material, std::string pathToFile)
{
    this->m_PathToFile = pathToFile;
    //------------------------------------------------------------------------------------------------
    //Firstly we initialise and create our vertices container.
    unsigned int numOfVertices = mesh.mNumVertices;
    DirectX::VertexPositionNormalTexture* vertices = new DirectX::VertexPositionNormalTexture[numOfVertices];

    m_MinCoord = DirectX::XMFLOAT3(mesh.mVertices[0].x, mesh.mVertices[0].y, mesh.mVertices[0].z);
    m_MaxCoord = DirectX::XMFLOAT3(mesh.mVertices[0].x, mesh.mVertices[0].y, mesh.mVertices[0].z);

    for (UINT i = 0; i < numOfVertices; i++)
    {
        DirectX::VertexPositionNormalTexture vertex = {};
        if (mesh.mNumVertices > 0) //If we have vertices and normals push them back
        {
            vertex.position.x = mesh.mVertices[i].x;
            vertex.position.y = mesh.mVertices[i].y;
            vertex.position.z = mesh.mVertices[i].z;

            //Check the positions to create a bounding box for the model
            if (vertex.position.x < m_MinCoord.x)
                m_MinCoord.x = vertex.position.x;
            if (vertex.position.y < m_MinCoord.y)
                m_MinCoord.y = vertex.position.y;
            if (vertex.position.z < m_MinCoord.z)
                m_MinCoord.z = vertex.position.z;

            if (vertex.position.x > m_MaxCoord.x)
                m_MaxCoord.x = vertex.position.x;
            if (vertex.position.y > m_MaxCoord.y)
                m_MaxCoord.y = vertex.position.y;
            if (vertex.position.z > m_MaxCoord.z)
                m_MaxCoord.z = vertex.position.z;
        }

        if (mesh.mTextureCoords[0]) //Check the first channel for coords, further channels are used for cube mapping e.t.c
        {
            vertex.textureCoordinate.x = (float)mesh.mTextureCoords[0][i].x;
            vertex.textureCoordinate.y = (float)mesh.mTextureCoords[0][i].y;
        }

        if (mesh.HasNormals())
        {
            vertex.normal.x = mesh.mNormals[i].x;
            vertex.normal.y = mesh.mNormals[i].y;
            vertex.normal.z = mesh.mNormals[i].z;
        }
        vertices[i] = vertex;
    }
    //------------------------------------------------------------------------------------------------

    unsigned int numOfIndices = 0; //initialise a container size for the number of indices
    for (UINT i = 0; i < mesh.mNumFaces; i++)
    {
        aiFace face = mesh.mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++) //for each index for each face in the mesh, we add to the total numOfIndices
        {
            numOfIndices++;
        }
    }

    WORD* indices = new WORD[numOfIndices]; //Create and initialise the num of indices container
    int currInd = 0; //we need to hold the current index we are on

    for (UINT i = 0; i < mesh.mNumFaces; i++)
    {
        aiFace face = mesh.mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++) //for each face within the mesh, we store it's indices
        {
            indices[currInd] = (WORD)face.mIndices[j];
            currInd++;
        }
    }
    //------------------------------------------------------------------------------------------------

    D3D11_BUFFER_DESC vertexDesc; //Create the vertex and index buffer descriptions
    ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.ByteWidth = sizeof(DirectX::VertexPositionNormalTexture) * numOfVertices;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = vertices;

    device->CreateBuffer(&vertexDesc, &initData, &m_MeshGeometry.vertexBuffer);

    D3D11_BUFFER_DESC indexDesc;
    ZeroMemory(&indexDesc, sizeof(indexDesc));
    indexDesc.Usage = D3D11_USAGE_DEFAULT;
    indexDesc.ByteWidth = sizeof(WORD) * numOfIndices;
    indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory(&indexData, sizeof(indexData));
    indexData.pSysMem = indices;

    device->CreateBuffer(&indexDesc, &indexData, &m_MeshGeometry.indexBuffer);
    //------------------------------------------------------------------------------------------------

    m_MeshGeometry.vertexBufferStride = sizeof(DirectX::VertexPositionNormalTexture); //Create the vertex buffer stride
    m_MeshGeometry.vertexBufferOffset = 0;
    m_MeshGeometry.numberOfIndices = numOfIndices;
    //------------------------------------------------------------------------------------------------
    aiString texturePath;
    this->m_Texture = nullptr;

    unsigned int texCountSpecular = material.GetTextureCount(aiTextureType_SPECULAR);
    unsigned int texCountDiffuse = material.GetTextureCount(aiTextureType_DIFFUSE); //Get the count for the diffuse, ambient and specular textures
    unsigned int texCountAmbient = material.GetTextureCount(aiTextureType_AMBIENT);

    //Check if the textures exist, if so, set the texture
    if (texCountDiffuse > 0)
    {
        material.GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
        this->m_Texture = CreateTextureFromFile(texturePath.C_Str(), device);
    }

    if (texCountAmbient > 0)
    {
        material.GetTexture(aiTextureType_AMBIENT, 0, &texturePath);
        this->m_Texture = CreateTextureFromFile(texturePath.C_Str(), device);
    }

    if (texCountSpecular > 0)
    {
        material.GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
        this->m_Texture = CreateTextureFromFile(texturePath.C_Str(), device);
    }
    //------------------------------------------------------------------------------------------------
    //Finally, create the material values from our material passed in
    aiColor4D matDiffuse;
    aiColor4D matAmbient;
    aiColor4D matSpecular;
    ai_real specularPower;

    material.Get(AI_MATKEY_COLOR_DIFFUSE, matDiffuse);
    material.Get(AI_MATKEY_COLOR_AMBIENT, matAmbient);
    material.Get(AI_MATKEY_COLOR_SPECULAR, matSpecular);
    material.Get(AI_MATKEY_SHININESS, specularPower);

    //------------------------------------------------------------------------------------------------
}

void Mesh::Draw(ID3D11Buffer* pConstantBuffer, ConstantBuffer& cb,ID3D11DeviceContext1 &context)
{
    context.PSSetShaderResources(0, 1, m_Texture.GetAddressOf());
    context.IASetVertexBuffers(0, 1, &this->m_MeshGeometry.vertexBuffer, &this->m_MeshGeometry.vertexBufferStride, &this->m_MeshGeometry.vertexBufferOffset);
    context.IASetIndexBuffer(this->m_MeshGeometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    context.UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
    context.DrawIndexed(this->m_MeshGeometry.numberOfIndices, 0, 0);
}

Mesh::~Mesh()
{
}

void Mesh::ReleaseObjects()
{
    if (this->m_MeshGeometry.vertexBuffer)
        this->m_MeshGeometry.vertexBuffer->Release();

    if (this->m_MeshGeometry.indexBuffer)
        this->m_MeshGeometry.indexBuffer->Release();

    //if (this->m_Texture)
    //    this->m_Texture->Release();
}

ID3D11ShaderResourceView* Mesh::CreateTextureFromFile(std::string texturePath,ID3D11Device1 * device)
{
    std::string fullPath = m_PathToFile + texturePath; //Firstly we get our full path

    const char* pathAsCharArr = texturePath.c_str(); //Then we need to convert our full path to a char array

    std::string extension;
    extension = extension + pathAsCharArr[texturePath.size() - 5] + pathAsCharArr[texturePath.size() - 4] + pathAsCharArr[texturePath.size() - 3] + pathAsCharArr[texturePath.size() - 2] + pathAsCharArr[texturePath.size() - 1];

    ID3D11ShaderResourceView* texture = nullptr;
    std::wstring textureFilePath;

    for (int i = 0; i < fullPath.length(); ++i)
        textureFilePath += wchar_t(fullPath[i]); //here we are reformatting the filePath so it can be used for our dds and WIC texture loaders

    const wchar_t* reformattedPath = textureFilePath.c_str();

    if (extension[0] == '.')
    {
        DirectX::CreateWICTextureFromFile(device, reformattedPath, nullptr, &texture); //if the first index of the extension is '.' we know it's a 4 char long extension therefore not a dds and WIC is used
        return texture;
    }

    else if (extension[0] != '.')
    {
        std::string path;
        path = path + extension[2] + extension[3] + extension[4]; //set a new string called path which is our last 3 characters of the extension
        if (path == "DDS")
            DirectX::CreateDDSTextureFromFile(device, reformattedPath, nullptr, &texture); //if our texture extension is DDS, we create a DDS texture
        else if (path != "DDS")
            DirectX::CreateWICTextureFromFile(device, reformattedPath, nullptr, &texture); //otherwise we create a WIC texture
        return texture;
    }
    else
    {
        OutputDebugStringA("No extension found");
        return nullptr;
    }
}
