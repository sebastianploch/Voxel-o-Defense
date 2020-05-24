#pragma once

class VoxelModel {
private:
	std::string m_name;
	bool m_airRemovesBlocks;
	DirectX::SimpleMath::Vector3Int m_size;
	DirectX::SimpleMath::Vector3Int m_origin;
	std::vector<std::vector<std::vector<char>>> m_voxelData;

public:
	VoxelModel(std::string name, bool airRemovesBlocks, DirectX::SimpleMath::Vector3Int size, DirectX::SimpleMath::Vector3Int origin, std::vector<std::vector<std::vector<char>>> data);

	inline const std::string GetName()						 { return m_name; }
	inline const bool GetAirRemoveBlocks()					 { return m_airRemovesBlocks; }
	inline const DirectX::SimpleMath::Vector3Int GetSize()	 { return m_size; }
	inline const DirectX::SimpleMath::Vector3Int GetOrigin() { return m_origin; }
	inline const std::vector<std::vector<std::vector<char>>> GetVoxelData()			 { return m_voxelData; }
};