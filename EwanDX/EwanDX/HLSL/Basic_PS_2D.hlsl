#include "Basic.hlsli"

// ������ɫ����2D��
float4 PS_2D(VertexPosTex pIn) : SV_Target
{
	return g_Tex.Sample(g_SamLinear,pIn.Tex);
}