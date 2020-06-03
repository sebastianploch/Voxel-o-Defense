#pragma once

#include "StepTimer.h"
#include "IGameObject.h"
#include "ShaderManager.h"
#include "CameraManager.h"


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
    void Render();

    void CreateDevice();
    void CreateResources();
    void CreateConstantBuffer();

    void OnDeviceLost();

    void InitialiseVoxelWorld();

	void Clear();
	void Present();
	void Prepare();
	HRESULT CompileShader(LPCTSTR path,
						  LPCSTR entryPoint,
						  LPCSTR profile,
						  ID3DBlob** blob);

private:
    // Device resources.
    HWND                                            m_window;
    int                                             m_windowWidth;
    int                                             m_windowHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Input Handler
    std::unique_ptr<InputState>                     m_inputState;

    // Game Objects
    std::vector<std::shared_ptr<IGameObject>>       m_gameObjects;

    // Rendering
    std::unique_ptr<ShaderManager>                  m_shaderManager;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_constantBuffer;
    std::unique_ptr<DirectX::CommonStates>          m_states;

    // Basic Camera
    std::unique_ptr<CameraManager>                  m_cameraManager;

    // DeltaTime Timer
    DX::StepTimer                                   m_timer;
};
