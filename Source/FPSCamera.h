#pragma once


class FPSCamera : public Camera
{
public:
	FPSCamera(float width,
			  float height,
			  float nearPlane,
			  float farPlane,
			  const float fov = DirectX::XM_PIDIV4,
			  const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
			  const DirectX::SimpleMath::Vector3& target = -DirectX::SimpleMath::Vector3::UnitZ,
			  const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::Up);

	~FPSCamera();

	void Update(float deltaTime,
				const InputState& state) override;

	void Resize(float width,
				float height,
				float nearPlane,
				float farPlane,
				const float fov = DirectX::XM_PIDIV4) override;

private:
	void ProcessMouseInput(float deltaTime,
						   const InputState& input);

	DirectX::SimpleMath::Vector3 ProcessKeyboardInput(float deltaTime,
													  const InputState& input);

	void ResetCamera();
	void WrapRotation();
	void UpdateTargetPosition();

private:
	float m_cameraBoost;
};