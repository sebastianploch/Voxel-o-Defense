#pragma once

enum class SCENETYPE
{
	MENU = 0,
	PLAY,
	SPLASH
};

class IScene;

class SceneManager
{
public:
	SceneManager(int windowWidth,
				 int windowHeight,
				 Microsoft::WRL::ComPtr <ID3D11Device1> device,
				 UIManager& uiManager,
				 CameraManager& cameraManager,
				 ShaderManager& shaderManager,
				 DirectX::CommonStates& states);
	~SceneManager();

	void Update(float deltaTime, float time, InputState& inputState);
	void Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch);

	void Add(SCENETYPE sceneType);
	void Remove(SCENETYPE sceneType);
	bool Find(SCENETYPE sceneType);

	void SetWindowDimensions(int windowWidth, int windowHeight);

private:
	//std::vector<IScene*> m_scenes;
	std::vector<std::unique_ptr<IScene>> m_scenes;

	// Refs
	int						m_windowWidth;
	int						m_windowHeight;
	Microsoft::WRL::ComPtr <ID3D11Device1> m_d3dDevice;
	DirectX::CommonStates&	m_states;
	UIManager&			    m_UIManager;
	CameraManager&			m_cameraManager;
	ShaderManager&			m_shaderManager;
};