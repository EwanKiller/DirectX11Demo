#ifndef GAMEAPP_H
#define GAMEAPP_H

#include"d3dApp.h"

class GameApp : public D3DApp
{
public:
	struct VerTexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	// 物体表面材质
	struct Material 
	{
		Material() { memset(this, 0, sizeof(Material)); }
		
		DirectX::XMFLOAT4 Ambient;  // 环境光
		DirectX::XMFLOAT4 Diffuse;  // 漫射光
		DirectX::XMFLOAT4 Specular; // w = 镜面反射强度
		DirectX::XMFLOAT4 Reflect;  // 反射强度
	};

	// 方向光
	struct DirectionalLight
	{
		DirectionalLight() { memset(this, 0, sizeof(DirectionalLight)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT3 Direction;
		float Pad;  //用一个浮点数填充使结构体大小满足16倍数，便于在HLSL设置数组
	};

	//点光源
	struct PointLight
	{
		PointLight() { memset(this, 0, sizeof(PointLight)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		// 打包成4D向量 (Position,Range)
		DirectX::XMFLOAT3 Position;
		float Range;

		// 打包成4D向量 （A0,A1,A2,Pad）
		DirectX::XMFLOAT3 Att; // Attenuation
		float Pad;
	};

	// 聚光灯
	struct SpotLight
	{
		SpotLight() { memset(this, 0, sizeof(SpotLight)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		DirectX::XMFLOAT3 Position;
		float Range;

		DirectX::XMFLOAT3 Direction;
		float Spot;

		DirectX::XMFLOAT3 Att;
		float Pad;
	};


public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	
private:

	bool InitEffect();
	bool InitResource();

private:
	ComPtr<ID3D11InputLayout> m_pVertexLayout; // 顶点输入布局
	ComPtr<ID3D11Buffer> m_pVertexBuffer; // 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer; // 索引缓冲区
	ComPtr<ID3D11Buffer>m_pConstantBuffer; // 常量缓冲区

	ComPtr<ID3D11VertexShader> m_pVertexShader; // 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader; //像素着色器

	ConstantBuffer m_CBuffer; // 用于修改GPU常量缓冲区的变量

};


#endif

