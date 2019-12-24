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

	// ƽ��
	void Pan(float d);
	// ֱ��orƽ���ƶ�
	void GoStraight(float d);
	// ǰ��
	void MoveForward(float d);
	// ���¹۲�
	void Pitch(float rad);
	// ���ҹ۲�
	void Yaw(float rad);

	// ���¹۲����
	void UpdateViewMatrix() override;
};

