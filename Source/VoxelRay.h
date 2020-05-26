#pragma once

class VoxelRay {
public:
	static DirectX::SimpleMath::Vector3Int VoxelRaycast(DirectX::SimpleMath::Vector3 ray_start, DirectX::SimpleMath::Vector3 ray_end);
};