#include "LightHelper.hlsli"

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s1);

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World; // matrix����float4x4���������row_major������£�����Ĭ��Ϊ��������
	matrix g_View;	// ������ǰ����� row_major ��ʾ��������
	matrix g_Proj;  // ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++����Ԥ�Ƚ��������ת��
	matrix g_WolrdInvTranspose; // ��������ת�þ���
}

cbuffer PSConstantBuffer : register(b1)
{
	DirectionalLight g_DirLight[10];
	PointLight g_PointLight[10];
	SpotLight g_SpotLight[10];
	Material g_Material;
	int g_NumDirLight;
	int g_NumPointLight;
	int g_NumSpotLight;
	float g_Pad1;

	float3 g_EyePosW;
	float g_Pad2;
}

struct VertexPosNormalTex
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexPosTex
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct VertexPosWNormalTex
{
	float4 Pos : SV_POSITION;
	float3 PosW : POSITION;  // ��������ϵ�µ�λ��
	float3 NormalW: NORMAL;  // ��������ϵ�µķ�����
	float2 Tex : TEXCOORD;
};

struct VertexPosHTex
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
};