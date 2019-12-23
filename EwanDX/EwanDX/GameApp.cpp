#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
	,m_IndexCount()
	,m_VSConstantBuffer()
	,m_PSConstantBuffer()
	,m_CurrFrame()
	,m_CurrMode(ShowMode::WoodCrate)
	,m_DirLight()
	,m_PointLight()
	,m_SpotLight()
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
	{
		return false;
	}
	if (!InitEffect())
	{
		return false;
	}
	if (!InitResource())
	{
		return false;
	}

	// init mouse ,keyboard dont need 
	m_pMouse->SetWindow(m_hMainWnd);
	m_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	return true;
}

bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;
	// Create 2D vertex shader
	HR(CreateShaderFromFile(L"HLSL\\Basic_VS_2D.cso", L"HLSL\\Basic_VS_2D.hlsl", "VS_2D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pD3DDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader2D.GetAddressOf()));
	// Create 2D vertex layout
	HR(m_pD3DDevice->CreateInputLayout(VertexPosTex::inputLayout, ARRAYSIZE(VertexPosTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout2D.GetAddressOf()));
	// Create 2D pixel shader
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS_2D.cso", L"HLSL\\Basic_PS_2d.hlsl", "PS_2D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pD3DDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader2D.GetAddressOf()));


	// 创建顶点着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_VS_3D.cso", L"HLSL\\Basic_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pD3DDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));
	// 创建顶点布局(3D)
	HR(m_pD3DDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));

	// 创建像素着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS_3D.cso", L"HLSL\\Basic_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pD3DDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));
	return true;
}

bool GameApp::InitResource()
{
	// ******************
	// 初始化网格模型

	auto MeshData = Geometry::CreateBox();
	ResetMesh(MeshData);
	// ******************
	// 初始化纹理和采样器状态
	//

	// 初始化木箱纹理
	HR(CreateDDSTextureFromFile(m_pD3DDevice.Get(), L"Texture\\WoodCrate.dds", nullptr, m_pWoodCrate.GetAddressOf()));
	// 初始化火焰纹理
	WCHAR strFile[40];
	m_pFireAnims.resize(120);
	for (int i = 1; i <= 120; ++i)
	{
		wsprintf(strFile, L"Texture\\FireAnim\\Fire%03d.bmp", i);
		HR(CreateWICTextureFromFile(m_pD3DDevice.Get(), strFile, nullptr, m_pFireAnims[static_cast<size_t>(i) - 1].GetAddressOf()));
	}

	// 初始化采样器状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pD3DDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()));

	/* 初始化常量缓冲区 */

	// 设置常量缓冲区描述
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(VSConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建用于VS和PS的常量缓冲区
	HR(m_pD3DDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(PSConstantBuffer);
	HR(m_pD3DDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));


	// 初始化用于VS的常量缓冲区的值
	m_VSConstantBuffer.World = XMMatrixIdentity();
	m_VSConstantBuffer.View = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_VSConstantBuffer.Proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	m_VSConstantBuffer.WorldInvTranspose = XMMatrixIdentity();

	// ******************
	// 初始化默认光照

	// 方向光
	//m_DirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	//m_DirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	//m_DirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_DirLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
	// 点光
	//m_PointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	//m_PointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//m_PointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//m_PointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//m_PointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	//m_PointLight.range = 25.0f;
	// 聚光灯
	//m_SpotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	//m_SpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//m_SpotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_SpotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_SpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_SpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//m_SpotLight.spot = 12.0f;
	//m_SpotLight.range = 10000.0f;

	// 使用一盏点光
	//m_PSConstantBuffer.PointLight[0] = m_PointLight;
	m_PSConstantBuffer.PointLight[0].position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	m_PSConstantBuffer.PointLight[0].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_PSConstantBuffer.PointLight[0].diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PSConstantBuffer.PointLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PSConstantBuffer.PointLight[0].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	m_PSConstantBuffer.PointLight[0].range = 25.0f;
	m_PSConstantBuffer.NumDirLight = 0;
	m_PSConstantBuffer.NumPointLight = 1;
	m_PSConstantBuffer.NumSpotLight = 0;
	// 注意不要忘记设置此处的观察位置，否则高亮部分会有问题
	m_PSConstantBuffer.EyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);
	// 初始化材质
	m_PSConstantBuffer.Material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PSConstantBuffer.Material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PSConstantBuffer.Material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);

	// 更新PS常量缓冲区资源
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pD3DImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
	m_pD3DImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

	// ******************
	// 初始化光栅化状态
	//
	//D3D11_RASTERIZER_DESC rasterizerDesc;
	//ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	//rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterizerDesc.CullMode = D3D11_CULL_NONE;
	//rasterizerDesc.FrontCounterClockwise = false;
	//rasterizerDesc.DepthClipEnable = true;
	//HR(m_pD3DDevice->CreateRasterizerState(&rasterizerDesc, m_pRSWireframe.GetAddressOf()));

	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	//

	// 设置图元类型，设定输入布局
	m_pD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	// 将着色器绑定到渲染管线
	m_pD3DImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	// VS常量缓冲区对应HLSL寄存于b0的常量缓冲区
	m_pD3DImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
	// PS常量缓冲区对应HLSL寄存于b1的常量缓冲区
	m_pD3DImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	// 设置像素阶段的采样器
	m_pD3DImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	m_pD3DImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());
	// 将像素着色器绑定到渲染管线
	m_pD3DImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	
	/* 设置调试对象名 */
	D3D11SetDebugObjectName(m_pVertexLayout2D.Get(), "VertexPosTexLayout");
	D3D11SetDebugObjectName(m_pVertexLayout3D.Get(), "VertexPosNormalTexLayout");
	D3D11SetDebugObjectName(m_pConstantBuffers[0].Get(), "VSConstantBuffer");
	D3D11SetDebugObjectName(m_pConstantBuffers[1].Get(), "PSConstantBuffer");
	D3D11SetDebugObjectName(m_pVertexShader2D.Get(), "Basic_VS_2D");
	D3D11SetDebugObjectName(m_pVertexShader3D.Get(), "Basic_VS_3D");
	D3D11SetDebugObjectName(m_pPixelShader2D.Get(), "Basic_PS_2D");
	D3D11SetDebugObjectName(m_pPixelShader3D.Get(), "Basic_PS_3D");
	D3D11SetDebugObjectName(m_pSamplerState.Get(), "SSLinearWrap");

	return true;
}

template<class VertexType>
bool GameApp::ResetMesh(const Geometry::MeshData<VertexType>& meshData)
{
	// 释放旧资源
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexType);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(m_pD3DDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexType); // 跨越字节数
	UINT offset = 0; // 起始偏移量

	m_pD3DImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	// 设置索引缓冲区描述
	m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = m_IndexCount * sizeof(WORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(m_pD3DDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	// 输入配装阶段的索引缓冲区设置
	m_pD3DImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// 设置调试对象名
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");

	return true;	
}

void GameApp::OnResize()
{
	assert(m_pD2DFactory);
	assert(m_pDWriteFactory);
	// 释放D2D相关资源
	m_pColorBrush.Reset();
	m_pD2DRenderTarget.Reset();

	D3DApp::OnResize();

	// 为D2D创建DXGI表面渲染
	ComPtr<IDXGISurface> surface;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, m_pD2DRenderTarget.GetAddressOf());
	surface.Reset();

	if (hr == E_NOINTERFACE)
	{
		OutputDebugStringW(L"\n warning:Direct2D与Direct3D互操作性功能受限，你将无法看到文本信息。现可提供下属可选方法：\n"
			L"1. 对于Win7系统，需要更新至Win7 SP1，并安装KB2670838补丁以支持Direct2D显示。\n"
			L"2. 自行完成Direct3D 10.1与Direct2D的交互。详情参阅："
			L"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
			L"3. 使用别的字体库，比如FreeType。\n\n");
	}
	else if(hr == S_OK)
	{
		// 创建固定颜色刷和文本格式
		HR(m_pD2DRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			m_pColorBrush.GetAddressOf()));
		HR(m_pDWriteFactory->CreateTextFormat(L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn",
			m_pTextFormat.GetAddressOf()));
	}
	else
	{
		// 报告异常问题
		assert(m_pD2DRenderTarget);
	}

}

void GameApp::UpdateScene(float dt)
{
	// 键盘切换灯光类型
	Keyboard::State state = m_pKeyBoard->GetState();
	m_KeyboardTracker.Update(state);

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D1))
	{
		//设置灯光
		m_PSConstantBuffer.DirLight[0] = m_DirLight;
		m_PSConstantBuffer.PointLight[0] = PointLight();
		m_PSConstantBuffer.SpotLight[0] = SpotLight();
		m_PSConstantBuffer.NumDirLight = 1;
		m_PSConstantBuffer.NumPointLight = 0;
		m_PSConstantBuffer.NumSpotLight = 0;
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D2))
	{
		m_PSConstantBuffer.DirLight[0] = DirectionalLight();
		m_PSConstantBuffer.PointLight[0] = m_PointLight;
		m_PSConstantBuffer.SpotLight[0] = SpotLight();
		m_PSConstantBuffer.NumDirLight = 0;
		m_PSConstantBuffer.NumPointLight = 1;
		m_PSConstantBuffer.NumSpotLight = 0;
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D3))
	{
		m_PSConstantBuffer.DirLight[0] = DirectionalLight();
		m_PSConstantBuffer.PointLight[0] = PointLight();
		m_PSConstantBuffer.SpotLight[0] = m_SpotLight;
		m_PSConstantBuffer.NumDirLight = 0;
		m_PSConstantBuffer.NumPointLight = 0;
		m_PSConstantBuffer.NumSpotLight = 1;
	}

	// 键盘切换模型类型
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Q))
	{
		m_CurrMode = ShowMode::WoodCrate;
		m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateBox();
		ResetMesh(meshData);
		m_pD3DImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::W))
	{
		m_CurrMode = ShowMode::WoodCrate;
		m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateSphere();
		ResetMesh(meshData);
		m_pD3DImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::E))
	{
		m_CurrMode = ShowMode::WoodCrate;
		m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateCylinder();
		ResetMesh(meshData);
		m_pD3DImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::R))
	{
		m_CurrMode = ShowMode::WoodCrate;
		m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateCone();
		ResetMesh(meshData);
		m_pD3DImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		m_pD3DImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());
	}
	// 键盘切换光栅化状态
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::S))
	{
		m_IsWireframeMode = !m_IsWireframeMode;
		m_pD3DImmediateContext->RSSetState(m_IsWireframeMode ? m_pRSWireframe.Get() : nullptr);
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::F))
	{
		m_CurrMode = ShowMode::FireAnim;
		m_CurrFrame = 0;
		m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout2D.Get());
		auto meshData = Geometry::Create2DShow();
		ResetMesh(meshData);
		m_pD3DImmediateContext->VSSetShader(m_pVertexShader2D.Get(),nullptr,0);
		m_pD3DImmediateContext->PSSetShader(m_pPixelShader2D.Get(),nullptr,0);
		m_pD3DImmediateContext->PSSetShaderResources(0,1, m_pFireAnims[0].GetAddressOf());
	}

	if (m_CurrMode == ShowMode::WoodCrate)
	{
		static float phi = 0.0f, theta = 0.0f;
		//phi += 0.0001f, theta += 0.00015f;
		XMMATRIX w = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
		m_VSConstantBuffer.World = XMMatrixTranspose(w);
		m_VSConstantBuffer.WorldInvTranspose = XMMatrixInverse(nullptr, w);

		// 更新常量缓冲区，让立方体转起来
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pD3DImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
		m_pD3DImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);
	}
	else if(m_CurrMode == ShowMode::FireAnim)
	{
		// 用于限制1秒60帧
		static float totalDeltaTime = 0;

		totalDeltaTime += dt;
		if (totalDeltaTime > 1.0f / 60)
		{
			totalDeltaTime -= 1.0f / 60;
			m_CurrFrame = (m_CurrFrame + 1) % 120;
			m_pD3DImmediateContext->PSSetShaderResources(0, 1, m_pFireAnims[m_CurrFrame].GetAddressOf());
		}
	}
}

void GameApp::DrawScene()
{
	assert(m_pD3DImmediateContext);
	assert(m_pSwapChain);

	/* 绘制Direct3D部分 */
	m_pD3DImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));
	m_pD3DImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	// 绘制几何模型
	m_pD3DImmediateContext->DrawIndexed(m_IndexCount,0,0);
	
	/* 绘制Direct2D部分 */
	if (m_pD2DRenderTarget != nullptr)
	{
		m_pD2DRenderTarget->BeginDraw();
		std::wstring textStr = L"切换灯光类型:1-平行光 2-点光 3-聚光灯\n"
			"切换模式:Q-立方体 W-球体 E-圆柱体 R-圆锥体 F-火焰\n"
			"S-切换模式 当前模式:";
		if (m_IsWireframeMode)
		{
			textStr += L"线框模式";
		}
		else
		{
			textStr += L"面模式";
		}
		m_pD2DRenderTarget->DrawTextW(textStr.c_str(), (UINT32)textStr.size(), m_pTextFormat.Get(),
			D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, m_pColorBrush.Get());
		HR(m_pD2DRenderTarget->EndDraw());
	}

	HR(m_pSwapChain->Present(0, 0));
}
