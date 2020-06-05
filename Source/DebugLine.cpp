#include "pch.h"
#include "DebugLine.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;


DebugLine::DebugLine(const Vector3& start,
					 const Vector3& end,
					 ID3D11Device* device) :
	m_device(device)
{
	m_shaderType = Shaders::LINE_SHADER;
	UpdateLine(start, end);
}

DebugLine::~DebugLine()
{

}

void DebugLine::Update(float deltaTime)
{

}

void DebugLine::Draw(ID3D11DeviceContext* context)
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	auto stride = static_cast<UINT>(sizeof(VertexPositionColor));
	UINT offset = 0;

	context->IASetVertexBuffers(0,
								1,
								m_vertexBuffer.GetAddressOf(),
								&stride,
								&offset);

	context->Draw(2, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DebugLine::UpdateLine(const Vector3& start,
						   const Vector3& end)
{
	m_vertices[0] = VertexPositionColor(start, Colors::Magenta);
	m_vertices[1] = VertexPositionColor(end, Colors::Magenta);

	CreateVertexBuffer();
}

void DebugLine::CreateVertexBuffer()
{
	// Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexPositionColor) * 2;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sBd;
	ZeroMemory(&sBd, sizeof(sBd));
	sBd.pSysMem = m_vertices;

	DX::ThrowIfFailed(m_device->CreateBuffer(&bd,
										    &sBd,
										    m_vertexBuffer.ReleaseAndGetAddressOf()));
}