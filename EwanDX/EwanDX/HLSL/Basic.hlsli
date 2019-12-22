#include "LightHelper.hlsli"

Texture2D g_Tex : register(t0);
SamplerState g_SamLinear : register(s1);

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World; // matrix可以float4x4替代，不加row_major的情况下，矩阵默认为列主矩阵
	matrix g_View;	// 可以在前面加上 row_major 表示行主矩阵
	matrix g_Proj;  // 使用默认的列主矩阵，但需要在C++代码预先将矩阵进行转置
	matrix g_WolrdInvTranspose; // 世界矩阵的转置矩阵
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
	float3 PosW : POSITION;  // 世界坐标系下的位置
	float3 NormalW: NORMAL;  // 世界坐标系下的法向量
	float2 Tex : TEXCOORD;
};

struct VertexPosHTex
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
};