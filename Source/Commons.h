#pragma once

struct ConstantBuffer
{
	DirectX::SimpleMath::Matrix projection;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix world;
};

struct JSONINFO
{

};

struct GameObjectCfg : public JSONINFO
{
	std::string* type = new std::string();
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 rotation;
	DirectX::SimpleMath::Vector3 scale;
};

struct CamCfg : public JSONINFO
{
	DirectX::SimpleMath::Vector3 eye;
	DirectX::SimpleMath::Vector3 up;
	DirectX::SimpleMath::Vector3 at;
	std::string* type = new std::string();
};
//
//enum class EVENT
//{
//	CLICK = 0
//};