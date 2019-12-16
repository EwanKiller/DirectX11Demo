// Light.hlsli
#include "LightHelper.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World; // matrix可以float4x4替代，不加row_major的情况下，矩阵默认为列主矩阵
	matrix g_View;	// 可以在前面加上 row_major 表示行主矩阵
	matrix g_Proj;  // 使用默认的列主矩阵，但需要在C++代码预先将矩阵进行转置
	matrix g_WolrdInvTranspose; // 世界矩阵的转置矩阵
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
	float3 posW : POSITION;   // 世界坐标系下的位置
	float3 normalW : NORMAL;  // 世界坐标系下的法向量
	float4 color : COLOR;
};