#pragma once
#include "CameraManager.h"
#include "IGameObject.h"
#include "Turret.h"
#include <vector>
#include <string>

class BuildManager {
public:
	BuildManager() = default;

	std::vector<DirectX::SimpleMath::Vector3> Update(int deltaTime, InputState* input, CameraManager* cameraManager, DirectX::SimpleMath::Vector2Int winDimensions, std::vector<std::shared_ptr<Turret>>& turrets);

	inline std::string GetCurrentModel() const { return currentModel; }
	inline void SetCurrentModel(std::string dir) { currentModel = dir; }

private:
	std::string currentModel = "Resources/Models/Voxel/wall_tier_1.vxml";
	DirectX::SimpleMath::Vector2Int prevMouseCoords = DirectX::SimpleMath::Vector2Int(0, 0);

private:
	std::vector<DirectX::SimpleMath::Vector3> GeneratePreviewVertices(InputState* input, CameraManager* cameraManager, DirectX::SimpleMath::Vector2Int winDimensions);
};