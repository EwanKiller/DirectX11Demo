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
	// Pitch 左右旋转
	float RotatePitch(float rad);
	// Yaw 上下视野 (视野角度Phi限制在[pi/6,pi/2]）
	float RotateYaw(float rad);
	// 拉近推远
	void Approach(float dist);
	// 设置pitch的弧度
	void SetPitch(float phi);
	// 设置yaw的弧度
	void SetYaw(float theta);
	// 设置跟踪目标的位置
	void SetTargetPosition(const DirectX::XMFLOAT3& target);
	// 设置初始距离
	void SetDistance(float dist);
	// 设置最小or最大允许的距离
	void SetDistanceLimit(float minDist, float maxDist);
	// 更新观察矩阵
	void UpdateViewMatrix() override;

private:
	DirectX::XMFLOAT3 m_Target;
	float m_Distance;
	float m_MinDist, m_MaxDist;
	// 以世界坐标系为基准下，当前的旋转角度
	float m_Theta, m_Phi;
};

