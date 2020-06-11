#include "pch.h"
#include "PlayScene.h"

#include "Skybox.h"
#include "DebugLine.h"
#include "DebugSimpleCube.h"
#include "PlaneGameObject.h"
#include "ParticleEmitter.h"
#include "ChunkObject.h"
#include "ChunkHandler.h"
#include "VoxelModel.h"
#include "VoxelModelManager.h"
#include "VoxelRay.h"
#include "UIButton.h"
#include "UISprite.h"
#include "UIText.h"
#include "ModelSelectionObserver.h"
#include "NextWaveObserver.h"

#include "ISOCamera.h"

//- Required Header for threading dont delete - David -//
#include <future>

using namespace DirectX;
using namespace DirectX::SimpleMath;


PlayScene::PlayScene(SceneManager* sceneManager,
					 int windowWidth,
					 int windowHeight,
					 UIManager& uiManager,
					 Microsoft::WRL::ComPtr <ID3D11Device1> device,
					 CameraManager& cameraManager,
					 ShaderManager& shaderManager,
					 DirectX::CommonStates& states) :
	IScene(SCENETYPE::PLAY, sceneManager, windowWidth, windowHeight, uiManager, states),
	m_d3dDevice(device.Get()),
	m_cameraManager(cameraManager),
	m_shaderManager(shaderManager)
{
	// Initialise Build Manager
	m_buildManager = std::make_unique<BuildManager>();

	// Initialise Vertex & Index buffers (static) for debug cubes
	DebugSimpleCube::InitBuffers(m_d3dDevice);
	DebugSimpleCube::InitDebugTexture(L"Resources/Textures/DebugCubeTexture.dds", m_d3dDevice);

	// Initialise Water
	PlaneGameObject::InitMeshDataAndBuffers(DirectX::SimpleMath::Vector2Int(static_cast<int>((ChunkHandler::GetChunk(0, 0)->GetWidth() * ChunkHandler::GetMapSize() + 4) * 0.25f),
																			static_cast<int>((ChunkHandler::GetChunk(0, 0)->GetDepth() * ChunkHandler::GetMapSize() + 4) * 0.25f)),
											m_d3dDevice);
	PlaneGameObject::InitDebugTexture(L"Resources/Textures/water.dds", m_d3dDevice);

	// Create Debug Line
	m_gameObjects.push_back(std::make_shared<DebugLine>(Vector3(0.0f, 20.0f, 0.0f), Vector3(0.0f, 0.0f, -30.0f), m_d3dDevice));
	m_gameObjects.push_back(std::make_shared<DebugLine>(Vector3(0.0f, 20.0f, 0.0f), Vector3(0.0f, 0.0f, -30.0f), m_d3dDevice));
	m_gameObjects.push_back(std::make_shared<DebugLine>(Vector3(0.0f, 20.0f, 0.0f), Vector3(0.0f, 0.0f, -30.0f), m_d3dDevice));
	m_gameObjects.push_back(std::make_shared<DebugLine>(Vector3(0.0f, 20.0f, 0.0f), Vector3(0.0f, 0.0f, -30.0f), m_d3dDevice));

	// Create Skybox
	m_gameObjects.push_back(std::make_shared<Skybox>(m_d3dDevice, m_cameraManager, m_states));

	// Create Water
	m_gameObjects.push_back(std::make_shared<PlaneGameObject>(Vector3(0, 11.5f, 0), Vector3(), Vector3(4, 4, 4)));

	// AI
	m_enemyFactory = std::make_shared<EnemyFactory>(device);
	m_AiManager = std::make_shared<AiManager>(2800, DirectX::XMFLOAT3(50, 30, 20), m_enemyFactory.get());

	// Initialise turret pool
	for (int i = 0; i < 50; i++)
		m_turrets.push_back(std::make_shared<Turret>(Vector3::Zero));

	// Build Mode UI
	InitialiseBuildModeUI();

	// Create Brazier
	m_brazierObject.Initialise("Resources/Models/Mesh/brazier/brazier.obj", m_d3dDevice);
	int halfMap = (32 * 15) / 2;
	m_brazierObject.SetTranslation(Vector3(halfMap, 
										   WorldManipulation::GetHeightmap(Vector2Int(halfMap, halfMap)) + m_brazierObject.GetFeetPos(), 
										   halfMap));
	m_brazierObject.SetScale(Vector3::One * 2);

	InitialiseVoxelWorld();
}

PlayScene::~PlayScene()
{
}

void PlayScene::OnNotify(Event* event)
{
	UNREFERENCED_PARAMETER(event);

}

bool PlayScene::Update(const float deltaTime, float time, InputState& inputState)
{
	IUIObject* uiObject;

	// Handle Build Mode
	ISOCamera* cam = static_cast<ISOCamera*>(m_cameraManager.GetActiveCamera());
	// Temporary Build Mode toggling
	if (inputState.GetKeyboardState().pressed.H) {
		cam->SetIsBuildMode(!cam->GetIsBuildMode());
	}

	// Update build manager and build preview if build mode enabled
	if (cam->GetIsBuildMode()) {
		// Update build manager
		std::vector<SimpleMath::Vector3> verts = m_buildManager->Update(deltaTime,
			&inputState,
			&m_cameraManager,
			SimpleMath::Vector2Int(m_windowWidth, m_windowHeight), m_turrets);

		// Update preview with new vertex positions
		if (verts.size()) {	//Only if verts.size() != 0. verts size == 0 when mouse hasn't moved between frame
			static_cast<DebugLine*>(m_gameObjects[0].get())->UpdateLine(verts[0], verts[1]);
			static_cast<DebugLine*>(m_gameObjects[1].get())->UpdateLine(verts[1], verts[2]);
			static_cast<DebugLine*>(m_gameObjects[2].get())->UpdateLine(verts[2], verts[3]);
			static_cast<DebugLine*>(m_gameObjects[3].get())->UpdateLine(verts[3], verts[0]);
		}

		//Enable UI
		for (int id : m_buildModeIDs)
		{
			uiObject = m_uiManager.Get(id);
			if (uiObject)
				uiObject->SetVisible(true);
		}

	}
	else {
		// Set all preview vertex positions to (1, 1, 1) if not build mode
		static_cast<DebugLine*>(m_gameObjects[0].get())->UpdateLine(Vector3::One, Vector3::One);
		static_cast<DebugLine*>(m_gameObjects[1].get())->UpdateLine(Vector3::One, Vector3::One);
		static_cast<DebugLine*>(m_gameObjects[2].get())->UpdateLine(Vector3::One, Vector3::One);
		static_cast<DebugLine*>(m_gameObjects[3].get())->UpdateLine(Vector3::One, Vector3::One);

		//Enable UI
		for (int id : m_buildModeIDs)
		{
			uiObject = m_uiManager.Get(id);
			if (uiObject)
				uiObject->SetVisible(false);
		}
	}

	// Begin wave input
	if (inputState.GetKeyboardState().pressed.M && cam->GetIsBuildMode()) {
		const auto result = std::async(std::launch::async, &AiManager::StartWave, m_AiManager.get());
		Sound::Fire(L"WaveStart");
		cam->SetIsBuildMode(false);
	}

	//AiPathingThread.join();
	m_AiManager->Update(deltaTime, time);

	// Update chunks if they have been modified
	ChunkHandler::UpdateChunkMeshes(m_d3dDevice);

	// Update Turret Objects
	for (auto& turret : m_turrets)
		turret->Update(deltaTime, m_AiManager->GetAiAgents());

	// Update Brazier GameObject
	m_brazierObject.Update(deltaTime);

	// Update all objects
	for (auto object : m_gameObjects)
	{
		object->Update(deltaTime);
	}

	return false;
}

void PlayScene::Render(ID3D11DeviceContext1* context, ConstantBuffer& cb, ID3D11Buffer* constantBuffer, DirectX::SpriteBatch* spriteBatch)
{
	UNREFERENCED_PARAMETER(spriteBatch);

	context->UpdateSubresource(constantBuffer,
							   0,
							   nullptr,
							   &cb,
							   0, 0);

	// Render chunks
	ChunkHandler::DrawChunks(context, cb, constantBuffer, &m_shaderManager);

	// Render AI
	m_AiManager->Render(context, context, cb, constantBuffer, &m_shaderManager);

	// Render Brazier
	m_brazierObject.Draw(context, cb, constantBuffer);

	//Render all objects
	for (const auto& object : m_gameObjects)
	{
		// Assign Shader to be used to render upcoming object
		m_shaderManager.SetShader(object->GetShaderType(), context);

		// Assign Object World Mat data to ConstantBuffer
		cb.world = object->GetWorldMatrix();

		// Update Constant Buffer
		context->UpdateSubresource(constantBuffer,
								   0,
								   nullptr,
								   &cb,
								   0, 0);

		// Draw Object
		object->Draw(context, cb, constantBuffer);
	}
}

void PlayScene::InitialiseVoxelWorld() {
	// Initialise Voxel Chunk Objects
	ChunkObject::InitTexture(L"Resources/Textures/block_textures.dds", m_d3dDevice);

	WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel("Resources/Models/Voxel/wall_tier_1.vxml"), Vector3Int(10, 4, 10));
	WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel("Resources/Models/Voxel/wall_tier_2.vxml"), Vector3Int(21, 4, 10));
	WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel("Resources/Models/Voxel/wall_tier_3.vxml"), Vector3Int(32, 4, 10));
	WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel("Resources/Models/Voxel/wall_tier_4.vxml"), Vector3Int(43, 4, 10));
	WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel("Resources/Models/Voxel/wall_tier_4.vxml"), Vector3Int(53, 4, 10));

	// Create Initial Chunk Meshes
	ChunkHandler::UpdateChunkMeshes(m_d3dDevice);
}

void PlayScene::InitialiseBuildModeUI() {
	m_buildModeIDs = std::vector<int>();

	//Border, Text and Icon at top left, Gradient at bottom
	std::shared_ptr<UISprite> buildmodeBorderSprite = std::make_shared<UISprite>();
	buildmodeBorderSprite->Initialise(SimpleMath::Vector2(0.0f, 0.0f), L"Resources/Textures/UI/BuildMode/Border.dds", m_d3dDevice, -1.0f, m_windowWidth, m_windowHeight);
	m_buildModeIDs.push_back(m_uiManager.Add(buildmodeBorderSprite));

	//Create Model Selection Buttons
	std::shared_ptr<UIButton> wallTier1Button = std::make_shared<UIButton>();
	std::shared_ptr<UIButton> wallTier2Button = std::make_shared<UIButton>();
	std::shared_ptr<UIButton> wallTier3Button = std::make_shared<UIButton>();
	std::shared_ptr<UIButton> wallTier4Button = std::make_shared<UIButton>();
	std::shared_ptr<UIButton> turretButton = std::make_shared<UIButton>();

	//Initialise Position, Sprite, etc.
	wallTier1Button->Initialise(SimpleMath::Vector2(0.1 * m_windowWidth + (0.16 * m_windowWidth * 0), m_windowHeight * 0.9f), L"Resources/Textures/UI/BuildMode/wall_1_button.dds", L"Resources/Fonts/Calibri.spritefont", L"", m_d3dDevice);
	wallTier2Button->Initialise(SimpleMath::Vector2(0.1 * m_windowWidth + (0.16 * m_windowWidth * 1), m_windowHeight * 0.9f), L"Resources/Textures/UI/BuildMode/wall_2_button.dds", L"Resources/Fonts/Calibri.spritefont", L"", m_d3dDevice);
	wallTier3Button->Initialise(SimpleMath::Vector2(0.1 * m_windowWidth + (0.16 * m_windowWidth * 2), m_windowHeight * 0.9f), L"Resources/Textures/UI/BuildMode/wall_3_button.dds", L"Resources/Fonts/Calibri.spritefont", L"", m_d3dDevice);
	wallTier4Button->Initialise(SimpleMath::Vector2(0.1 * m_windowWidth + (0.16 * m_windowWidth * 3), m_windowHeight * 0.9f), L"Resources/Textures/UI/BuildMode/wall_4_button.dds", L"Resources/Fonts/Calibri.spritefont", L"", m_d3dDevice);
	turretButton->Initialise(SimpleMath::Vector2(0.1 * m_windowWidth + (0.16 * m_windowWidth * 4), m_windowHeight * 0.9f), L"Resources/Textures/UI/BuildMode/turret_button.dds", L"Resources/Fonts/Calibri.spritefont", L"", m_d3dDevice);

	//Add to UI Manager
	m_buildModeIDs.push_back(m_uiManager.Add(wallTier1Button));
	m_buildModeIDs.push_back(m_uiManager.Add(wallTier2Button));
	m_buildModeIDs.push_back(m_uiManager.Add(wallTier3Button));
	m_buildModeIDs.push_back(m_uiManager.Add(wallTier4Button));
	m_buildModeIDs.push_back(m_uiManager.Add(turretButton));

	//Add observer object and specify relative model
	wallTier1Button->Clicked()->AddObserver(new ModelSelectionObserver("Resources/Models/Voxel/wall_tier_1.vxml", m_buildManager.get()));
	wallTier2Button->Clicked()->AddObserver(new ModelSelectionObserver("Resources/Models/Voxel/wall_tier_2.vxml", m_buildManager.get()));
	wallTier3Button->Clicked()->AddObserver(new ModelSelectionObserver("Resources/Models/Voxel/wall_tier_3.vxml", m_buildManager.get()));
	wallTier4Button->Clicked()->AddObserver(new ModelSelectionObserver("Resources/Models/Voxel/wall_tier_4.vxml", m_buildManager.get()));
	turretButton->Clicked()->AddObserver(new ModelSelectionObserver("Resources/Models/Voxel/turret_tier_1.vxml", m_buildManager.get()));

	//Create "Next Wave" Button 
	std::shared_ptr<UIText> nextWaveText = std::make_shared<UIText>();
	nextWaveText->Initialise(Vector2(0.9 * m_windowWidth, 0.02 * m_windowWidth), 
							L"Press M to start wave", 
							SimpleMath::Color(204.0f / 255.0f, 54.0f / 255.0f, 54.0f / 255.0f, 1.0f), 
							L"Resources/Fonts/5x5.spritefont", 
							m_d3dDevice);
	m_buildModeIDs.push_back(m_uiManager.Add(nextWaveText));
}