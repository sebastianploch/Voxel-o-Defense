#include "pch.h"
#include "GreedyVoxelMeshGeneration.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void GreedyVoxelMeshGeneration::GenerateMesh(Chunk* chunk, ID3D11Device* device, VoxelMesh* mesh) {
    std::vector<Vector3> m_vertices;
    std::vector<int> m_indices;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_uvs;
    std::vector<Vector2> m_uvs1;

    int dims[] = { chunk->GetWidth(), chunk->GetHeight(), chunk->GetDepth() };    //Chunk dimensions

    //Sweep over 3-axes
    for (int dir = 0; dir < 3; dir++) {
        int i = 0;
        int j = 0;
        int k = 0;
        int l = 0;
        int width = 0;
        int height = 0;

        int u = (dir + 1) % 3;
        int v = (dir + 2) % 3;

        int x[] = { 0, 0, 0 };
        int q[] = { 0, 0, 0 };
        int* mask = new int[(dims[u] + 1.0) * (dims[v] + 1.0)]();

        q[dir] = 1;

        for (x[dir] = -1; x[dir] < dims[dir];) {
            // Compute the mask
            int n = 0;
            for (x[v] = 0; x[v] < dims[v]; ++x[v]) {
                for (x[u] = 0; x[u] < dims[u]; ++x[u], ++n) {
                    int vox1 = chunk->GetVoxel(x[0], x[1], x[2]);
                    int vox2 = chunk->GetVoxel(x[0] + q[0], x[1] + q[1], x[2] + q[2]);

                    int a = (0 <= x[dir] ? vox1 : 0);
                    int b = (x[dir] < dims[dir] - 1 ? vox2 : 0);

                    if ((a != 0) == (b != 0)) {
                        mask[n] = 0;
                    } else if ((a != 0)) {
                        mask[n] = a;
                    } else {
                        mask[n] = -b;
                    }
                }
            }

            // Increment x[d]
            ++x[dir];

            // Generate mesh for mask using lexicographic ordering
            n = 0;
            for (j = 0; j < dims[v]; ++j) {
                for (i = 0; i < dims[u];) {
                    auto voxType = mask[n];

                    if (voxType != 0) {
                        // compute width
                        for (width = 1; mask[n + width] == voxType && (i + width) < dims[u]; ++width) {}

                        // compute height
                        bool done = false;
                        for (height = 1; (j + height) < dims[v]; ++height) {
                            for (k = 0; k < width; ++k) {
                                if (mask[n + k + height * dims[u]] != voxType) {
                                    done = true;
                                    break;
                                }
                            }
                            if (done) {
                                break;
                            }
                        }

                        //Generate mesh segment
                        x[u] = i;
                        x[v] = j;

                        int du[] = { 0, 0, 0 };
                        int dv[] = { 0, 0, 0 };

                        if (voxType > 0) {
                            dv[v] = height;
                            du[u] = width;
                        } else {
                            du[v] = height;
                            dv[u] = width;
                        }

                        Vector3 chunkPos = Vector3(chunk->GetXIndex() * chunk->GetWidth(), 0, chunk->GetZIndex() * chunk->GetDepth());
                        Vector3 v1 = Vector3(x[0] + chunkPos.x, 
                                             x[1] + chunkPos.y, 
                                             x[2] + chunkPos.z);

                        Vector3 v2 = Vector3(x[0] + du[0] + chunkPos.x, 
                                             x[1] + du[1] + chunkPos.y, 
                                             x[2] + du[2] + chunkPos.z);

                        Vector3 v3 = Vector3(x[0] + du[0] + dv[0] + chunkPos.x, 
                                             x[1] + du[1] + dv[1] + chunkPos.y, 
                                             x[2] + du[2] + dv[2] + chunkPos.z);

                        Vector3 v4 = Vector3(x[0] + dv[0] + chunkPos.x,
                                             x[1] + dv[1] + chunkPos.y, 
                                             x[2] + dv[2] + chunkPos.z);

                        GenerateQuad(v1, v2, v3, v4, GetNormal(voxType < 0, dir),
                            &m_vertices, &m_indices, &m_normals, &m_uvs, &m_uvs1,
                            width, height, voxType);

                        for (l = 0; l < height; ++l) {
                            for (k = 0; k < width; ++k) {
                                mask[n + k + l * dims[u]] = 0;
                            }
                        }
                        // increment counters
                        i += width;
                        n += width;
                    } else {
                        ++i;
                        ++n;
                    }
                }
            }
        }

        delete[] mask;
    }

    CreateBuffers(device, mesh, &m_vertices, &m_indices, &m_normals, &m_uvs, &m_uvs1);
}

void GreedyVoxelMeshGeneration::CreateBuffers(ID3D11Device* device, VoxelMesh* mesh,
                                              std::vector<Vector3>* vertices, std::vector<int>* indices, std::vector<Vector3>* normals, std::vector<Vector2>* uvs, std::vector<Vector2>* uvs1) {
   
    //Create vertex and index array
    auto verticesArray = std::make_unique<VertexPositionNormalDualTexture[]>(vertices->size());
    for (unsigned int i = 0; i < vertices->size(); i++) {
        verticesArray[i].position = vertices->at(i);
        verticesArray[i].normal = normals->at(i);
        verticesArray[i].textureCoordinate0 = uvs->at(i);
        verticesArray[i].textureCoordinate1 = uvs1->at(i);
    }

    auto indicesArray = std::make_unique<unsigned short[]>(indices->size());
    for (unsigned int i = 0; i < indices->size(); i++) {
        indicesArray[i] = indices->at(i);
    }

    //Create Vertex Buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VertexPositionNormalDualTexture) * vertices->size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = verticesArray.get();

    device->CreateBuffer(&bd, &InitData, &vertexBuffer);


    //Create index buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC bd1;
    ZeroMemory(&bd1, sizeof(bd1));
    bd1.Usage = D3D11_USAGE_DEFAULT;
    bd1.ByteWidth = sizeof(WORD) * indices->size();
    bd1.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd1.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData1;
    ZeroMemory(&InitData1, sizeof(InitData1));
    InitData1.pSysMem = indicesArray.get();
    device->CreateBuffer(&bd1, &InitData1, &indexBuffer);

    //Assign buffers to mesh object
    mesh->m_VertexBuffer = vertexBuffer;
    mesh->m_VBOffset = 0;
    mesh->m_VBStride = sizeof(VertexPositionNormalDualTexture);
    mesh->m_IndexCount = indices->size();
    mesh->m_IndexBuffer = indexBuffer;
}

void GreedyVoxelMeshGeneration::GenerateQuad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, Vector3 normal,
                                            std::vector<Vector3>* vertices, std::vector<int>* indices, std::vector<Vector3>* normals, std::vector<Vector2>* uvs, std::vector<Vector2>* uvs1, 
                                            int w, int h, int vox) {

    int vc = vertices->size();
    char abs_vox = abs(vox);

    //Add vertices
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);

    //Add indices
    indices->push_back(vc + 0);
    indices->push_back(vc + 1);
    indices->push_back(vc + 2);
    indices->push_back(vc + 2);
    indices->push_back(vc + 3);
    indices->push_back(vc + 0);

    //Calculate uv variables
    const int TEX_SIZE = 512;
    const int TILE_SIZE = 32;
    const int TILES_PER_AXIS = TEX_SIZE / TILE_SIZE;
    const float UV_TILE_INCR = (float)TILE_SIZE / TEX_SIZE;

    int tileY = floorf(abs_vox / (float)TILES_PER_AXIS);            //Tile offset X
    int tileX = abs_vox - (tileY * TILES_PER_AXIS);                 //Tile offset Y
    Vector2 offset = Vector2(tileX * UV_TILE_INCR, tileY * UV_TILE_INCR); //Converted to UV space

    //Add width and height UV's
    int nW = vox < 0 ? w : h;   //Backfaces have width and height swapped, this reverses that
    int nH = vox < 0 ? h : w;
    std::vector<Vector2> nUv = GenerateCorrectUVs(normal, nW, nH);
    uvs->insert(uvs->end(), nUv.begin(), nUv.end());

    //Add tile position
    uvs1->push_back(offset);
    uvs1->push_back(offset);
    uvs1->push_back(offset);
    uvs1->push_back(offset);

    //Add normals
    normals->push_back(normal);
    normals->push_back(normal);
    normals->push_back(normal);
    normals->push_back(normal);
}

std::vector<Vector2> GreedyVoxelMeshGeneration::GenerateCorrectUVs(Vector3 normal, int w, int h) {
    //Was having problems with UV's being rotated 90 degs for some face directions.
    //I have no idea what the corrolation is to the mesh generation, so UV's are manually rotated 
    //based on direction here (Up, South and East faces were the problem).
    std::vector<Vector2> resultList;

    if (normal.y == 1 ||
        normal.z == -1 ||
        normal.x == 1) {
        resultList.push_back(Vector2(0, h));
        resultList.push_back(Vector2(0, 0));
        resultList.push_back(Vector2(w, 0));
        resultList.push_back(Vector2(w, h));
        
    } else {
        resultList.push_back(Vector2(h, w));
        resultList.push_back(Vector2(0, w));
        resultList.push_back(Vector2(0, 0));
        resultList.push_back(Vector2(h, 0));
        
    }

    return resultList;
}

Vector3 GreedyVoxelMeshGeneration::GetNormal(bool isBackFace, int axis) {
    int backface = (isBackFace ? -1 : 1);
    switch (axis) {
    case 0:
        return Vector3(1 * backface, 0, 0);

    case 1:
        return Vector3(0, 1 * backface, 0);

    case 2:
        return Vector3(0, 0, 1 * backface);

    default:
        return Vector3(0, 0, 0);
    }
}