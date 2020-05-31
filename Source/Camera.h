#pragma once


enum class CAMERA_TYPE
{
	PERSPECTIVE = 0,
	ORTHOGRAPHIC
};

class Camera
{
public:
	virtual ~Camera();

	virtual void Update(float deltaTime,
						const InputState& input) = 0;

	void Resize(float width,
				float height,
				float nearPlane,
				float farPlane,
				const float fov = DirectX::XM_PIDIV4);

	// Camera Position (Eye)
	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	// Camera Target (looking at)
	inline DirectX::SimpleMath::Vector3 GetTarget() const { return m_target; }
	inline void SetTarget(const DirectX::SimpleMath::Vector3& targetPosition) { m_target = targetPosition; }

	// Camera Up Vector
	inline DirectX::SimpleMath::Vector3 GetUp() const { return m_up; }
	inline void SetUp(const DirectX::SimpleMath::Vector3& upPosition) { m_up = upPosition; }

	// Camera Moving Speed
	inline float GetMovementSpeed() const { return m_movementSpeed; }
	inline void SetMovementSpeed(float speed) { m_movementSpeed = speed; }

	// Get View Matrix
	inline DirectX::SimpleMath::Matrix GetView() const { return m_view; }

	// Get Projection Matrix
	inline DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }

protected:
	Camera(float width,
		   float height,
		   float nearPlane,
		   float farPlane,
		   const CAMERA_TYPE& type,
		   float fov = DirectX::XM_PIDIV4,
		   const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
		   const DirectX::SimpleMath::Vector3& target = -DirectX::SimpleMath::Vector3::UnitZ,
		   const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::Up);

protected:
	// Camera Eye
	DirectX::SimpleMath::Vector3 m_position;

	// Camera At
	DirectX::SimpleMath::Vector3 m_target;

	// Camera Up
	DirectX::SimpleMath::Vector3 m_up;

	// Camera Moving Speed
	float						 m_movementSpeed;
	CAMERA_TYPE                  m_type;

	DirectX::SimpleMath::Matrix	 m_projection;
	DirectX::SimpleMath::Matrix	 m_view;
};