#pragma once
#include "IGameObject.h"


class DebugLine : public IGameObject
{
public:
	DebugLine(const DirectX::SimpleMath::Vector3& start,
			  const DirectX::SimpleMath::Vector3& end,
			  ID3D11Device* device);
	~DebugLine();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext1* context,
			  ConstantBuffer& cb,
			  ID3D11Buffer* constantBuffer) override;

	void UpdateLine(const DirectX::SimpleMath::Vector3& start,
					const DirectX::SimpleMath::Vector3& end);

private:
	void CreateVertexBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;

	DirectX::VertexPositionColor m_vertices[2];
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};