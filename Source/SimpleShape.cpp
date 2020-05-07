#include "pch.h"
#include "SimpleShape.h"

using namespace DirectX;

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

SimpleShape::SimpleShape(const DirectX::SimpleMath::Matrix& viewMat,
						 const DirectX::SimpleMath::Matrix& projMat,
						 ID3D11DeviceContext* context) :
	m_viewMat(viewMat),
	m_projMat(projMat)
{
	m_worldMat = Matrix::Identity;

	m_shape = GeometricPrimitive::CreateSphere(context);
}

SimpleShape::~SimpleShape()
{
	m_shape.reset();
}

void SimpleShape::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void SimpleShape::Draw(ID3D11DeviceContext* context)
{
	UNREFERENCED_PARAMETER(context);

	m_shape->Draw(m_worldMat, m_viewMat, m_projMat);
}
