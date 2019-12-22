#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Geometry.h"
#include "LightHelper.h"

class GameApp : public D3DApp
{
public:

	struct VSConstantBuffer
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Proj;
		DirectX::XMMATRIX WorldInvTranspose;

	};

	struct PSConstantBuffer
	{
		DirectionalLight DirLight[10];
		PointLight PointLight[10];
		SpotLight SpotLight[10];
		Material Material;
		int NumDirLight;
		int NumPointLight;
		int NumSpotLight;
		float Pad;  // 打包保证16字节对齐
		DirectX::XMFLOAT4 EyePos;
	};

	enum class ShowMode{WoodCrate,FireAnim};

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

	template<class VertexType>
	bool ResetMesh(const Geometry::MeshData<VertexType>& meshData);

private:
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush; // 单色笔刷
	ComPtr<IDWriteFont>m_pFont; // 字体
	ComPtr<IDWriteTextFormat> m_pTextFormat; // 文本格式

	ComPtr<ID3D11InputLayout> m_pVertexLayout2D; // 2D顶点输入布局
	ComPtr<ID3D11InputLayout> m_pVertexLayout3D; // 3D顶点输入布局
	ComPtr<ID3D11Buffer> m_pVertexBuffer; // 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer; // 索引缓冲区
	ComPtr<ID3D11Buffer>m_pConstantBuffers[2]; // 常量缓冲区
	UINT m_IndexCount; // 绘制物体的索引数组大小
	int m_CurrFrame; // 当前帧率
	ShowMode m_CurrMode; // 当前模式

	ComPtr<ID3D11ShaderResourceView> m_pWoodCrate; // 木箱纹理
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pFireAnims; // 火焰纹理集
	ComPtr<ID3D11SamplerState> m_pSamplerState; // 采样器状态

	ComPtr<ID3D11VertexShader> m_pVertexShader3D; // 3D顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader3D; // 3D像素着色器
	ComPtr<ID3D11VertexShader> m_pVertexShader2D; // 2D顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader2D; // 2D像素着色器

	VSConstantBuffer m_VSConstantBuffer;			// 用于修改用于VS的GPU常量缓冲区的变量
	PSConstantBuffer m_PSConstantBuffer;			// 用于修改用于PS的GPU常量缓冲区的变量

	DirectionalLight m_DirLight;					// 默认环境光
	PointLight m_PointLight;						// 默认点光
	SpotLight m_SpotLight;						    // 默认汇聚光

	ComPtr<ID3D11RasterizerState> m_pRSWireframe;	// 光栅化状态: 线框模式
	bool m_IsWireframeMode;							// 当前是否为线框模式

};


#endif

