// Light.hlsli
#include "LightHelper.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World; // matrix����float4x4���������row_major������£�����Ĭ��Ϊ��������
	matrix g_View;	// ������ǰ����� row_major ��ʾ��������
	matrix g_Proj;  // ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++����Ԥ�Ƚ��������ת��
	matrix g_WolrdInvTranspose; // ��������ת�þ���
}

cbuffer PSConstantBuffer : register(b1)
{
	DirectionalLight g_DirLight;
	PointLight g_PointLight;
	SpotLight g_SpotLight;
	Material g_Material;
	float3 g_EyePosW;
	float g_Pad;
}

struct VertexIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float3 posW : POSITION;   // ��������ϵ�µ�λ��
	float3 normalW : NORMAL;  // ��������ϵ�µķ�����
	float4 color : COLOR;
};