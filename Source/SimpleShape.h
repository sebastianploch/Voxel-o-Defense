#pragma once
#include "IGameObject.h"


class SimpleShape : public IGameObject
{
public:
	SimpleShape(const DirectX::SimpleMath::Matrix& viewMat,
				const DirectX::SimpleMath::Matrix& projMat,
				ID3D11DeviceContext* context);
	~SimpleShape();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* context) override;

private:
	DirectX::SimpleMath::Matrix					 m_viewMat;
	DirectX::SimpleMath::Matrix					 m_projMat;

	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
};

