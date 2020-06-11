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
	m_Scale = Vector3(45.0f, 42.0f, 45.0f);
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
	// Apply Perspective Projection to constant Buffer
	cb.projection = m_cmRef.GetFPSCamera()->GetProjection();

	// Get view from origin in perspective
	if (m_cmRef.GetActiveCamera()->GetType() == CAMERA_TYPE::ORTHOGRAPHIC)
	{
		cb.view = m_cmRef.GetFPSCamera()->GetViewAtOrigin(m_cmRef.GetISOCamera()->GetYaw());
	}
	else
	{
		cb.view = m_cmRef.GetFPSCamera()->GetView();
	}

	// Turn Back Face-Culling and Switch Depth test
	context->RSSetState(m_csRef.CullNone());
	context->OMSetDepthStencilState(m_depthStencilView.Get(), 0);

	// Draw Skybox
	Model::Draw(context, cb, constantBuffer);

	// Switch back to regular Culling and Depth Test States
	context->OMSetDepthStencilState(m_csRef.DepthDefault(), 0);
	context->RSSetState(m_csRef.CullClockwise());

	// Switch constant buffer var's back to active camera
	cb.projection = m_cmRef.GetActiveCamera()->GetProjection();
	cb.view = m_cmRef.GetActiveCamera()->GetView();
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

void Skybox::UpdatePosition()
{
	m_Translation = m_cmRef.GetFPSCamera()->GetPosition();
}
