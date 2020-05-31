#include "pch.h"

using namespace DirectX;

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;


Camera::Camera(float width,
               float height, 
               float nearPlane,
               float farPlane,
               const CAMERA_TYPE& type,
               const float fov,
               const Vector3& position,
               const Vector3& target,
               const Vector3& up) :
    m_position(position),
    m_target(target),
    m_up(up),
    m_movementSpeed(5.0f),
    m_type(type)
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
		m_projection = Matrix::CreateOrthographic(width * 0.01f,
                                                  height * 0.01f,
												  nearPlane,
												  farPlane);
    }
}

Camera::~Camera()
{
}

void Camera::Resize(float width,
                    float height,
                    float nearPlane,
                    float farPlane,
                    const float fov)
{
    m_projection = Matrix::Identity;

    if (m_type == CAMERA_TYPE::PERSPECTIVE)
    {
        m_projection = Matrix::CreatePerspectiveFieldOfView(fov,
                                                            width / height,
                                                            nearPlane,
                                                            farPlane);
    }

	else if (m_type == CAMERA_TYPE::ORTHOGRAPHIC)
	{
		m_projection = Matrix::CreateOrthographic(width * 0.01f,
												  height * 0.01f,
												  nearPlane,
												  farPlane);
	}
}