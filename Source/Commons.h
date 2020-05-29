#pragma once


enum class Shaders
{
	BASIC_SHADER = 0,
	VOXEL_SHADER,
	//TOON_SHADER,
	NONE
};

struct ConstantBuffer
{
	DirectX::SimpleMath::Matrix projection;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix world;
};

struct JSONINFO
{

};

struct SoundsCfg : public JSONINFO
{
    std::string* type = new std::string();
    int amount;
    std::vector<std::string> paths;
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

enum VOXEL_TYPE {
    AIR = 0,
    STONE,
    GRASS,
    DIRT,
    WOOD_LOG,
    RED_BRICK,
    SAND,
    STONE_BRICK,
    MOSSY_STONE_BRICK,
    GRAVEL,
    WOOD_PLANKS,
    STONE_TILES_SMALL,
    STONE_TILES_SMALL_CRACKED,
    STONE_TILE_LARGE,
    BOOKCASE,
    REDWOOD_LOGS,
    REDWOOD_PLANKS,
};

struct VoxelMesh {
    ID3D11Buffer* m_VertexBuffer;
    ID3D11Buffer* m_IndexBuffer;
    UINT m_VBStride;
    UINT m_VBOffset;
    UINT m_IndexCount;
};

#pragma region STRING_CONVERSION
static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> s_converter;

// Helper function to convert string into wstring
static std::wstring TO_WSTRING(const std::string& string)
{
    std::wstring str = s_converter.from_bytes(string);
    return str;
}

// Helper function to convert string into wstring
static std::string TO_STRING(const std::wstring& string)
{
    std::string str = s_converter.to_bytes(string);
    return str;
}
#pragma endregion STRING_CONVERSION