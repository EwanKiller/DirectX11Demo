#pragma once
#include "Camera.h"
class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	// Set camera position
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);
	
	// Set camera forward
	void XM_CALLCONV LookAt(DirectX::FXMVECTOR pos,
		DirectX::FXMVECTOR target,
		DirectX::FXMVECTOR up);
	void LookAt(const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& target,
		const DirectX::XMFLOAT3& up);
	void XM_CALLCONV LookTo(DirectX::FXMVECTOR pos,
		DirectX::FXMVECTOR to,
		DirectX::FXMVECTOR up);
	void LookTo(const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& to,
		const DirectX::XMFLOAT3& up);

	// 平移
	void Pan(float d);
	// 直行or平面移动
	void GoStraight(float d);
	// 前进
	void MoveForward(float d);
	// 上下观察
	void Pitch(float rad);
	// 左右观察
	void Yaw(float rad);

	// 更新观察矩阵
	void UpdateViewMatrix() override;
};

