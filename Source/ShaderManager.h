#pragma once
#include "Shader.h"


class ShaderManager
{
public:
	ShaderManager(ID3D11Device1* device);
	~ShaderManager();

	void SetShader(const Shaders& shader, ID3D11DeviceContext1* context);

	void InitialiseShaders(ID3D11Device1* device);

private:
	Shaders								 m_currentShader;
	std::vector<std::unique_ptr<Shader>> m_shaders;
};