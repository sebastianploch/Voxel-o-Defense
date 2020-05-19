#include "pch.h"
#include "Camera.h"

using namespace DirectX;

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;


Camera::Camera(float width,
               float height, 
               float nearPlane,
               float farPlane,
               const Vector3& position,
               const Vector3& target,
               const Vector3& up) :
    m_position(position),
    m_target(target),
    m_up(up),
    m_movementSpeed(2.0f)
{
    m_view = Matrix::CreateLookAt(m_position,
                                  m_target,
                                  m_up);

    m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV4,
                                                        width / height,
                                                        nearPlane,
                                                        farPlane);
}

Camera::~Camera()
{
}