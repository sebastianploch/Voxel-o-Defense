#include "pch.h"
#include "ISOCamera.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

static constexpr float s_16by9ratio = 1.77f;
static constexpr float s_scrollInLimit = 10.0f;
static constexpr float s_scrollOutLimit = 150.0f;
static constexpr float s_movementScaleBias = 2.5f;
static constexpr float s_minimumMovementSpeed = 10.0f;
static constexpr float s_maximumMovementSpeed = 80.0f;


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
		   CAMERA_TYPE::ORTHOGRAPHIC,
		   XM_PIDIV4,
           position,
           target,
           up),
	m_zoom(1.0f),
	m_scrollSpeed(5.0f),
	m_lookOffset(Vector3(100.0f, 0.0f, 100.0f))
{
}

ISOCamera::~ISOCamera()
{
}

void ISOCamera::Update(float deltaTime, const InputState& input)
{
	ProcessMouse(deltaTime, input);

	Quaternion orientation = Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);

	m_position += Vector3::Transform(ProcessKeyboard(deltaTime, input),
									 orientation);

	UpdateTargetPosition();

	m_view = Matrix::CreateLookAt(m_position,
								  m_target,
								  m_up);
}

void ISOCamera::Resize(float width,
					   float height,
					   float nearPlane,
					   float farPlane,
					   const float fov)
{
	UNREFERENCED_PARAMETER(fov);
	m_projection = Matrix::Identity;

	// Clamp zooming in/out
	width = std::clamp(width, s_scrollInLimit, s_scrollOutLimit);
	height = std::clamp(height, s_scrollInLimit / s_16by9ratio, s_scrollOutLimit / s_16by9ratio);

	m_windowWidth = width;
	m_windowHeight = height;

	m_projection = Matrix::CreateOrthographic(m_windowWidth,
											  m_windowHeight,
											  nearPlane,
											  farPlane);
}

Vector3 ISOCamera::ProcessKeyboard(float deltaTime, const InputState& input)
{
	auto kbState = input.GetKeyboard().GetState();
	Vector3 move = Vector3::Zero;

	// Camera Movement
	if (kbState.W)
	{
		move.z -= m_movementSpeed * deltaTime;
	}
	if (kbState.S)
	{
		move.z += m_movementSpeed * deltaTime;
	}
	if (kbState.A)
	{
		move.x -= m_movementSpeed * deltaTime;
	}
	if (kbState.D)
	{
		move.x += m_movementSpeed * deltaTime;
	}

	// Camera Rotation
	if (kbState.Q)
	{
		m_yaw += m_rotationSpeed * deltaTime;
	}

	if (kbState.E)
	{
		m_yaw -= m_rotationSpeed * deltaTime;
	}

	WrapRotation();

	return move;
}

void ISOCamera::ProcessMouse(float deltaTime, const InputState& input)
{
	auto state = input.GetMouse().GetState();

	// Scroll up - get closer
	if (state.scrollWheelValue > 0.0f)
	{
		m_zoom -= m_scrollSpeed * deltaTime;

		Zoom();
	}

	// Scroll down - get further
	if (state.scrollWheelValue < 0.0f)
	{
		m_zoom += m_scrollSpeed * deltaTime;

		Zoom();
	}

	// Reset scroll value
	if (state.scrollWheelValue != 0.0f)
	{
		input.GetMouse().ResetScrollWheelValue();
	}
}

void ISOCamera::Zoom()
{
	// Resize window by the zoom offset
	Resize(m_windowWidth * m_zoom,
		   m_windowHeight * m_zoom,
		   m_nearPlane,
		   m_farPlane);

	ScaleMovement();

	// Reset Zoom
	m_zoom = 1.0f;
}

void ISOCamera::ScaleMovement()
{
	if (m_movementSpeed >= s_minimumMovementSpeed && m_movementSpeed <= s_maximumMovementSpeed)
	{
		// Zoom-in
		if (m_zoom < 1.0f)
		{
			m_movementSpeed -= m_zoom * s_movementScaleBias;
		}
		// Zoom-out
		else
		{
			m_movementSpeed += m_zoom * s_movementScaleBias;
		}

		m_movementSpeed = std::clamp(m_movementSpeed, s_minimumMovementSpeed, s_maximumMovementSpeed);

		DEBUG_PRINT(m_movementSpeed);
	}
}

void ISOCamera::WrapRotation()
{
	// Wrap yaw
	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}
}

void ISOCamera::UpdateTargetPosition()
{
	float z = cosf(m_yaw);
	float x = sinf(m_yaw);

	m_target = Vector3(m_position.x, 0.0f, m_position.z) + Vector3(-x * m_lookOffset.x, 0.0f, -z * m_lookOffset.z);
}