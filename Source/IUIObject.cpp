#include "pch.h"
#include "IUIObject.h"

using namespace DirectX;

IUIObject::IUIObject() :
	m_screenPos(SimpleMath::Vector2()),
	m_lifeTime(-1.0f),
	m_lifeTimeTimer(0.0f),
	m_isVisible(true)
{
}

IUIObject::~IUIObject()
{
}