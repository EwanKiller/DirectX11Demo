#include "Basic.hlsli"

// Pixel shader
float4 PS_3D(VertexPosWNormalTex pIn) : SV_Target
{
	// normalize normal in world space
	pIn.NormalW = normalize(pIn.NormalW);

	// vector in world space from vertex to eye
	float3 toEyeW = normalize(g_EyePosW - pIn.PosW);

	// init
	float4 ambient, diffuse, spec;
	float4 A, D, S;
	ambient = diffuse = spec = A = D = S =
		float4(0.0f, 0.0f, 0.0f, 0.0f);

	int i;
	// 强制展开循环以减少指令数
	[unroll]
	for (i = 0; i < g_NumDirLight; ++i)
	{
		ComputeDirectionalLight(g_Material, g_DirLight[i], pIn.NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	[unroll]
	for ( i = 0; i < g_NumPointLight; ++i)
	{
		ComputePointLight(g_Material, g_PointLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	[unroll]
	for ( i = 0; i < g_NumSpotLight; ++i)
	{
		ComputeSpotLight(g_Material, g_SpotLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 texColor = g_Tex.Sample(g_SamLinear, pIn.Tex);
	float4 litColor = texColor * (ambient + diffuse) + spec;
	litColor.a = texColor.a * g_Material.Diffuse.a;

	return litColor;
}