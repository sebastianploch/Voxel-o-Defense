#pragma once

#include "StepTimer.h"
#include "IGameObject.h"

using VertexType = DirectX::VertexPositionColor;


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

    void Clear();
    void Present();
    HRESULT CompileShader(LPCTSTR path,
                          LPCSTR entryPoint,
                          LPCSTR profile,
                          ID3DBlob** blob);

    void CreateDevice();
    void CreateResources();
    void CreateShaders();

    void OnDeviceLost();

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

    // Game Objects
    std::vector<std::unique_ptr<IGameObject>>       m_gameObjects;

    // Rendering
    std::unique_ptr<DirectX::CommonStates>          m_states;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_constantBuffer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_basicPixelShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_basicVertexShader;

    // Basic Camera
	DirectX::SimpleMath::Matrix                     m_viewMat;
	DirectX::SimpleMath::Matrix                     m_projMat;

    // DeltaTime Timer
    DX::StepTimer                                   m_timer;
};
