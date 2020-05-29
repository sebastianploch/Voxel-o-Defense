#include "pch.h"
#include "Game.h"

#include "DebugSimpleCube.h"

#include "ChunkObject.h"
#include "ChunkHandler.h"
#include "VoxelModel.h"
#include "VoxelModelManager.h"
#include "VoxelRay.h"

// Ignore 'unscoped enum' warning
#pragma warning(disable : 26812)

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


Game::Game() noexcept :
    m_window(nullptr),
    m_windowWidth(800),
    m_windowHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_11_1)
{
}

#pragma region Initialise
// Initialise the DirectX
void Game::Initialize(HWND window,
					  int width,
					  int height)
{
    m_window = window;
    m_windowWidth = std::max(width, 1);
    m_windowHeight = std::max(height, 1);

    CreateDevice();
    CreateResources();

    // Set locked framerate (60fps)
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60.0);

	// Initialise Input Handler
	m_inputState = std::make_unique<InputState>(m_window);

	// Initialise Vertex & Index buffers (static) for debug cubes
	DebugSimpleCube::InitBuffers(m_d3dDevice.Get());
	DebugSimpleCube::InitDebugTexture(L"Resources/Textures/DebugCubeTexture.dds", m_d3dDevice.Get());

	// Create one debug cube
	m_gameObjects.push_back(std::make_shared<DebugSimpleCube>("Resources/config/cube.json", "cube"));

	// Initialise Voxel Chunk Objects
	ChunkObject::InitTexture(L"Resources/Textures/block_textures.dds", m_d3dDevice.Get());

	// Create Initial Chunk Meshes
	ChunkHandler::UpdateChunkMeshes(m_d3dDevice.Get());
}

// Create direct3d context and allocate resources that don't depend on window size change.
void Game::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	// Create the DX11 API device object, and get a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	DX::ThrowIfFailed(D3D11CreateDevice(nullptr,
										D3D_DRIVER_TYPE_HARDWARE,
										nullptr,
										creationFlags,
										featureLevels,
										_countof(featureLevels),
										D3D11_SDK_VERSION,
										device.ReleaseAndGetAddressOf(),
										&m_featureLevel,
										context.ReleaseAndGetAddressOf()));

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	DX::ThrowIfFailed(device.As(&m_d3dDevice));
	DX::ThrowIfFailed(context.As(&m_d3dContext));

	// Create Common States Instance
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// Create Shader Manager Instance
	m_shaderManager = std::make_unique<ShaderManager>(m_d3dDevice.Get());

	CreateConstantBuffer();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	const UINT backBufferWidth = static_cast<UINT>(m_windowWidth);
	const UINT backBufferHeight = static_cast<UINT>(m_windowHeight);
	const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	constexpr UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount,
												backBufferWidth,
												backBufferHeight,
												backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// OnDeviceLost will reenter this method and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = backBufferCount;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(),
															  m_window,
															  &swapChainDesc,
															  &fsSwapChainDesc,
															  nullptr,
															  m_swapChain.ReleaseAndGetAddressOf()));

		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D render target.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the render target to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(),
														  nullptr,
														  m_renderTargetView.ReleaseAndGetAddressOf()));

	// Create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat,
										   backBufferWidth,
										   backBufferHeight,
										   1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc,
												   nullptr,
												   depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(),
														  &depthStencilViewDesc,
														  m_depthStencilView.ReleaseAndGetAddressOf()));

	// Set Primitive Topology (Triangles)
	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Initialise camera
	m_camera = std::make_unique<FPSCamera>((float)backBufferWidth,
										(float)backBufferHeight,
										0.1f,
										300.0f,
										Vector3(0.0f, 0.0f, 4.0f));
}

// Create constant buffer to be used as a resource by shader.
void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	DX::ThrowIfFailed(m_d3dDevice->CreateBuffer(&cbd,
												nullptr,
												m_constantBuffer.ReleaseAndGetAddressOf()));
}

// Reset and re-initialise component upon "Device Lost" flag
void Game::OnDeviceLost()
{
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_d3dContext.Reset();
	m_d3dDevice.Reset();

	m_camera.reset();
	m_inputState.reset();

	m_states.reset();
	m_constantBuffer.Reset();
	m_shaderManager.reset();

	CreateDevice();
	CreateResources();
}
#pragma endregion Initialise

#pragma region GameLoop
// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

void Game::Update(DX::StepTimer const& timer)
{
    float deltaTime = static_cast<float>(timer.GetElapsedSeconds());
	
	m_camera->Update(deltaTime,
					 *m_inputState);

	// Update Input Handler
	m_inputState->Update();

	// Exit game on 'Escape' key press
	if (m_inputState->GetKeyboardState().pressed.Escape)
	{
		ExitGame();
	}

	// Example code for casting ray from camera
	if (m_inputState->GetKeyboardState().pressed.Space) {
		//Place random voxel at ray hit point
		Vector3 diff = (m_camera.get()->GetTarget() * 2) - (m_camera.get()->GetPosition() * 2);	//Get normalised direction
		diff *= 50;	//Multiply by scalar length
		diff += m_camera.get()->GetPosition() * 2;	//Reapply the camera position
		DirectX::SimpleMath::Vector3Int rayHit = VoxelRay::VoxelRaycast(m_camera.get()->GetPosition() * 2, diff);
		WorldManipulation::SetVoxel(rand() % 16 + 1, rayHit + DirectX::SimpleMath::Vector3Int::UnitY);
	}
	if (m_inputState->GetKeyboardState().pressed.Enter) {
		//Place Structure at ray hit point
		Vector3 diff = (m_camera.get()->GetTarget() * 2) - (m_camera.get()->GetPosition() * 2);	//Get normalised direction
		diff *= 50;	//Multiply by scalar length
		diff += m_camera.get()->GetPosition() * 2;	//Reapply the camera position
		DirectX::SimpleMath::Vector3Int rayHit = VoxelRay::VoxelRaycast(m_camera.get()->GetPosition() * 2, diff);
		WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel("Resources/Models/Voxel/castle_structure.vxml"), rayHit + DirectX::SimpleMath::Vector3Int::UnitY);
	}

	// Update chunks if they have been modified
	ChunkHandler::UpdateChunkMeshes(m_d3dDevice.Get());

	// Update all objects
	for (auto object : m_gameObjects)
	{
		object->Update(deltaTime);
	}
}

void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
	Prepare();

	// Create ConstantBuffer and assign camera mat's
	ConstantBuffer cb;
	cb.projection = m_camera->GetProjection();
	cb.view = m_camera->GetView();

	// Render chunks
	ChunkHandler::DrawChunks(m_d3dContext.Get(), m_shaderManager.get());

	// Render all objects
	for (const auto& object : m_gameObjects)
	{
		// Assign Shader to be used to render upcoming object
		m_shaderManager->SetShader(object->GetShaderType(), m_d3dContext.Get());

		// Assign Object World Mat data to ConstantBuffer
		cb.world = object->GetWorldMatrix();

		// Update Constant Buffer
		m_d3dContext->UpdateSubresource(m_constantBuffer.Get(),
										0,
										nullptr,
										&cb,
										0, 0);

		// Draw Object
		object->Draw(m_d3dContext.Get());
	}

	// Swap backbuffer
    Present();
}
#pragma endregion GameLoop

#pragma region Helpers
// Assigns default window size to the provided var's
void Game::GetDefaultSize(int& width,
						  int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// Compile shader (hlsl) using d3dcompiler
HRESULT Game::CompileShader(LPCTSTR path,
							LPCSTR entryPoint,
							LPCSTR profile,
							ID3DBlob** blob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif // _DEBUG

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(path,
									NULL,
									D3D_COMPILE_STANDARD_FILE_INCLUDE,
									entryPoint,
									profile,
									flags,
									0,
									&shaderBlob,
									&errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}

		if (shaderBlob)
		{
			shaderBlob->Release();
		}

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

// Helper method to clear the back buffers and set device states.
void Game::Clear()
{
	// Clear the views.
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::Salmon);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set Target
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Set Depth buffer
	m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight));
	m_d3dContext->RSSetViewports(1, &viewport);

	// Rasterizer State (Clockwise)
	m_d3dContext->RSSetState(m_states->CullClockwise());
}

// Helper method to prepare the scene (set shaders/constant buffer)
void Game::Prepare()
{
	// Set Texture Sampler
	ID3D11SamplerState* sampler = nullptr;
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = 0;
	m_d3dDevice->CreateSamplerState(&sampDesc, &sampler);
	
	m_d3dContext->PSSetSamplers(0, 1, &sampler);

	// Set Constant Buffer for VS and PS Shaders
	m_d3dContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_d3dContext->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}
#pragma endregion Helpers

#pragma region WindowCalls
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
	m_inputState->Reset();
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
	m_inputState->Reset();
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
	m_inputState->Reset();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
	m_inputState->Reset();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_windowWidth = std::max(width, 1);
    m_windowHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}
#pragma endregion WindowCalls