#include "pch.h"
#include "FPSCamera.h"

// Ignore 'unscoped enum' warning
#pragma warning(disable : 26812)

using namespace DirectX;

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

constexpr float s_pitchLimit = XM_PI / 2.0f - 0.01f;


FPSCamera::FPSCamera(float width,
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
           up),
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_rotationSpeed(0.4f),
	m_cameraBoost(2.0f)
{
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Update(float deltaTime, const InputState& state)
{
    ProcessMouseInput(deltaTime, state);

	// Create orientation from Pitch & Yaw
    Quaternion orientation = Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.0f);

	// Update position transformed by the orientation
    m_position += Vector3::Transform(ProcessKeyboardInput(deltaTime, state),
                                     orientation);

	// Update position to which camera is targeting (forward)
	UpdateTargetPosition();

	// Re-calculate View Matrix
    m_view = Matrix::CreateLookAt(m_position,
                                  m_target,
                                  m_up);
}

void FPSCamera::ProcessMouseInput(float deltaTime,
								  const InputState& input)
{
    auto state = input.GetMouse().GetState();

	if (state.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 changeInPos = (Vector3(float(state.x), float(state.y), 0.0f) * m_rotationSpeed) * deltaTime;

		m_pitch -= changeInPos.y;
		m_yaw -= changeInPos.x;
	}

    // Switch to relative mode when LBM is held
    input.GetMouse().SetMode(state.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

	WrapRotation();
}

Vector3 FPSCamera::ProcessKeyboardInput(float deltaTime,
                                        const InputState& input)
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

	// Camera Reset
	if (input.GetKeyboardState().pressed.R)
	{
		ResetCamera();
	}

	// Camera Boost
	if (input.GetKeyboardState().pressed.LeftShift)
	{
		m_movementSpeed *= m_cameraBoost;
	}
	if (input.GetKeyboardState().released.LeftShift)
	{
		m_movementSpeed /= m_cameraBoost;
	}

    return move;
}

void FPSCamera::ResetCamera()
{
	m_position = Vector3(0.0f, 0.0f, 3.0f);
	m_target = Vector3::Zero;
	m_up = Vector3::Up;
	m_yaw = 0.0f;
	m_pitch = 0.0f;
}

void FPSCamera::WrapRotation()
{
	// Limit pitch
	m_pitch = std::max(-s_pitchLimit, m_pitch);
	m_pitch = std::min(s_pitchLimit, m_pitch);

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

void FPSCamera::UpdateTargetPosition()
{
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	m_target = m_position + Vector3(-x, y, -z);
}