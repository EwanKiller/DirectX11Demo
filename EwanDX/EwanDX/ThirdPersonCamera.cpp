#include "ThirdPersonCamera.h"
using namespace DirectX;

ThirdPersonCamera::ThirdPersonCamera()
	: Camera(), m_Target(), m_Distance(), m_MinDist(), m_MaxDist(), m_Theta(), m_Phi()
{
}


ThirdPersonCamera::~ThirdPersonCamera()
{
}

DirectX::XMFLOAT3 ThirdPersonCamera::GetTargetPosition() const
{
	return m_Target;
}

float ThirdPersonCamera::GetDistance() const
{
	return m_Distance;
}

float ThirdPersonCamera::GetRotationX() const
{
	return m_Phi;
}

float ThirdPersonCamera::GetRotationY() const
{
	return m_Theta;
}

float ThirdPersonCamera::RotatePitch(float rad)
{
	m_Phi -= rad;
	// 将上下视野角度Phi限制在[pi/6, pi/2]，
	// 即余弦值[0, cos(pi/6)]之间
	if (m_Phi < XM_PI / 6)
		m_Phi = XM_PI / 6;
	else if (m_Phi > XM_PIDIV2)
		m_Phi = XM_PIDIV2;
}

float ThirdPersonCamera::RotateYaw(float rad)
{
	m_Theta = XMScalarModAngle(m_Theta - rad);
}

void ThirdPersonCamera::Approach(float dist)
{
	m_Distance += dist;
	// 限制距离在[m_MinDist, m_MaxDist]之间
	if (m_Distance < m_MinDist)
		m_Distance = m_MinDist;
	else if (m_Distance > m_MaxDist)
		m_Distance = m_MaxDist;
}

void ThirdPersonCamera::SetPitch(float phi)
{
	m_Phi = XMScalarModAngle(phi);
	// 将上下视野角度Phi限制在[pi/6, pi/2]，
	// 即余弦值[0, cos(pi/6)]之间
	if (m_Phi < XM_PI / 6)
		m_Phi = XM_PI / 6;
	else if (m_Phi > XM_PIDIV2)
		m_Phi = XM_PIDIV2;
}

void ThirdPersonCamera::SetYaw(float theta)
{
	m_Theta = XMScalarModAngle(theta);
}

void ThirdPersonCamera::SetTargetPosition(const DirectX::XMFLOAT3 & target)
{
	m_Target = target;
}

void ThirdPersonCamera::SetDistance(float dist)
{
	m_Distance = dist;
}

void ThirdPersonCamera::SetDistanceLimit(float minDist, float maxDist)
{
	m_MinDist = minDist;
	m_MaxDist = maxDist;
}

void ThirdPersonCamera::UpdateViewMatrix()
{
	// 球面坐标系
	float x = m_Target.x + m_Distance * sinf(m_Phi) * cosf(m_Theta);
	float z = m_Target.z + m_Distance * sinf(m_Phi) * sinf(m_Theta);
	float y = m_Target.y + m_Distance * cosf(m_Phi);
	m_Position = { x, y, z };
	XMVECTOR P = XMLoadFloat3(&m_Position);
	XMVECTOR L = XMVector3Normalize(XMLoadFloat3(&m_Target) - P);
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), L));
	XMVECTOR U = XMVector3Cross(L, R);

	// 更新向量
	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Look, L);

	m_View = {
		m_Right.x, m_Up.x, m_Look.x, 0.0f,
		m_Right.y, m_Up.y, m_Look.y, 0.0f,
		m_Right.z, m_Up.z, m_Look.z, 0.0f,
		-XMVectorGetX(XMVector3Dot(P, R)), -XMVectorGetX(XMVector3Dot(P, U)), -XMVectorGetX(XMVector3Dot(P, L)), 1.0f
	};
}
