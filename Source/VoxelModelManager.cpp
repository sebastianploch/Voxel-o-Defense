#include "pch.h"
#include "VoxelModelManager.h"
#include <fstream>

using namespace DirectX::SimpleMath;

std::vector<VoxelModel> VoxelModelManager::s_modelList = {};

VoxelModel* VoxelModelManager::GetOrLoadModel(std::string fileDir) {
	//Check to see if model is already loaded
	for (auto& model : s_modelList) {
		if (model.GetName() == fileDir)
			return &model;
	}

	//If less than 5 chars long, it can't have .vxml on the end
	if (fileDir.size() < 5)	
		return nullptr;

	//Verify file type
	if (fileDir.substr(fileDir.size() - 5) != ".vxml")	
		return nullptr;

	std::fstream fin(fileDir, std::ios::in | std::ios::binary);
	if (fin.is_open()) {
		//Declare variables to be loaded
		int				  versionNumber;
		bool			  airRemovesBlocks;
		Vector3Int		  size;
		Vector3Int		  min;
		Vector3Int		  origin;
		std::vector<std::vector<std::vector<char>>> data;

		//Load data from file
		fin.read((char*)&versionNumber, 4);
		fin.read((char*)&airRemovesBlocks, 1);
		fin.read((char*)&size.x, 4);	fin.read((char*)&size.y, 4);	fin.read((char*)&size.z, 4);
		fin.read((char*)&min.x, 4);		fin.read((char*)&min.y, 4);		fin.read((char*)&min.z, 4);
		fin.read((char*)&origin.x, 4);	fin.read((char*)&origin.y, 4);	fin.read((char*)&origin.z, 4);
		
		for (int x = 0; x <= size.x; x++) {
			data.push_back(std::vector<std::vector<char>>());
			for (int y = 0; y <= size.y; y++) {
				data[x].push_back(std::vector<char>());
				for (int z = 0; z <= size.z; z++) {
					data[x][y].push_back(0x00);
					fin.read((char*)&data[x][y][z], 1);
				}
			}
		}
			
		fin.close();

		s_modelList.push_back(VoxelModel(fileDir, airRemovesBlocks, size, Vector3Int(origin.x - min.x, origin.y - min.y, origin.z - min.z), data));
		return &s_modelList.back();
	}

	return nullptr;
}
