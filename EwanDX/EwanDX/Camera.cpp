#include "Camera.h"
using namespace DirectX;

Camera::Camera()
	: m_Position(0.0f,0.0f,0.0f)
	,m_Right(0.0f,0.0f,0.0f)
	,m_Up(0.0f,0.0f,0.0f)
	,m_Look(0.0f,0.0f,0.0f)
	,m_NearZ()
	,m_FarZ()
	,m_FovY()
	,m_Aspect()
	,m_NearWindowHeight()
	,m_FarWindowHeight()
	,m_View()
	,m_Proj()
	,m_ViewPort()
{
}
Camera::~Camera()
{
}

DirectX::XMVECTOR Camera::GetPositionXM() const
{
	return XMLoadFloat3(&m_Position);
}

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return m_Position;
}

DirectX::XMVECTOR Camera::GetRightXM() const
{
	return XMLoadFloat3(&m_Right);
}

DirectX::XMFLOAT3 Camera::GetRight() const
{
	return m_Right;
}

DirectX::XMVECTOR Camera::GetUpXM() const
{
	return XMLoadFloat3(&m_Up);
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return m_Up;
}

DirectX::XMVECTOR Camera::GetLookXM() const
{
	return XMLoadFloat3(&m_Look);
}

DirectX::XMFLOAT3 Camera::GetLook() const
{
	return m_Look;
}

float Camera::GetNearWindowWidth() const
{
	return m_Aspect * m_NearWindowHeight;
}

float Camera::GetNearWindowHeight() const
{
	return m_NearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return m_Aspect * m_FarWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return m_FarWindowHeight;
}

DirectX::XMMATRIX Camera::GetViewXM() const
{
	return XMLoadFloat4x4(&m_View);
}

DirectX::XMMATRIX Camera::GetProjXM() const
{
	return XMLoadFloat4x4(&m_Proj);
}

DirectX::XMMATRIX Camera::GetViewProjXM() const
{
	return XMLoadFloat4x4(&m_View) * XMLoadFloat4x4(&m_Proj);
}

D3D11_VIEWPORT Camera::GetViewPort() const
{
	return m_ViewPort;
}

void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ));
}

void Camera::SetViewPort(const D3D11_VIEWPORT & viewPort)
{
	m_ViewPort = viewPort;
}

void Camera::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewPort.TopLeftX = topLeftX;
	m_ViewPort.TopLeftY = topLeftY;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = minDepth;
	m_ViewPort.MaxDepth = maxDepth;
}
