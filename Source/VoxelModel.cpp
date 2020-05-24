#include "pch.h"
#include "VoxelModel.h"

VoxelModel::VoxelModel(std::string name, bool airRemovesBlocks, DirectX::SimpleMath::Vector3Int size, DirectX::SimpleMath::Vector3Int origin, std::vector<std::vector<std::vector<char>>> data) :
					   m_name(name),
					   m_airRemovesBlocks(airRemovesBlocks),
					   m_size(size),
					   m_origin(origin),
					   m_voxelData(data) {
}