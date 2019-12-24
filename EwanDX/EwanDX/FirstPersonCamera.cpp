#include "FirstPersonCamera.h"
using namespace DirectX;

FirstPersonCamera::FirstPersonCamera()
{
}


FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
	SetPosition(DirectX::XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const DirectX::XMFLOAT3& v)
{
	m_Position = v;
}

void XM_CALLCONV FirstPersonCamera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up)
{
	LookTo(pos, target - pos, up);
}
