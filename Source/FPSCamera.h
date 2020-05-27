#pragma once
#include "Camera.h"


class FPSCamera : public Camera
{
public:
	FPSCamera(float width,
			  float height,
			  float nearPlane,
			  float farPlane,
			  const float fov = DirectX::XM_PIDIV4,
			  const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::UnitZ,
			  const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero,
			  const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::Up);

	~FPSCamera();

	void Update(float deltaTime,
				const InputState& state) override;

	// Camera Rotation Speed
	inline float GetRotationSpeed() const { return m_rotationSpeed; }
	inline void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }

private:
	void ProcessMouseInput(float deltaTime,
						   const InputState& input);

	DirectX::SimpleMath::Vector3 ProcessKeyboardInput(float deltaTime,
													  const InputState& input);

	void ResetCamera();
	void WrapRotation();
	void UpdateTargetPosition();

private:
	float m_yaw;
	float m_pitch;
	float m_rotationSpeed;
	float m_cameraBoost;
};