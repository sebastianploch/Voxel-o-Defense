#pragma once

#include "IGameObject.h"


class DebugSimpleCube : public IGameObject
{
public:
	DebugSimpleCube();
	~DebugSimpleCube();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext* context) override;

	static void InitBuffers(ID3D11Device* device);
	static void InitDebugTexture(const wchar_t* texturePath, ID3D11Device* device);

private:
	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_cubeVertexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11Buffer>             s_cubeIndexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> s_debugTexture;

	static DirectX::VertexPositionNormalTexture				s_cubeVertices[24];
	static WORD												s_cubeIndices[36];
};