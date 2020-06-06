#pragma once
#include "CameraManager.h"
#include "IGameObject.h"
#include <vector>

class BuildManager {
public:
	BuildManager() = default;

	std::vector<DirectX::SimpleMath::Vector3> Update(int deltaTime, InputState* input, CameraManager* cameraManager, DirectX::SimpleMath::Vector2Int winDimensions);

private:
	std::vector<DirectX::SimpleMath::Vector3> GeneratePreviewVertices(InputState* input, CameraManager* cameraManager, DirectX::SimpleMath::Vector2Int winDimensions);
};