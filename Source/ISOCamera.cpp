#include "pch.h"
#include "ISOCamera.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;


ISOCamera::ISOCamera(float width,
                     float height,
                     float nearPlane,
                     float farPlane,
                     const Vector3& position,
                     const Vector3& target,
                     const Vector3& up) :
    Camera(width,
           height,
           nearPlane,
           farPlane,
           position,
           target,
           up)
{
}

ISOCamera::~ISOCamera()
{

}

void ISOCamera::Update(float deltaTime, const InputState& state)
{

}