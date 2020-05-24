#pragma once
#include "VoxelModel.h"


class VoxelModelManager {
public:
	static VoxelModel* GetOrLoadModel(std::string fileDir);

private:
	static std::vector<VoxelModel> s_modelList;
};