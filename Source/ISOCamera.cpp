#include "pch.h"
#include "ISOCamera.h"
#include "Sound.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

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
	m_scrollInLimit(20.0f),
	m_scrollOutLimit(300.0f),
	m_movementScaleBias(2.5f),
	m_minimumMovementSpeed(10.0f),
	m_maximumMovementSpeed(80.0f),
	m_rotationScaleBias(0.1f),
	m_minimumRotationSpeed(0.3f),
	m_maximumRotationSpeed(1.5f),
	m_lookOffset(Vector3(100.0f, 0.0f, 100.0f)),
	m_isBuildMode(false),
	m_targetYaw(0.0f)
{
	m_movementSpeed = 80.0f;
	m_rotationSpeed = 1.5f;
	m_yaw = -XM_PI;
	m_orgYaw = -2.5f;
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
					   float height)
{
	m_projection = Matrix::Identity;

	ResetMovement();
	ClampDimensions(width, height);

	// Replace stored window dimensions
	m_windowWidth = width;
	m_windowHeight = height;

	// Update Projection
	m_projection = Matrix::CreateOrthographic(m_windowWidth,
											  m_windowHeight,
											  m_nearPlane,
											  m_farPlane);
}

Vector3 ISOCamera::ProcessKeyboard(float deltaTime, const InputState& input)
{
	auto kbState = input.GetKeyboard().GetState();
	Vector3 move = Vector3::Zero;

	// Camera Movement
	if (kbState.W && m_position.z)
	{
		move.z -= m_movementSpeed * 2.0f * deltaTime;
	}
	if (kbState.S)
	{
		move.z += m_movementSpeed * 2.0f * deltaTime;
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
	if (input.GetKeyboardState().pressed.R && !m_isBuildMode)
	{
		ResetCamera();
		Resize(m_orgWindowWidth, m_orgWindowHeight);
	}

	// Camera Snapping (Build Mode Only)
	if (m_targetYaw != 0.0f) {
		float rotAmount = deltaTime * 5;
		if (m_targetYaw > 0) {
			if (m_targetYaw < rotAmount) {
				m_yaw += m_targetYaw;
				m_targetYaw = 0.0f;
			} else {
				m_yaw += rotAmount;
				m_targetYaw -= rotAmount;
			}
		} else {
			if (m_targetYaw > rotAmount) {
				m_yaw -= m_targetYaw;
				m_targetYaw = 0.0f;
			} else {
				m_yaw -= rotAmount;
				m_targetYaw += rotAmount;
			}
		}
		
		return move;
	}


	// Camera Rotation
	if (kbState.Q)
	{
		if (m_isBuildMode) {
			m_targetYaw += XM_PIDIV2;
			Sound::Fire(L"CameraSwipe");
		} else {
			m_yaw += m_rotationSpeed * deltaTime;
		}
	}

	if (kbState.E)
	{
		if (m_isBuildMode) {
			m_targetYaw -= XM_PIDIV2;
			Sound::Fire(L"CameraSwipe");
		} else {
			m_yaw -= m_rotationSpeed * deltaTime;
		}
	}

	WrapRotation();

	return move;
}

void ISOCamera::ResizeZoom(float width, float height)
{
	m_projection = Matrix::Identity;

	ClampDimensions(width, height);

	// Replace stored window dimensions
	m_windowWidth = width;
	m_windowHeight = height;

	// Update Projection
	m_projection = Matrix::CreateOrthographic(m_windowWidth,
											  m_windowHeight,
											  m_nearPlane,
											  m_farPlane);
}

void ISOCamera::ClampDimensions(float& width, float& height)
{
	// Calculate ratio of new Camera Dimensions and Clamp
	float ratio = GetRatio(width, height);
	width = std::clamp(width, m_scrollInLimit, m_scrollOutLimit);
	height = std::clamp(height, m_scrollInLimit / ratio, m_scrollOutLimit / ratio);
}

void ISOCamera::SetIsBuildMode(bool isEnabled) { 
	if (isEnabled)
		m_targetYaw = -m_yaw;
	m_isBuildMode = isEnabled; 
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
	// Resize window with new zoomed camera window dimensions
	ResizeZoom(m_windowWidth * m_zoom,
			   m_windowHeight * m_zoom);

	ScaleMovement();

	// Reset Zoom
	m_zoom = 1.0f;
}

void ISOCamera::ScaleMovement()
{
	// Zoom-in
	if (m_zoom < 1.0f)
	{
		m_movementSpeed -= m_zoom * m_movementScaleBias;
		m_rotationSpeed -= m_zoom * m_rotationScaleBias;
	}
	// Zoom-out
	else
	{
		m_movementSpeed += m_zoom * m_movementScaleBias;
		m_rotationSpeed += m_zoom * m_rotationScaleBias;
	}

	m_movementSpeed = std::clamp(m_movementSpeed, m_minimumMovementSpeed, m_maximumMovementSpeed);
	m_rotationSpeed = std::clamp(m_rotationSpeed, m_minimumRotationSpeed, m_maximumRotationSpeed);
}

void ISOCamera::ResetMovement()
{
	m_movementSpeed = m_maximumMovementSpeed;
	m_rotationSpeed = m_maximumRotationSpeed;
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