#include "pch.h"
#include "Particle.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

using Vertex = VertexPositionNormalTexture;


Particle::Particle()
{
}

Particle::Particle(const ParticleGeometry& geometry,
								const DirectX::SimpleMath::Vector3& position,
								const DirectX::SimpleMath::Vector3& rotation,
								const DirectX::SimpleMath::Vector3& scale,
								float lifetime) :
	m_geometry(geometry),
	m_position(position),
	m_rotation(rotation),
	m_scaling(scale),
	m_maxLifeTime(lifetime),
	m_lifetime(lifetime)
{
	m_worldMat *= Matrix::CreateScale(m_scaling);
	m_worldMat *= Matrix::CreateRotationX(m_rotation.x) * Matrix::CreateRotationY(m_rotation.y) * Matrix::CreateRotationZ(m_rotation.z);
	m_worldMat *= Matrix::CreateTranslation(m_position);
}

Particle::~Particle()
{
}

void Particle::setForce(const DirectX::SimpleMath::Vector3& force)
{
	m_force = force;
}

DirectX::SimpleMath::Vector3 Particle::getForce()
{
	return m_force;
}

bool Particle::isAlive()
{
	return m_lifetime > 0.0f;
}

float Particle::getMaxLifetime()
{
	return m_maxLifeTime;
}

float Particle::getLifetime()
{
	return m_lifetime;
}

void Particle::Update(float deltaTime)
{
	m_position += m_force * deltaTime;

	// Apply transformation
	m_worldMat = Matrix::Identity;
	m_worldMat *= Matrix::CreateScale(m_scaling);
	m_worldMat *= Matrix::CreateRotationX(m_rotation.x) * Matrix::CreateRotationY(m_rotation.y) * Matrix::CreateRotationZ(m_rotation.z);
	m_worldMat *= Matrix::CreateTranslation(m_position);

	m_lifetime -= deltaTime;
}

void Particle::Draw(ID3D11DeviceContext1* context,
					ConstantBuffer& cb,
					ID3D11Buffer* constantBuffer)
{
	UNREFERENCED_PARAMETER(cb);
	UNREFERENCED_PARAMETER(constantBuffer);

	auto stride = static_cast<UINT>(sizeof(Vertex));
	UINT offset = 0;

	context->PSSetShaderResources(0,
								  1,
								  particleDebugTexture.GetAddressOf());

	context->IASetVertexBuffers(0,
								1,
								particleVertexBuffer.GetAddressOf(),
								&stride,
								&offset);

	context->IASetIndexBuffer(particleIndexBuffer.Get(),
							  DXGI_FORMAT_R16_UINT,
							  offset);

	context->DrawIndexed(36, 0, 0);
}

void Particle::InitParticleData(ID3D11Device* device, const wchar_t* texturePath)
{
	InitBuffers(device);
	InitDebugTexture(texturePath, device);
}

// Init Buffers
void Particle::InitBuffers(ID3D11Device* device)
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
	sBd.pSysMem = m_geometry.particleVertices;

	DX::ThrowIfFailed(device->CreateBuffer(&bd,
										   &sBd,
											particleVertexBuffer.ReleaseAndGetAddressOf()));

	// Index Buffer
	D3D11_BUFFER_DESC iBd;
	ZeroMemory(&iBd, sizeof(iBd));
	iBd.Usage = D3D11_USAGE_DEFAULT;
	iBd.ByteWidth = sizeof(WORD) * 36;
	iBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sIbd;
	ZeroMemory(&sIbd, sizeof(sIbd));
	sIbd.pSysMem = m_geometry.particleIndices;

	DX::ThrowIfFailed(device->CreateBuffer(&iBd,
										   &sIbd,
											particleIndexBuffer.ReleaseAndGetAddressOf()));
}

void Particle::InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device)
{
	CreateDDSTextureFromFile(device,
							 texturePath,
							 nullptr,
							 particleDebugTexture.ReleaseAndGetAddressOf());
}