#include "Camera.h"

class ThirdPersonCamera :
	public Camera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera() override;

	DirectX::XMFLOAT3 GetTargetPosition() const;
	float GetDistance() const;
	float GetRotationX() const;
	float GetRotationY() const;
	// Pitch ������ת
	float RotatePitch(float rad);
	// Yaw ������Ұ (��Ұ�Ƕ�Phi������[pi/6,pi/2]��
	float RotateYaw(float rad);
	// ������Զ
	void Approach(float dist);
	// ����pitch�Ļ���
	void SetPitch(float phi);
	// ����yaw�Ļ���
	void SetYaw(float theta);
	// ���ø���Ŀ���λ��
	void SetTargetPosition(const DirectX::XMFLOAT3& target);
	// ���ó�ʼ����
	void SetDistance(float dist);
	// ������Сor�������ľ���
	void SetDistanceLimit(float minDist, float maxDist);
	// ���¹۲����
	void UpdateViewMatrix() override;

private:
	DirectX::XMFLOAT3 m_Target;
	float m_Distance;
	float m_MinDist, m_MaxDist;
	// ����������ϵΪ��׼�£���ǰ����ת�Ƕ�
	float m_Theta, m_Phi;
};

