#include "pch.h"
#include "CameraManager.h"

#include "ISOCamera.h"
#include "FPSCamera.h"

using namespace DirectX;

using DirectX::SimpleMath::Vector3;


CameraManager::CameraManager(float width,
							 float height) :
	m_isoActive(true)
{
	InitialiseFPSCamera(width, height);
	InitialiseISOCamera(width, height);
}

CameraManager::~CameraManager()
{
	m_iso.reset();
	m_fps.reset();
}

void CameraManager::Update(float deltaTime, const InputState& state)
{
	// Switch Cameras
	if (state.GetKeyboardState().pressed.F1) // Isometric camera
	{
		m_isoActive = true;
	}
	if (state.GetKeyboardState().pressed.F2) // FPS camera
	{
		m_isoActive = false;
	}

	// Update active camera
	if (m_isoActive)
		m_iso->Update(deltaTime, state);

	else
		m_fps->Update(deltaTime, state);
}

void CameraManager::Resize(float width,
						   float height)
{
	m_iso->Resize(width, height);
	m_fps->Resize(width, height);
}

void CameraManager::InitialiseFPSCamera(float width,
										float height)
{
	m_fps = std::make_unique<FPSCamera>(width,
										height,
										0.01f,
										500.0f,
										XM_PIDIV4,
										Vector3(0.0f, 15.0f, 4.0f));
}

void CameraManager::InitialiseISOCamera(float width,
										float height)
{
	m_iso = std::make_unique<ISOCamera>(width,
										height,
										-500.0f,
										500.0f,
										Vector3(240.0f, 20.0f, 240.0f));
}