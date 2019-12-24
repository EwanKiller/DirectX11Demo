#include "Camera.h"

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

Camera::Camera()
	:m_Position(0.0f,0.0f,0.0f)
	,m_Right(0.0f,0.0f,0.0f)
	,m_Up(0.0f,0.0f,0.0f)
	,m_Look(0.0f,0.0f,0.0f)
	,m_NearZ(),m_FarZ()
	,m_FovY()
	,m_Aspect()
	,m_NearWindowHeight()
	,m_FarWindowHeight()
	,m_View(),m_Proj(),m_ViewPort()
{
}
