#pragma once


class ISOCamera : public Camera
{
public:
	ISOCamera(float width,
			  float height,
			  float nearPlane,
			  float farPlane,
			  const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
			  const DirectX::SimpleMath::Vector3& target = -DirectX::SimpleMath::Vector3::UnitZ,
			  const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY);

	~ISOCamera();

	void Update(float deltaTime,
				const InputState& input) override;
	
	void Resize(float width,
				float height) override;

#pragma region Getters/Setters
	inline float GetZoom() const { return m_zoom; }
	inline void SetZoom(float zoom) { m_zoom = zoom; }

	inline float GetScrollSpeed() const { return m_scrollSpeed; }
	inline void SetScrollSpeed(float speed) { m_scrollSpeed = speed; }

	inline float GetScrollInLimit() const { return m_scrollInLimit; }
	inline void SetScrollInLimit(float scrollIn) { m_scrollInLimit = scrollIn; }

	inline float GetScrollOutLimit() const { return m_scrollOutLimit; }
	inline void SetScrollOutLimit(float scrollOut) { m_scrollOutLimit = scrollOut; }

	inline float GetMovementScaleBias() const { return m_movementScaleBias; }
	inline void SetMovementScaleBias(float bias) { m_movementScaleBias = bias; }

	inline float GetMaximumMovementSpeed() const { return m_maximumMovementSpeed; }
	inline void SetMaximumMovementSpeed(float maxSpeed) { m_maximumMovementSpeed = maxSpeed; }

	inline float GetMinimumMovementSpeed() const { return m_minimumMovementSpeed; }
	inline void SetMinimumMovementSpeed(float minSpeed) { m_minimumMovementSpeed = minSpeed; }

	inline float GetRotationScaleBias() const { return m_rotationScaleBias; }
	inline void SetRotationScaleBias(float bias) { m_rotationScaleBias = bias; }

	inline float GetMinimumRotationSpeed() const { return m_minimumRotationSpeed; }
	inline void SetMinimumRotationSpeed(float minSpeed) { m_minimumRotationSpeed = minSpeed; }

	inline float GetMaximumRotationSpeed() const { return m_maximumRotationSpeed; }
	inline void SetMaximumRotationSpeed(float maxSpeed) { m_maximumRotationSpeed = maxSpeed; }

	inline DirectX::SimpleMath::Vector3 GetLookOffset() const { return m_lookOffset; }
	inline void SetLookOffset(const DirectX::SimpleMath::Vector3& offset) { m_lookOffset = offset; }

	inline bool GetIsBuildMode() const { return m_isBuildMode; }
	void SetIsBuildMode(bool isEnabled);
#pragma endregion Getters/Setters

private:
	void ProcessMouse(float deltaTime,
					  const InputState& input) override;

	DirectX::SimpleMath::Vector3 ProcessKeyboard(float deltaTime,
												 const InputState& input) override;

	// Version of Resize that doesn't reset movement
	void ResizeZoom(float width,
					float height);

	// Clamp Camera Window Dimensions with scroll limit and apply ratio
	void ClampDimensions(float& width,
						 float& height);

	void Zoom();
	void ScaleMovement();
	void ResetMovement();

	void WrapRotation();
	void UpdateTargetPosition();

private:
	float						 m_zoom;
	float						 m_scrollSpeed;
	float						 m_scrollInLimit;
	float						 m_scrollOutLimit;
	float						 m_movementScaleBias;
	float						 m_maximumMovementSpeed;
	float						 m_minimumMovementSpeed;
	float						 m_rotationScaleBias;
	float						 m_minimumRotationSpeed;
	float						 m_maximumRotationSpeed;
	DirectX::SimpleMath::Vector3 m_lookOffset;
	bool						 m_isBuildMode;
	float						 m_targetYaw;
};