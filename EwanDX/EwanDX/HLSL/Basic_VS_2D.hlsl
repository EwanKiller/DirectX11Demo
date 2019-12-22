// Basic_VS_2D.hlsl
#include "Basic.hlsli"

// ������ɫ����2D��
VertexPosHTex VS_2D(VertexPosTex vIn)
{
	VertexPosHTex vOut;
	vOut.Pos = float4(vIn.Pos, 1.0f);
	vOut.Tex = vIn.Tex;
	return vOut;
}