#pragma once


class Camera
{
public:
	Camera(float width,
		   float height,
		   float nearPlane,
		   float farPlane,
		   const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::UnitZ,
		   const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero,
		   const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY);

	~Camera();

	void Update(float deltaTime, const InputState& state);

	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_eye; }
	inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_eye = position; }

	inline DirectX::SimpleMath::Vector3 GetTarget() const { return m_target; }
	inline void SetTarget(const DirectX::SimpleMath::Vector3& targetPosition) { m_target = targetPosition; }

	inline DirectX::SimpleMath::Vector3 GetUp() const { return m_up; }
	inline void SetUp(const DirectX::SimpleMath::Vector3& upPosition) { m_up = upPosition; }

	inline DirectX::SimpleMath::Matrix GetView() const { return m_view; }

	inline DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }

private:
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_up;

	DirectX::SimpleMath::Matrix	m_projection;
	DirectX::SimpleMath::Matrix	m_view;
};