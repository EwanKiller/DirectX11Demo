// Cube.hlsli

cbuffer ConstantBuffer : register(b0)
{
	matrix g_World; // matrix����float4x4���������row_major������£�����Ĭ��Ϊ��������
	matrix g_View;	// ������ǰ����� row_major ��ʾ��������
	matrix g_Proj;  // ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++����Ԥ�Ƚ��������ת��
}

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};