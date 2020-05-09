#include "pch.h"
#include "DebugSimpleCube.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

using Vertex = VertexPositionNormalTexture;


DebugSimpleCube::DebugSimpleCube()
{
}

DebugSimpleCube::DebugSimpleCube(const DirectX::SimpleMath::Vector3& position,
								 const DirectX::SimpleMath::Vector3& rotation,
								 const DirectX::SimpleMath::Vector3& scale) :
	m_position(position),
	m_rotation(rotation),
	m_scaling(scale)
{
	m_worldMat *= Matrix::CreateScale(m_scaling);
	m_worldMat *= Matrix::CreateRotationX(m_rotation.x) * Matrix::CreateRotationY(m_rotation.y) * Matrix::CreateRotationZ(m_rotation.z);
	m_worldMat *= Matrix::CreateTranslation(m_position);
}

DebugSimpleCube::~DebugSimpleCube()
{
}

void DebugSimpleCube::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void DebugSimpleCube::Draw(ID3D11DeviceContext* context)
{
	auto stride = static_cast<UINT>(sizeof(Vertex));
	UINT offset = 0;

	context->PSSetShaderResources(0,
								  1,
								  s_debugTexture.GetAddressOf());

	context->IASetVertexBuffers(0,
								1,
								s_cubeVertexBuffer.GetAddressOf(),
								&stride,
								&offset);

	context->IASetIndexBuffer(s_cubeIndexBuffer.Get(),
							  DXGI_FORMAT_R16_UINT,
							  offset);

	context->DrawIndexed(36, 0, 0);
}

#pragma region STATICS
// Init Static Buffers
void DebugSimpleCube::InitBuffers(ID3D11Device* device)
{
	// Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sBd;
	ZeroMemory(&sBd, sizeof(sBd));
	sBd.pSysMem = DebugSimpleCube::s_cubeVertices;

	DX::ThrowIfFailed(device->CreateBuffer(&bd,
										   &sBd,
										   s_cubeVertexBuffer.ReleaseAndGetAddressOf()));

	// Index Buffer
	D3D11_BUFFER_DESC iBd;
	ZeroMemory(&iBd, sizeof(iBd));
	iBd.Usage = D3D11_USAGE_DEFAULT;
	iBd.ByteWidth = sizeof(WORD) * 36;
	iBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sIbd;
	ZeroMemory(&sIbd, sizeof(sIbd));
	sIbd.pSysMem = DebugSimpleCube::s_cubeIndices;

	DX::ThrowIfFailed(device->CreateBuffer(&iBd,
										   &sIbd,
										   s_cubeIndexBuffer.ReleaseAndGetAddressOf()));
}

void DebugSimpleCube::InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device)
{
	CreateDDSTextureFromFile(device,
							 texturePath,
							 nullptr,
							 s_debugTexture.ReleaseAndGetAddressOf());
}

// Init Static Variables
Microsoft::WRL::ComPtr<ID3D11Buffer> DebugSimpleCube::s_cubeVertexBuffer;

Microsoft::WRL::ComPtr<ID3D11Buffer> DebugSimpleCube::s_cubeIndexBuffer;

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DebugSimpleCube::s_debugTexture;

Vertex DebugSimpleCube::s_cubeVertices[24] = 
{
	Vertex(Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.0f)),
	Vertex(Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.0f)),
	Vertex(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
	Vertex(Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f)),

	Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f)),
	Vertex(Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, -1.0f, -1.0f), Vector2(0.0f, 0.0f)),
	Vertex(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f)),
	Vertex(Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, -1.0f, 1.0f), Vector2(1.0f, 1.0f)),

	Vertex(Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, -1.0f, 1.0f), Vector2(1.0f, 1.0f)),
	Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f)),
	Vertex(Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.0f)),
	Vertex(Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),

	Vertex(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f)),
	Vertex(Vector3(1.0f, -1.0f, -1.0f),Vector3(1.0f, -1.0f, -1.0f), Vector2(1.0f, 1.0f)),
	Vertex(Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.0f)),
	Vertex(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f)),

	Vertex(Vector3(-1.0f, -1.0f, -1.0f),Vector3(-1.0f, -1.0f, -1.0f), Vector2(1.0f, 1.0f)),
	Vertex(Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f)),
	Vertex(Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 1.0f, -1.0f), Vector2(0.0f, 0.0f)),
	Vertex(Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 1.0f, -1.0f), Vector2(1.0f, 0.0f)),

	Vertex(Vector3(-1.0f, -1.0f, 1.0f),Vector3(-1.0f, -1.0f, 1.0f), Vector2(0.0f, 1.0f)),
	Vertex(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, -1.0f, 1.0f), Vector2(1.0f, 1.0f)),
	Vertex(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f)),
	Vertex(Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f))
};

WORD DebugSimpleCube::s_cubeIndices[36] = 
{
	3, 1, 0,
	2, 1, 3,

	6, 4, 5,
	7, 4, 6,

	11, 9, 8,
	10, 9, 11,

	14, 12, 13,
	15, 12, 14,

	19, 17, 16,
	18, 17, 19,

	22, 20, 21,
	23, 20, 22
};
#pragma endregion STATICS