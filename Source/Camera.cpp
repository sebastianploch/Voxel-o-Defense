#include "pch.h"

using namespace DirectX;

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;


Camera::Camera(float width,
               float height, 
               float nearPlane,
               float farPlane,
               const CAMERA_TYPE& type,
               const float fov,
               const Vector3& position,
               const Vector3& target,
               const Vector3& up) :
    m_type(type),
    m_position(position),
    m_target(target),
    m_up(up),
    m_nearPlane(nearPlane),
    m_farPlane(farPlane),
    m_windowWidth(width),
    m_windowHeight(height),
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_roll(0.0f),
    m_movementSpeed(80.0f),
    m_rotationSpeed(0.5f)
{
    m_view = Matrix::CreateLookAt(m_position,
                                  m_target,
                                  m_up);

    if (m_type == CAMERA_TYPE::PERSPECTIVE)
    {
		m_projection = Matrix::CreatePerspectiveFieldOfView(fov,
															width / height,
															nearPlane,
															farPlane);
    }

    else if (m_type == CAMERA_TYPE::ORTHOGRAPHIC)
    {
		m_projection = Matrix::CreateOrthographic(width,
                                                  height,
												  nearPlane,
												  farPlane);
    }
}

Camera::~Camera()
{
}