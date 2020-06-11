#include "pch.h"
#include "Skybox.h"

using namespace DirectX;
using DirectX::SimpleMath::Vector3;


Skybox::Skybox(ID3D11Device1* device,
			   CameraManager& cameraManager,
			   DirectX::CommonStates& states) :
	Model(),
	m_cmRef(cameraManager),
	m_csRef(states)
{
	Initialise("Resources/Models/Mesh/skydome/skydome.obj", device);
	InitDepthDesc(device);
	m_Scale = Vector3(50.0f, 50.0f, 50.0f);
	UpdatePosition();
}

Skybox::~Skybox()
{

}

void Skybox::Update(float deltaTime)
{
	UpdatePosition();

	Model::Update(deltaTime);
}

void Skybox::Draw(ID3D11DeviceContext1* context,
				  ConstantBuffer& cb,
				  ID3D11Buffer* constantBuffer)
{
	context->RSSetState(m_csRef.CullNone());
	context->OMSetDepthStencilState(m_depthStencilView.Get(), 0);

	Model::Draw(context, cb, constantBuffer);

	context->OMSetDepthStencilState(m_csRef.DepthDefault(), 0);
	context->RSSetState(m_csRef.CullClockwise());
}

void Skybox::UpdatePosition()
{
	auto activeCamera = m_cmRef.GetActiveCamera();
	m_Translation = activeCamera->GetPosition();
}

void Skybox::InitDepthDesc(ID3D11Device1* device)
{
	CD3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(CD3D11_DEPTH_STENCIL_DESC));
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthDesc, m_depthStencilView.ReleaseAndGetAddressOf());
}