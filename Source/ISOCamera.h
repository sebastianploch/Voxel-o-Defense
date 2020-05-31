#pragma once


class ISOCamera : public Camera
{
public:
	ISOCamera(float width,
			  float height,
			  float nearPlane,
			  float farPlane,
			  const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::UnitZ,
			  const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero,
			  const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY);

	~ISOCamera();

	void Update(float deltaTime,
				const InputState& input) override;

private:
	DirectX::SimpleMath::Vector3 ProcessKeyboard(float deltaTime,
												 const InputState& input);

	void ProcessMouse(float deltaTime,
					  const InputState& input);

	void WrapRotation();
	void UpdateTargetPosition();

private:
	float m_scrollSpeed;
};