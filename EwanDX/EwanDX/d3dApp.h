#ifndef D3DAPP_H
#define D3DAPP_H

#include <wrl/client.h>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "GameTimer.h"

#pragma  comment(lib,"d2d1.lib")
#pragma  comment(lib,"dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")


class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();
	// 获取应用实例的句柄
	HINSTANCE AppInst() const;
	// 获取主窗口句柄
	HWND MainWnd() const;
	// 获取屏幕宽高比
	float AspectRatio() const;
	// 游戏主循环
	int Run();
	
	/*
	框架方法，派生类需要重载以下函数
	*/
	// 初始化窗口和Direct3D
	virtual bool Init();
	// 需要在窗口大小变动的时候调用
	virtual void OnResize();
	// 完成每一帧的更新
	virtual void UpdateScene(float dt) = 0;
	// 完成每一帧的绘制
	virtual void DrawScene() = 0;
	// 消息处理
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	/* 窗口的消息回调函数 */
protected:
	// 窗口初始化
	bool InitMainWindow();
	// Direct2D初始化
	bool InitDirect2D();
	// Direct3D初始化
	bool InitDirect3D();
	// 计算每秒帧数并且在窗口中显示
	void CalculateFrameStates();

protected:
	// 获取实例句柄
	HINSTANCE m_hAppInst;
	// 主窗口句柄
	HWND m_hMainWnd;
	// 应用是否暂停
	bool m_AppPaused;
	// 应用是否最小化
	bool m_Minimized;
	// 应用是否最大化
	bool m_Maximized;
	// 窗口大小是否变化
	bool m_Resizing;
	// 是否开启4倍多重采样
	bool m_Enable4xMsaa;
	// MSAA支持的质量等级
	UINT m_4xMsaaQuality;

	// 计时器
	GameTimer m_Timer;

	// 使用模板简化类型名
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	/* Direct2D */
	// D2D工厂
	ComPtr<ID2D1Factory> m_pD2DFactory; 
	// D2D渲染目标
	ComPtr<ID2D1RenderTarget> m_pD2DRenderTarget;
	// DWrite工厂
	ComPtr<IDWriteFactory> m_pDWriteFactory;

	/* Direct3D 11 */
	// D3D11设备
	ComPtr<ID3D11Device> m_pD3DDevice;
	// D3D设备上下文
	ComPtr<ID3D11DeviceContext> m_pD3DImmediateContext;
	// D3D交换链
	ComPtr<IDXGISwapChain> m_pSwapChain;

	/* Direct3D 11.1 */
	// D3D11.1设备
	ComPtr<ID3D11Device1> m_pD3DDevice1;
	// D3D11.1设备上下文
	ComPtr<ID3D11DeviceContext1> m_pD3DImmediateContext1;
	// D3D11.1交换链
	ComPtr<IDXGISwapChain1> m_pSwapChain1;

	/* 常用资源*/
	// 深度缓冲模板
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	// 渲染目标视图
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	// 深度模板视图
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	// 视口
	D3D11_VIEWPORT m_ScreenViewport;

	/* 键盘输入 */
	// 鼠标
	std::unique_ptr<DirectX::Mouse> m_pMouse; 
	// 鼠标状态追踪器
	DirectX::Mouse::ButtonStateTracker m_MouseTracker;
	// 键盘
	std::unique_ptr<DirectX::Keyboard> m_pKeyBoard;
	// 键盘状态追踪器
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	/* 派生类应该在构造函数设置好的自定义初始参数*/
	// 主窗口标题
	std::wstring m_MainWndCaption; 
	// 视口宽度
	int m_ClientWidth;
	// 视口高度
	int m_ClientHeight;
};

#endif //D3DApp_HH