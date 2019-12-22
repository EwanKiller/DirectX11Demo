#include "Basic.hlsli"

// Vertex shader
VertexPosWNormalTex VS_3D(VertexPosNormalTex vIn)
{
	VertexPosWNormalTex vOut;
	// clip matrix = mul(view matrix,projection matrix)
	matrix viewProj = mul(g_View, g_Proj); 
	// pos in wolrd space = mul(pos in model space,world matrix)
	float4 posW = mul(float4(vIn.Pos, 1.0f), g_World);
	// pos in clip space = mul(pos in world space,clip matrix)
	vOut.Pos = mul(posW, viewProj);
	// out pos in world space as float3
	vOut.PosW = posW.xyz;
	// calculate normal in world space
	// mul(normal in model space,world matrix's invtranspose)
	vOut.NormalW = mul(vIn.Normal, (float3x3)g_WolrdInvTranspose);
	// color
	vOut.Tex = vIn.Tex;

	return vOut;
}