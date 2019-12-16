#include "Cube.hlsli"

// Vertex shader
VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.pos = mul(float4(vIn.pos, 1.0f), g_World); // ����ռ�任������ռ�
	vOut.pos = mul(vOut.pos, g_View); // ����ռ䵽�۲�ռ�
	vOut.pos = mul(vOut.pos, g_Proj); // �۲�ռ䵽ͶӰ�ռ�
	vOut.color = vIn.color;

	return vOut;
}