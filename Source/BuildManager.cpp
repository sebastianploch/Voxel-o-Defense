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

	//Place model
	if (input->GetMouseState().leftButton == input->GetMouseState().PRESSED && 
		input->GetMouse().GetState().y < 855) {
		Vector3Int rayHit = VoxelRay::VoxelRaycastFromMousePos(cameraManager->GetActiveCamera(),
																input,
																winDimensions.x,
																winDimensions.y);

		//If rayHit isn't (0,0,0), place voxel model
		if(rayHit.x != 0 && rayHit.y != 0 && rayHit.z != 0)
			WorldManipulation::PlaceVoxelModel(VoxelModelManager::GetOrLoadModel(currentModel), rayHit + Vector3Int::UnitY);
	}

	//return std::vector<Vector3>();
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
	size.x++;
	size.z++;
	Vector3Int position = rayHit + Vector3Int::UnitY;

	Vector3 pos = Vector3(position.x - model->GetOrigin().x + size.x / 2,
						  position.y + 0.1f,
						  position.z - model->GetOrigin().z + size.z / 2);

	DEBUG_PRINT(dir);

	//Create vertex positions based on rotation
	std::vector<Vector3> result;
	switch (dir) {
	case 1:
		pos.x--;
		pos.z++;
		result.push_back(pos + Vector3(0, 0, 0));
		result.push_back(pos + Vector3(0, 0, -size.x));
		result.push_back(pos + Vector3(size.z, 0, -size.x));
		result.push_back(pos + Vector3(size.z, 0, 0));
		break;
	case 2:
		pos.z++;
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
		pos.x--;
		result.push_back(pos + Vector3(0, 0, 0));
		result.push_back(pos + Vector3(0, 0, size.z));
		result.push_back(pos + Vector3(size.x, 0, size.z));
		result.push_back(pos + Vector3(size.x, 0, 0));
		break;
	}

	for (auto& vert : result) {
		vert.x = std::clamp(vert.x, 0.0f, 32.0f * 15.0f);	//Chunk width * number of chunks
		vert.y = std::clamp(vert.y, 0.0f, 150.0f);			//Chunk height
		vert.z = std::clamp(vert.z, 0.0f, 32.0f * 15.0f);	//Chunk depth * number of chunks
	}

	return result;
}
