#include "pch.h"
#include "PlaneGameObject.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

using Vertex = VertexPositionNormalTexture;


PlaneGameObject::PlaneGameObject()
{
}


PlaneGameObject::PlaneGameObject(const DirectX::SimpleMath::Vector3& position,
								 const DirectX::SimpleMath::Vector3& rotation,
								 const DirectX::SimpleMath::Vector3& scale) :
	m_position(position),
	m_rotation(rotation),
	m_scaling(scale)
{
	m_worldMat *= Matrix::CreateScale(m_scaling);
	m_worldMat *= Matrix::CreateRotationX(m_rotation.x) * Matrix::CreateRotationY(m_rotation.y) * Matrix::CreateRotationZ(m_rotation.z);
	m_worldMat *= Matrix::CreateTranslation(m_position);

	m_shaderType = Shaders::WATER_SHADER;
}

PlaneGameObject::~PlaneGameObject()
{
	
}

void PlaneGameObject::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void PlaneGameObject::Draw(ID3D11DeviceContext* context)
{
	auto stride = static_cast<UINT>(sizeof(Vertex));
	UINT offset = 0;

	context->PSSetShaderResources(0,
								  1,
								  s_texture.GetAddressOf());

	context->IASetVertexBuffers(0,
								1,
								s_planeVertexBuffer.GetAddressOf(),
								&stride,
								&offset);

	context->IASetIndexBuffer(s_planeIndexBuffer.Get(),
							  DXGI_FORMAT_R16_UINT,
							  offset);

	context->DrawIndexed(s_indexCount, 0, 0);
}

#pragma region STATICS
// Init Static Buffers
void PlaneGameObject::InitMeshDataAndBuffers(DirectX::SimpleMath::Vector2Int planeSize, ID3D11Device* device)
{
	//Create Vertices and Indices before buffers
	std::vector<Vertex> vertices = GeneratePlaneVertices(planeSize.x, planeSize.y);
	std::vector<unsigned short> indices = GeneratePlaneIndices(planeSize.x, planeSize.y);
	PlaneGameObject::s_planeVertices = new Vertex[vertices.size()];
	for (int v = 0; v < vertices.size(); v++) {
		s_planeVertices[v] = vertices[v];
	}
	PlaneGameObject::s_planeIndices = new unsigned short[indices.size()];
	for (int i = 0; i < indices.size(); i++) {
		s_planeIndices[i] = indices[i];
	}

	// Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sBd;
	ZeroMemory(&sBd, sizeof(sBd));
	sBd.pSysMem = PlaneGameObject::s_planeVertices;

	DX::ThrowIfFailed(device->CreateBuffer(&bd,
										   &sBd,
										   s_planeVertexBuffer.ReleaseAndGetAddressOf()));

	// Index Buffer
	D3D11_BUFFER_DESC iBd;
	ZeroMemory(&iBd, sizeof(iBd));
	iBd.Usage = D3D11_USAGE_DEFAULT;
	iBd.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	iBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sIbd;
	ZeroMemory(&sIbd, sizeof(sIbd));
	sIbd.pSysMem = PlaneGameObject::s_planeIndices;

	DX::ThrowIfFailed(device->CreateBuffer(&iBd,
										   &sIbd,
										   s_planeIndexBuffer.ReleaseAndGetAddressOf()));
	
	s_indexCount = (int)indices.size();

	delete[] s_planeVertices;
	delete[] s_planeIndices;
}

void PlaneGameObject::InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device)
{
	CreateDDSTextureFromFile(device,
							 texturePath,
							 nullptr,
							 s_texture.ReleaseAndGetAddressOf());
}

//The following two functions are from Anthony Sturdy FGGC Sem 1 Submission, it is being repurposed for this project.
std::vector<Vertex> PlaneGameObject::GeneratePlaneVertices(int width, int depth) {
	std::vector<Vertex> returnVec;

	for (int y = 0; y < depth; y++) {
		for (int x = 0; x < width; x++) {
			Vertex vert = { XMFLOAT3((float)x, 0.f, (float)y), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT2((float)x, (float)y) };
			returnVec.push_back(vert);
		}
	}

	return returnVec;
}

std::vector<unsigned short> PlaneGameObject::GeneratePlaneIndices(int width, int depth) {
	std::vector<unsigned short> returnVec;

	for (int y = 0; y < depth - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			//Triangle 1
			returnVec.push_back(static_cast<unsigned short>((y) * width + (x)));
			returnVec.push_back(static_cast<unsigned short>((y + 1) * width + (x)));
			returnVec.push_back(static_cast<unsigned short>((y)*width + (x + 1)));

			//Triangle 2
			returnVec.push_back(static_cast<unsigned short>((y) * width + (x + 1)));
			returnVec.push_back(static_cast<unsigned short>((y + 1) * width + (x)));
			returnVec.push_back(static_cast<unsigned short>((y + 1) * width + (x + 1)));
		}
	}

	return returnVec;
}

// Init Static Variables
Microsoft::WRL::ComPtr<ID3D11Buffer> PlaneGameObject::s_planeVertexBuffer;

Microsoft::WRL::ComPtr<ID3D11Buffer> PlaneGameObject::s_planeIndexBuffer;

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> PlaneGameObject::s_texture;

int PlaneGameObject::s_indexCount;
DirectX::VertexPositionNormalTexture* PlaneGameObject::s_planeVertices;
unsigned short* PlaneGameObject::s_planeIndices;

#pragma endregion STATICS