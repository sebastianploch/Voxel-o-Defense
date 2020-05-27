#pragma once

#include "Camera.h"


class ISOCamera : public Camera
{
public:
	ISOCamera(float width,
			  float height,
			  float nearPlane,
			  float farPlane,
			  const float fov = DirectX::XM_PIDIV4,
			  const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::UnitZ,
			  const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero,
			  const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY);

	~ISOCamera();

	void Update(float deltaTime,
				const InputState& state) override;

private:

};