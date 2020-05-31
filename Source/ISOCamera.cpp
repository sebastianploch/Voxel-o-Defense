#include "pch.h"
#include "ISOCamera.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

static constexpr float s_scrollInLimit = 2.0f;
static constexpr float s_scrollOutLimit = 10.0f;


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
	m_yaw(0.0f),
	m_scrollSpeed(20.0f),
	m_rotationSpeed(2.0f)
{
}

ISOCamera::~ISOCamera()
{
}

void ISOCamera::Update(float deltaTime, const InputState& input)
{
	ProcessMouse(deltaTime, input);

	Quaternion orientation = Quaternion::CreateFromYawPitchRoll(m_yaw, 0.0f, 0.0f);

	m_position += Vector3::Transform(ProcessKeyboard(deltaTime, input),
									 orientation);

	UpdateTargetPosition();

	m_view = Matrix::CreateLookAt(m_position,
								  m_target,
								  m_up);
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
		m_position.y -= m_scrollSpeed * deltaTime;
		m_position.y = std::max(s_scrollInLimit, m_position.y);
	}

	// Scroll down - get further
	if (state.scrollWheelValue < 0.0f)
	{
		m_position.y += m_scrollSpeed * deltaTime;
		m_position.y = std::min(s_scrollOutLimit, m_position.y);
	}

	// Reset scroll value
	if (state.scrollWheelValue != 0.0f)
	{
		input.GetMouse().ResetScrollWheelValue();
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

	m_target = Vector3(m_position.x, 0.0f, m_position.z) + Vector3(-x, 0.0f, -z);
}