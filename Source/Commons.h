#pragma once


enum class Shaders
{
	BASIC_SHADER = 0,
	VOXEL_SHADER,
    WATER_SHADER,
	//TOON_SHADER,
	NONE
};

struct ConstantBuffer
{
	DirectX::SimpleMath::Matrix projection;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix world;
    float time;
    float padding1;
    float padding2;
    float padding3;
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
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer = nullptr;
    UINT m_VBStride;
    UINT m_VBOffset;
    UINT m_IndexCount;
};

/* Prints provided compatible data structure to the Output Window during runtime
   @Int's, Float's, Double's
   @String's, Char's
   @Vector3's, Vector2's
*/
template <class T>
static void DEBUG_PRINT(T val)
{
    std::string str = std::to_string(val) + "\n";
    OutputDebugStringA(str.c_str());
}
template<>
void DEBUG_PRINT(std::string str)
{
    str += "\n";
    OutputDebugStringA(str.c_str());
}
template<>
void DEBUG_PRINT(const char* msg)
{
    std::string str = msg;
    str += "\n";
    OutputDebugStringA(str.c_str());
}
template<>
void DEBUG_PRINT(DirectX::SimpleMath::Vector3 vec)
{
    std::string str = "X: " + std::to_string(vec.x);
    str += "  Y: " + std::to_string(vec.y);
    str += "  Z: " + std::to_string(vec.z);
    str += "\n";
    OutputDebugStringA(str.c_str());
}
template<>
void DEBUG_PRINT(DirectX::SimpleMath::Vector2 vec)
{
	std::string str = "X: " + std::to_string(vec.x);
	str += "  Y: " + std::to_string(vec.y);
	str += "\n";
	OutputDebugStringA(str.c_str());
}