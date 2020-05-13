#include "pch.h"
#include "JSONLoader.h"

void JSONLoader::LoadFile(std::string filePath, GameObjectCfg& cfg, std::string name)
{
	nlohmann::json config = _CHECKFILE(filePath);
	try
	{ //try to set game objects members
		cfg.type = &name;
		cfg.position = DirectX::SimpleMath::Vector3(config[name]["pos"][0], config[name]["pos"][1], config[name]["pos"][2]);
		cfg.rotation = DirectX::SimpleMath::Vector3(config[name]["rot"][0], config[name]["rot"][1], config[name]["rot"][2]);
		cfg.scale = DirectX::SimpleMath::Vector3(config[name]["scale"][0], config[name]["scale"][1], config[name]["scale"][2]); //standard json format we load using jsonObject[objectname][otherpotentialname][0]
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error creating cube attributes from JSON file!");
	}
}

void JSONLoader::LoadFile(std::string filePath, CamCfg& cfg, std::string name)
{
	nlohmann::json file = _CHECKFILE(filePath);
	try
	{
		cfg.eye = DirectX::XMFLOAT3(file[name]["eye"][0], file[name]["eye"][1], file[name]["eye"][2]);
		cfg.at = DirectX::XMFLOAT3(file[name]["at"][0], file[name]["at"][1], file[name]["at"][2]);
		cfg.up = DirectX::XMFLOAT3(file[name]["up"][0], file[name]["up"][1], file[name]["up"][2]);
		cfg.type = &name;
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error creating camera attributes from JSON file!");
	}
}

void JSONLoader::LoadFile(std::string filePath, JSONINFO& cfg, CONFIG_TYPE cfgType, std::string name)
{
	GameObjectCfg* gObjCfg; //create local members since switch statements can't hold members
	CamCfg* camCfg;
	
	switch (cfgType)
	{
	case CONFIG_TYPE::GAMEOBJECT:
		gObjCfg = static_cast<GameObjectCfg*>(&cfg); //Cast the object to a game object and load it into the file if config_type = game object
		LoadFile(filePath, *gObjCfg, name);
		cfg = static_cast<JSONINFO>(*gObjCfg); //cast back to our type we need
		break;
	case CONFIG_TYPE::CAMERA:
		camCfg = static_cast<CamCfg*>(&cfg);
		LoadFile(filePath, *camCfg, name);
		cfg = static_cast<JSONINFO>(*camCfg);
		break;
	default:
		break;
	}
}

nlohmann::json JSONLoader::_CHECKFILE(std::string filePath)
{
	try //try to load our file, if we find an error, throw an exception
	{
		std::ifstream jsonFile(filePath);
		nlohmann::json jsonObject;
		jsonFile >> jsonObject;
		return jsonObject;
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error loading file into project!");
	}
}

void JSONLoader::CreateConfig(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up, DirectX::SimpleMath::Vector3 at, std::string fileName, std::string type)
{
	nlohmann::json j;
	try { //create config for camera attribs passed in
		j["type"] = type;

		j[type]["eye"][0] = eye.x;
		j[type]["eye"][1] = eye.y;
		j[type]["eye"][2] = eye.z;

		j[type]["up"][0] = up.x;
		j[type]["up"][1] = up.y;
		j[type]["up"][2] = up.z;

		j[type]["at"][0] = at.x;
		j[type]["at"][1] = at.y;
		j[type]["at"][2] = at.z;

		if (OpenAndCreateFile(fileName, j) == false) //if we couldn't open or create the file then this throws as false
			throw;
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error creating camera config json file!");
	}
}

void JSONLoader::CreateConfig(CamCfg config, std::string fileName, std::string type)
{
	nlohmann::json j;
	try {
		j["type"] = type;

		j[type]["eye"][0] = config.eye.x;
		j[type]["eye"][1] = config.eye.y;
		j[type]["eye"][2] = config.eye.z;

		j[type]["up"][0] = config.up.x;
		j[type]["up"][1] = config.up.y;
		j[type]["up"][2] = config.up.z;

		j[type]["at"][0] = config.at.x;
		j[type]["at"][1] = config.at.y;
		j[type]["at"][2] = config.at.z;

		if (OpenAndCreateFile(fileName, j) == false)
			throw;
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error creating Camera config json file!");
	}
}

void JSONLoader::CreateConfig(GameObjectCfg object, std::string fileName, std::string type)
{
	nlohmann::json j;
	try
	{
		j["type"] = type;
		j[type]["pos"][0] = object.position.x; j[type]["pos"][1] = object.position.y; j[type]["pos"][2] = object.position.z;
		j[type]["rot"][0] = object.rotation.x; j[type]["rot"][1] = object.rotation.y; j[type]["rot"][2] = object.rotation.z;
		j[type]["scale"][0] = object.scale.x; j[type]["scale"][1] = object.scale.y; j[type]["scale"][2] = object.scale.z;

		if (OpenAndCreateFile(fileName, j) == false)
			throw;
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error creating Camera config file!");
	}
}

bool JSONLoader::OpenAndCreateFile(std::string path, nlohmann::json j)
{
	try
	{ //try to open and append the file, otherwise just throw
		std::fstream file;
		file.open(path, std::fstream::app);
		file << j;
		file.close();
		return true;
	}
	catch (std::exception e)
	{
		return false;
	}
	return false;
}