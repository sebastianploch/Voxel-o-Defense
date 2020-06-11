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

	// Resize Camera to match new window size
	virtual void Resize(float width,
						float height) = 0;

#pragma region Getters/Setters
	#pragma region Camera_Properties
	// Camera Position (Eye)
	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	// Camera Target (looking at)
	inline DirectX::SimpleMath::Vector3 GetTarget() const { return m_target; }
	inline void SetTarget(const DirectX::SimpleMath::Vector3& targetPosition) { m_target = targetPosition; }

	// Camera Up Vector
	inline DirectX::SimpleMath::Vector3 GetUp() const { return m_up; }
	inline void SetUp(const DirectX::SimpleMath::Vector3& upPosition) { m_up = upPosition; }
	#pragma endregion Camera_Properties

	#pragma region Movement
	inline float GetMovementSpeed() const { return m_movementSpeed; }
	inline void SetMovementSpeed(float speed) { m_movementSpeed = speed; }

	inline float GetRotationSpeed() const { return m_rotationSpeed; }
	inline void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
	#pragma endregion Movement

	#pragma region Rotation
	inline float GetYaw() const { return m_yaw; }
	inline void SetYaw(float yaw) { m_yaw = yaw; }

	inline float GetPitch() const { return m_pitch; }
	inline void SetPitch(float pitch) { m_pitch = pitch; }

	inline float GetRoll() const { return m_roll; }
	inline void SetRoll(float roll) { m_roll = roll; }
	#pragma endregion Rotation

	#pragma region Window_Dimensions
	/* Get Current Camera Window Dimensions
	   @.x returns window width
	   @.y returns window height
	*/
	inline DirectX::SimpleMath::Vector2 GetDimensions() const { return DirectX::SimpleMath::Vector2(m_windowWidth, m_windowHeight); }

	// Get Current Camera Window Width
	inline float GetWidth() const { return m_windowWidth; }

	// Get Current Camera Window Height
	inline float GetHeight() const { return m_windowHeight; }
	#pragma endregion Window_Dimensions

	#pragma region Camera_Frustum
	inline float GetFOV() const { return m_fov; }
	inline void SetFOV(float fov) { m_fov = fov; }

	inline float GetNearPlane() const { return m_nearPlane; }
	inline void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

	inline float GetFarPlane() const { return m_farPlane; }
	inline void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
	#pragma endregion Camera_Frustum

	// Get Camera Type
	inline CAMERA_TYPE GetType() const { return m_type; }

	// Get View Matrix
	inline DirectX::SimpleMath::Matrix GetView() const { return m_view; }

	// Get Projection Matrix
	inline DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
#pragma endregion Getters/Setters

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

	virtual void ProcessMouse(float deltaTime,
							  const InputState& input) = 0;

	virtual DirectX::SimpleMath::Vector3 ProcessKeyboard(float deltaTime,
														 const InputState& input) = 0;

	void ResetCamera();

	// Helper function to find screen ratio
	inline static float GetRatio(float width, float height) { return width / height; }

protected:
	// Camera Eye
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_orgPosition;

	// Camera At
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_orgTarget;

	// Camera Up
	DirectX::SimpleMath::Vector3 m_up;

	// Camera FrustumS
	float						 m_fov;
	float						 m_nearPlane;
	float						 m_farPlane;

	// Window Dimensions
	float						 m_windowWidth;
	float						 m_orgWindowWidth;
	float						 m_windowHeight;
	float						 m_orgWindowHeight;

	// Rotation
	float						 m_yaw;
	float						 m_orgYaw;
	float						 m_pitch;
	float						 m_orgPitch;
	float						 m_roll;
	float						 m_orgRoll;

	// Movement
	float						 m_movementSpeed;
	float						 m_rotationSpeed;

	CAMERA_TYPE                  m_type;

	DirectX::SimpleMath::Matrix	 m_projection;
	DirectX::SimpleMath::Matrix	 m_view;
};