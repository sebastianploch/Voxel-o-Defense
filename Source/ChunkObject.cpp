#include "pch.h"
#include "ChunkObject.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

ChunkObject::ChunkObject() :
	m_position(Vector3(0, 0, 0)),
	m_rotation(Vector3(0, 0, 0)),
	m_scaling(Vector3(1, 1, 1)) {

	m_worldMat *= Matrix::CreateScale(m_scaling);
	m_worldMat *= Matrix::CreateRotationX(m_rotation.x) * Matrix::CreateRotationY(m_rotation.y) * Matrix::CreateRotationZ(m_rotation.z);
	m_worldMat *= Matrix::CreateTranslation(m_position);

	m_shaderType = Shaders::VOXEL_SHADER;
}

ChunkObject::ChunkObject(const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& rotation,
	const DirectX::SimpleMath::Vector3& scale) :
	m_position(position),
	m_rotation(rotation),
	m_scaling(scale) {

	m_worldMat *= Matrix::CreateScale(m_scaling);
	m_worldMat *= Matrix::CreateRotationX(m_rotation.x) * Matrix::CreateRotationY(m_rotation.y) * Matrix::CreateRotationZ(m_rotation.z);
	m_worldMat *= Matrix::CreateTranslation(m_position);
}

ChunkObject::~ChunkObject() {

}

void ChunkObject::Update(float deltaTime) {
	UNREFERENCED_PARAMETER(deltaTime);
}

void ChunkObject::Draw(ID3D11DeviceContext* context) {
	context->PSSetShaderResources(0,
		1,
		s_texture.GetAddressOf());

	context->IASetVertexBuffers(0,
		1,
		m_mesh.m_VertexBuffer.GetAddressOf(),
		&m_mesh.m_VBStride,
		&m_mesh.m_VBOffset);

	context->IASetIndexBuffer(m_mesh.m_IndexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,
		m_mesh.m_VBOffset);

	context->DrawIndexed(m_mesh.m_IndexCount, 0, 0);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ChunkObject::s_texture;
void ChunkObject::InitTexture(const wchar_t* texturePath, ID3D11Device* device) {
	CreateDDSTextureFromFile(device,
							texturePath,
							nullptr,
							s_texture.ReleaseAndGetAddressOf());
}

void ChunkObject::UpdateMesh(VoxelMesh newMesh) {
	if (m_mesh.m_IndexBuffer) m_mesh.m_IndexBuffer->Release();
	if (m_mesh.m_VertexBuffer) m_mesh.m_VertexBuffer->Release();

	m_mesh = newMesh;
}