#pragma once

class FPSCamera;
class ISOCamera;


class CameraManager
{
public:
	CameraManager(float width, float height);
	~CameraManager();

	void Update(float deltaTime,
				const InputState& state);

	void Resize(float width,
				float height);

	inline Camera* GetActiveCamera() { return m_isoActive ? (Camera*)m_iso.get() : (Camera*)m_fps.get(); }

private:
	void InitialiseFPSCamera(float width, float height);
	void InitialiseISOCamera(float width, float height);

private:
	bool m_isoActive;

	std::unique_ptr<FPSCamera> m_fps;
	std::unique_ptr<ISOCamera> m_iso;
};