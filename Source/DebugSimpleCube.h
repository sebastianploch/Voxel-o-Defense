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

private:

private:
	static Microsoft::WRL::ComPtr<ID3D11Buffer> s_cubeVertexBuffer;
	static Microsoft::WRL::ComPtr<ID3D11Buffer> s_cubeIndexBuffer;

	static DirectX::VertexPositionColor			s_cubeVertices[24];
	static WORD									s_cubeIndices[36];
};