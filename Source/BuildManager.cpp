#include "pch.h"
#include "BuildManager.h"
#include "VoxelRay.h"
#include "WorldManipulation.h"
#include "VoxelModel.h"
#include "VoxelModelManager.h"

using namespace DirectX::SimpleMath;

std::vector<Vector3> BuildManager::Update(int deltaTime, InputState* input, CameraManager* cameraManager, Vector2Int winDimensions) {
	//Rotate model
	if (input->GetMouseState().rightButton == input->GetMouseState().PRESSED) {
		WorldManipulation::IncrementDir();
		prevMouseCoords = Vector2Int(0, 0);	//Forces build preview to update
	}

	//Place individual Voxel
	if (input->GetKeyboardState().pressed.Space) {
		Vector3Int rayHit = VoxelRay::VoxelRaycastFromMousePos(cameraManager->GetActiveCamera(),
																input,
																winDimensions.x,
																winDimensions.y);
		WorldManipulation::SetVoxel(rand() % 16 + 1, rayHit + Vector3Int::UnitY);
	}

	//Place model
	if (input->GetMouseState().leftButton == input->GetMouseState().PRESSED) {
		Vector3Int rayHit = VoxelRay::VoxelRaycastFromMousePos(cameraManager->GetActiveCamera(),
																input,
																winDimensions.x,
																winDimensions.y);
		WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel(currentModel), rayHit + Vector3Int::UnitY);
	}

	return GeneratePreviewVertices(input, cameraManager, winDimensions);
}

std::vector<Vector3> BuildManager::GeneratePreviewVertices(InputState* input, CameraManager* cameraManager, DirectX::SimpleMath::Vector2Int winDimensions) {
	if (input->GetMouse().GetState().x == prevMouseCoords.x &&
		input->GetMouse().GetState().y == prevMouseCoords.y)
		return std::vector<Vector3>();

	prevMouseCoords.x = input->GetMouse().GetState().x;
	prevMouseCoords.y = input->GetMouse().GetState().y;

	//Get current mouse hit position
	Vector3Int rayHit = VoxelRay::VoxelRaycastFromMousePos(cameraManager->GetActiveCamera(),
		input,
		winDimensions.x,
		winDimensions.y);

	//Get current model being placed
	VoxelModel* model = VoxelModelManager::GetOrLoadModel(currentModel);

	//Store values about the model
	int dir = WorldManipulation::GetDir();
	Vector3Int size = model->GetSize();
	Vector3Int position = rayHit + Vector3Int::UnitY;

	Vector3 pos = Vector3(position.x - model->GetOrigin().x + size.x / 2,
						  position.y + 0.1f,
						  position.z - model->GetOrigin().z + size.z / 2);

	//Create vertex positions based on rotation
	std::vector<Vector3> result;
	switch (dir) {
	case 1:
		result.push_back(pos + Vector3(0, 0, 0));
		result.push_back(pos + Vector3(0, 0, -size.x));
		result.push_back(pos + Vector3(size.z, 0, -size.x));
		result.push_back(pos + Vector3(size.z, 0, 0));
		break;
	case 2:
		result.push_back(pos + Vector3(0, 0, 0));
		result.push_back(pos + Vector3(0, 0, -size.z));
		result.push_back(pos + Vector3(-size.x, 0, -size.z));
		result.push_back(pos + Vector3(-size.x, 0, 0));
		break;
	case 3:
		result.push_back(pos + Vector3(0, 0, 0));
		result.push_back(pos + Vector3(0, 0, size.x));
		result.push_back(pos + Vector3(-size.z, 0, size.x));
		result.push_back(pos + Vector3(-size.z, 0, 0));
		break;
	default:
		result.push_back(pos + Vector3(0, 0, 0));
		result.push_back(pos + Vector3(0, 0, size.z));
		result.push_back(pos + Vector3(size.x, 0, size.z));
		result.push_back(pos + Vector3(size.x, 0, 0));
		break;
	}

	return result;
}
