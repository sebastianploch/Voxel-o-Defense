#include "pch.h"
#include "ISOCamera.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;


ISOCamera::ISOCamera(float width,
                     float height,
                     float nearPlane,
                     float farPlane,
                     const float fov,
                     const Vector3& position,
                     const Vector3& target,
                     const Vector3& up) :
    Camera(width,
           height,
           nearPlane,
           farPlane,
           fov,
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
    UNREFERENCED_PARAMETER(deltaTime);
    UNREFERENCED_PARAMETER(state);
}