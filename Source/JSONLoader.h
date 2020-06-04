#pragma once
namespace JSONLoader
{
	enum class CONFIG_TYPE
	{
		GAMEOBJECT = 0,
		CAMERA = 1,
		SOUNDS = 2,
	};

	void LoadFile(std::string filePath, GameObjectCfg& cfg, std::string name);
	void LoadFile(std::string filePath, CamCfg& cfg, std::string name);
	void LoadFile(std::string filePath, SoundsCfg& cfg, std::string name);
	void LoadFile(std::string filePath, JSONINFO& cfg, CONFIG_TYPE cfgType, std::string name); //Load different jsoninfo types to perform their specified file loaders
 	nlohmann::json _CHECKFILE(std::string filePath);

	void CreateConfig(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up, DirectX::SimpleMath::Vector3 at, std::string fileName, std::string type); //Camera loader for specified data at runtime
	void CreateConfig(CamCfg config, std::string fileName, std::string type); //Loaders for config types
	void CreateConfig(GameObjectCfg object, std::string fileName, std::string type);
	bool OpenAndCreateFile(std::string path, nlohmann::json j);
};