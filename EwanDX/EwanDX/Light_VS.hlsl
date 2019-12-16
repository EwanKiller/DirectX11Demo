#include "Light.hlsli"

// Vertex shader
VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	// clip matrix = mul(view matrix,projection matrix)
	matrix viewProj = mul(g_View, g_Proj); 
	// pos in wolrd space = mul(pos in model space,world matrix)
	float4 posW = mul(float4(vIn.pos, 1.0f), g_World);
	// pos in clip space = mul(pos in world space,clip matrix)
	vOut.pos = mul(posW, viewProj);
	// out pos in world space as float3
	vOut.posW = posW.xyz;
	// calculate normal in world space
	// mul(normal in model space,world matrix's invtranspose)
	vOut.normalW = mul(vIn.normal, (float3x3)g_WolrdInvTranspose);
	// color
	vOut.color = vIn.color;

	return vOut;
}