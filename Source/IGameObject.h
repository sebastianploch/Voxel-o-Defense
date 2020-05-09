#pragma once


class IGameObject
{
public:
	virtual ~IGameObject() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(ID3D11DeviceContext* context) = 0;

	inline DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMat; }
	inline void SetWorldMatrix(const DirectX::SimpleMath::Matrix& worldMat) { m_worldMat = worldMat; }

protected:
	IGameObject() = default;

protected:
	DirectX::SimpleMath::Matrix m_worldMat{ DirectX::SimpleMath::Matrix::Identity };
};