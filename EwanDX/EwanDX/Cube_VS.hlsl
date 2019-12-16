#include "Cube.hlsli"

// Vertex shader
VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.pos = mul(float4(vIn.pos, 1.0f), g_World); // 对象空间变换到世界空间
	vOut.pos = mul(vOut.pos, g_View); // 世界空间到观察空间
	vOut.pos = mul(vOut.pos, g_Proj); // 观察空间到投影空间
	vOut.color = vIn.color;

	return vOut;
}