#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC GameApp::VerTexPosColor::inputLayout[3] = 
{
	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
};

GameApp::GameApp(HINSTANCE hInstance)
	:D3DApp(hInstance),m_CBuffer()
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

	// Create Vertex shader
	HR(CreateShaderFromFile(L"HLSL\\Light_VS.cso", L"HLSL\\light_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pD3DDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
	// Create and bind input layout
	HR(m_pD3DDevice->CreateInputLayout(VerTexPosColor::inputLayout, ARRAYSIZE(VerTexPosColor::inputLayout), blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));
	// Create Pixel shader
	HR(CreateShaderFromFile(L"HLSL\\Light_PS.cso",L"HLSL\\Light_PS.hlsl","PS","ps_5_0",blob.ReleaseAndGetAddressOf()));
	HR(m_pD3DDevice->CreatePixelShader(blob->GetBufferPointer(),blob->GetBufferSize(),nullptr,m_pPixelShader.GetAddressOf()));

	return true;
}

bool GameApp::InitResource()
{
	// ******************
	// 设置立方体顶点
	//    5________ 6                      
	//    /|      /|					  
	//   /_|_____/ |					 
	//  1|4|_ _ 2|_|7					
	//   | /     | /
	//   |/______|/
	//  0       3

	// Set triangle vertices
	VerTexPosColor vertices[] = 
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};
	// Set vertex buffer description
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// Create vertex buffer
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	HR(m_pD3DDevice->CreateBuffer(&vbd, &initData, m_pVertexBuffer.GetAddressOf()));

	// Index Array
	WORD indices[] = {
		// forward
		0,1,2,
		2,3,0,
		// left
		4,5,1,
		1,0,4,
		// top
		1,5,6,
		6,2,1,
		// back
		7,6,5,
		5,4,7,
		// right
		3,2,6,
		6,7,3,
		// bottom
		4,0,3,
		3,7,4
	};
	// Set Index buffer description
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// Create index buffer
	initData.pSysMem = indices;
	HR(m_pD3DDevice->CreateBuffer(&ibd, &initData, m_pIndexBuffer.GetAddressOf()));
	// Set Index buffer
	m_pD3DImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Set constant buffer description
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// Create constant buffer
	HR(m_pD3DDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf()));
	// Init constant buffer value
	m_CBuffer.world = XMMatrixIdentity(); // 单位矩阵的转置是其本身
	m_CBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));


	/*
	给渲染管线的各阶段绑定所需的资源
	*/

	/* 渲染管线输入装配阶段 */
	// 顶点缓冲区设置
	UINT stride = sizeof(VerTexPosColor);  // 跨越字节数
	UINT offset = 0; // 起始偏移量
	m_pD3DImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	// 设置图元类型
	m_pD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 设置输入布局
	m_pD3DImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	// 将顶点色器绑定到渲染管线
	m_pD3DImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	// 将更新好的常量缓冲区绑定到顶点着色器
	m_pD3DImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	// 将像素着色器绑定到渲染管线
	m_pD3DImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	/* 设置调试对象名 */
	D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");
	D3D11SetDebugObjectName(m_pConstantBuffer.Get(), "ConstantBuffer");
	D3D11SetDebugObjectName(m_pVertexShader.Get(), "Shader_VS");
	D3D11SetDebugObjectName(m_pPixelShader.Get(), "Shader_PS");

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
	static float phi = 0.0f, theta = 0.0f;
	// Get mouse state
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	// Get keyboard state
	Keyboard::State keyState = m_pKeyBoard->GetState();
	Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();
	// update mouse button state tracker , moving when mouse pressed
	m_MouseTracker.Update(mouseState);
	m_KeyboardTracker.Update(keyState);
	if (mouseState.leftButton)
	{
	}
	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
	{
		theta -= (mouseState.x - lastMouseState.x)*0.01f;
		phi -= (mouseState.y - lastMouseState.y)*0.01f;
	}
	if (keyState.IsKeyDown(Keyboard::W))
	{
		phi += dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::S))
	{
		phi -= dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::A))
	{
		theta += dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::D))
	{
		theta -= dt * 2;
	}

	m_CBuffer.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));

	// update constant buffer and rotate cube
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pD3DImmediateContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));
	m_pD3DImmediateContext->Unmap(m_pConstantBuffer.Get(), 0);
}

void GameApp::DrawScene()
{
	assert(m_pD3DImmediateContext);
	assert(m_pSwapChain);

	static float backColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	m_pD3DImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), backColor);
	m_pD3DImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	// 绘制三角形
	m_pD3DImmediateContext->DrawIndexed(36,0,0);

	HR(m_pSwapChain->Present(0, 0));
}
