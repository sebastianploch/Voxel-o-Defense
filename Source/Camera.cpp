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
    m_orgPosition(position),
    m_target(target),
    m_orgTarget(target),
    m_up(up),
    m_fov(fov),
    m_nearPlane(nearPlane),
    m_farPlane(farPlane),
    m_windowWidth(width),
    m_orgWindowWidth(width),
    m_windowHeight(height),
    m_orgWindowHeight(height),
    m_yaw(0.0f),
    m_orgYaw(m_yaw),
    m_pitch(0.0f),
    m_orgPitch(m_pitch),
    m_roll(0.0f),
    m_orgRoll(m_roll),
    m_movementSpeed(1.0f),
    m_rotationSpeed(1.0f)
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

void Camera::ResetCamera()
{
	m_position = m_orgPosition;
	m_target = m_orgTarget;
	m_up = Vector3::Up;
	m_yaw = m_orgYaw;
	m_pitch = m_orgPitch;
	m_roll = m_orgRoll;
}