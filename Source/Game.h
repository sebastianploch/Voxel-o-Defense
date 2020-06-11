#pragma once

#include "StepTimer.h"
#include "IGameObject.h"
#include "ShaderManager.h"
#include "Model.h"
#include "UIManager.h"
#include "BuildManager.h"

#include "AiManager.h"
#include "Enemy.h"
#include "Turret.h"

class EnemyFactory;

class Game
{
public:
    Game() noexcept;
    ~Game() = default;

    // Initialization and management
    void Initialize(HWND window,
                    int width,
                    int height);

    // Game Loop
    void Tick();

    // Window Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width,
                             int height);
    // Properties
    void GetDefaultSize(int& width,
                        int& height) const noexcept;

private:
    void Update(DX::StepTimer const& timer);
    void UpdateAudio();

    void Render();

    void CreateDevice();
    void CreateResources();
    void CreateAudioEngine();
    void CreateConstantBuffer();

    void OnDeviceLost();

    void InitialiseVoxelWorld();
    void InitialiseBuildModeUI();

	void Clear();
	void Present();
	void Prepare();
	HRESULT CompileShader(LPCTSTR path,
						  LPCSTR entryPoint,
						  LPCSTR profile,
						  ID3DBlob** blob);

private:
    // Device resources.
    HWND												m_window;
    int													m_windowWidth;
    int													m_windowHeight;

    D3D_FEATURE_LEVEL									m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>				m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>		m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>				m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;

    // Input Handler
    std::unique_ptr<InputState>							m_inputState;

    // Game Objects
    std::vector<std::shared_ptr<IGameObject>>			m_gameObjects;

	// Ai Manager

	std::shared_ptr<AiManager>						    m_AiManager;

    // Rendering
    std::unique_ptr<ShaderManager>                      m_shaderManager;
    std::unique_ptr<DirectX::CommonStates>              m_states;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_constantBuffer;

    // Basic Camera
    std::unique_ptr<CameraManager>                      m_cameraManager;

    // Audio
    std::unique_ptr<DirectX::AudioEngine>               m_audioEngine;

	// UI
	std::unique_ptr<UIManager>						    m_UIManager;
	std::unique_ptr<DirectX::SpriteBatch>			    m_spriteBatch;

    // Building
    std::unique_ptr<BuildManager>                       m_buildManager;

    // DeltaTime Timer
    DX::StepTimer                                       m_timer;

    // Store Build Mode UI ID's
    std::vector<int>                                    m_buildModeIDs;

	std::shared_ptr<EnemyFactory>                       m_enemyFactory;

    // Turret Object
    std::vector<std::shared_ptr<Turret>>                m_turrets = std::vector<std::shared_ptr<Turret>>();
};
