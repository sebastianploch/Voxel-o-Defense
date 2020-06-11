#pragma once
#include "Model.h"


class Skybox : public Model
{
public:
	Skybox(ID3D11Device1* device,
		   CameraManager& cameraManager,
		   DirectX::CommonStates& states);

	~Skybox();

	void Update(float deltaTime) override;
	void Draw(ID3D11DeviceContext1* context,
			  ConstantBuffer& cb,
			  ID3D11Buffer* constantBuffer) override;

private:
	void UpdatePosition();
	void InitDepthDesc(ID3D11Device1* device);

private:
	CameraManager& m_cmRef;
	DirectX::CommonStates& m_csRef;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilView;
};