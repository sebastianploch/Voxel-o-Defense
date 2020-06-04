#include "pch.h"
#include "JSONLoader.h"

void JSONLoader::LoadFile(std::string filePath, GameObjectCfg& cfg, std::string name)
{
	json config = _CHECKFILE(filePath);
	try
	{ //try to set game objects members
		cfg.type = &name;
		cfg.m.ambient = XMFLOAT4(config[name]["material"]["ambient"][0], config[name]["material"]["ambient"][1], config[name]["material"]["ambient"][2], config[name]["material"]["ambient"][3]);
		cfg.m.specular = XMFLOAT4(config[name]["material"]["specular"][0], config[name]["material"]["specular"][1], config[name]["material"]["specular"][2], config[name]["material"]["specular"][3]);
		cfg.m.diffuse = XMFLOAT4(config[name]["material"]["diffuse"][0], config[name]["material"]["diffuse"][1], config[name]["material"]["diffuse"][2], config[name]["material"]["diffuse"][3]);
		cfg.m.specularPower = config[name]["material"]["specularPower"];

		cfg.pos = XMFLOAT3(config[name]["pos"][0], config[name]["pos"][1], config[name]["pos"][2]);
		cfg.rot = XMFLOAT3(config[name]["rot"][0], config[name]["rot"][1], config[name]["rot"][2]);
		cfg.scale = XMFLOAT3(config[name]["scale"][0], config[name]["scale"][1], config[name]["scale"][2]); //standard json format we load using jsonObject[objectname][otherpotentialname][0]
	}
	catch (exception e)
	{
		OutputDebugStringA("Error creating cube attributes from JSON file!");
	}
}

void JSONLoader::LoadFile(std::string filePath, CamCfg& cfg, std::string name)
{
	json j = _CHECKFILE(filePath);
	try
	{
		cfg.eye = XMFLOAT3(j[name]["eye"][0], j[name]["eye"][1], j[name]["eye"][2]);
		cfg.at = XMFLOAT3(j[name]["at"][0], j[name]["at"][1], j[name]["at"][2]);
		cfg.up = XMFLOAT3(j[name]["up"][0], j[name]["up"][1], j[name]["up"][2]);
		cfg.type = &name;
	}
	catch (exception e)
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
	case GAMEOBJECT:
		gObjCfg = static_cast<GameObjectCfg*>(&cfg); //Cast the object to a game object and load it into the file if config_type = game object
		LoadFile(filePath, *gObjCfg, name);
		cfg = static_cast<JSONINFO>(*gObjCfg); //cast back to our type we need
		break;
	case CAMERA:
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
		std::ifstream jsonObject(filePath);
		nlohmann::json jsonFile;
		jsonObject >> jsonFile;
		return jsonObject;
	}
	catch (std::exception e)
	{
		OutputDebugStringA("Error loading file into project!");
	}
}

void JSONLoader::CreateConfig(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 up, DirectX::XMFLOAT3 at, std::string fileName, std::string type)
{
	json j;
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
	catch (exception e)
	{
		OutputDebugStringA("Error creating camera config json file!");
	}
}

void JSONLoader::CreateConfig(CamCfg config, std::string fileName, std::string type)
{
	json j;
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
	catch (exception e)
	{
		OutputDebugStringA("Error creating Camera config json file!");
	}
}

void JSONLoader::CreateConfig(GameObjectCfg object, std::string fileName, std::string type)
{
	json j;
	try
	{
		j["type"] = type;
		j[type]["material"]["ambient"][0] = object.m.ambient.x;  j[type]["material"]["ambient"][1] = object.m.ambient.y;
		j[type]["material"]["ambient"][2] = object.m.ambient.z;  j[type]["material"]["ambient"][3] = object.m.ambient.w;

		j[type]["material"]["specular"][0] = object.m.specular.x; j[type]["material"]["specular"][1] = object.m.specular.y;
		j[type]["material"]["specular"][2] = object.m.specular.z; j[type]["material"]["specular"][3] = object.m.specular.w;

		j[type]["material"]["diffuse"][0] = object.m.diffuse.x; j[type]["material"]["diffuse"][1] = object.m.diffuse.y;
		j[type]["material"]["diffuse"][2] = object.m.diffuse.z; j[type]["material"]["diffuse"][3] = object.m.diffuse.w;
		j[type]["material"]["specularPower"] = object.m.specularPower;

		j[type]["pos"][0] = object.pos.x; j[type]["pos"][1] = object.pos.y; j[type]["pos"][2] = object.pos.z;
		j[type]["rot"][0] = object.rot.x; j[type]["rot"][1] = object.rot.y; j[type]["rot"][2] = object.rot.z;
		j[type]["scale"][0] = object.scale.x; j[type]["scale"][1] = object.scale.y; j[type]["scale"][2] = object.scale.z;

		if (OpenAndCreateFile(fileName, j) == false)
			throw;
	}
	catch (exception e)
	{
		OutputDebugStringA("Error creating Camera config file!");
	}
}

bool JSONLoader::OpenAndCreateFile(std::string path, nlohmann::json j)
{
	try
	{ //try to open and append the file, otherwise just throw
		std::fstream file;
		file.open(path, fstream::app);
		file << j;
		file.close();
		return true;
	}
	catch (exception e)
	{
		return false;
	}
	return false;
}
#pragma endregion STATICS