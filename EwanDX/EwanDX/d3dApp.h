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
	// ��ȡӦ��ʵ���ľ��
	HINSTANCE AppInst() const;
	// ��ȡ�����ھ��
	HWND MainWnd() const;
	// ��ȡ��Ļ��߱�
	float AspectRatio() const;
	// ��Ϸ��ѭ��
	int Run();
	
	/*
	��ܷ�������������Ҫ�������º���
	*/
	// ��ʼ�����ں�Direct3D
	virtual bool Init();
	// ��Ҫ�ڴ��ڴ�С�䶯��ʱ�����
	virtual void OnResize();
	// ���ÿһ֡�ĸ���
	virtual void UpdateScene(float dt) = 0;
	// ���ÿһ֡�Ļ���
	virtual void DrawScene() = 0;
	// ��Ϣ����
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	/* ���ڵ���Ϣ�ص����� */
protected:
	// ���ڳ�ʼ��
	bool InitMainWindow();
	// Direct2D��ʼ��
	bool InitDirect2D();
	// Direct3D��ʼ��
	bool InitDirect3D();
	// ����ÿ��֡�������ڴ�������ʾ
	void CalculateFrameStates();

protected:
	// ��ȡʵ�����
	HINSTANCE m_hAppInst;
	// �����ھ��
	HWND m_hMainWnd;
	// Ӧ���Ƿ���ͣ
	bool m_AppPaused;
	// Ӧ���Ƿ���С��
	bool m_Minimized;
	// Ӧ���Ƿ����
	bool m_Maximized;
	// ���ڴ�С�Ƿ�仯
	bool m_Resizing;
	// �Ƿ���4�����ز���
	bool m_Enable4xMsaa;
	// MSAA֧�ֵ������ȼ�
	UINT m_4xMsaaQuality;

	// ��ʱ��
	GameTimer m_Timer;

	// ʹ��ģ���������
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	/* Direct2D */
	// D2D����
	ComPtr<ID2D1Factory> m_pD2DFactory; 
	// D2D��ȾĿ��
	ComPtr<ID2D1RenderTarget> m_pD2DRenderTarget;
	// DWrite����
	ComPtr<IDWriteFactory> m_pDWriteFactory;

	/* Direct3D 11 */
	// D3D11�豸
	ComPtr<ID3D11Device> m_pD3DDevice;
	// D3D�豸������
	ComPtr<ID3D11DeviceContext> m_pD3DImmediateContext;
	// D3D������
	ComPtr<IDXGISwapChain> m_pSwapChain;

	/* Direct3D 11.1 */
	// D3D11.1�豸
	ComPtr<ID3D11Device1> m_pD3DDevice1;
	// D3D11.1�豸������
	ComPtr<ID3D11DeviceContext1> m_pD3DImmediateContext1;
	// D3D11.1������
	ComPtr<IDXGISwapChain1> m_pSwapChain1;

	/* ������Դ*/
	// ��Ȼ���ģ��
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	// ��ȾĿ����ͼ
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	// ���ģ����ͼ
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	// �ӿ�
	D3D11_VIEWPORT m_ScreenViewport;

	/* �������� */
	// ���
	std::unique_ptr<DirectX::Mouse> m_pMouse; 
	// ���״̬׷����
	DirectX::Mouse::ButtonStateTracker m_MouseTracker;
	// ����
	std::unique_ptr<DirectX::Keyboard> m_pKeyBoard;
	// ����״̬׷����
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	/* ������Ӧ���ڹ��캯�����úõ��Զ����ʼ����*/
	// �����ڱ���
	std::wstring m_MainWndCaption; 
	// �ӿڿ��
	int m_ClientWidth;
	// �ӿڸ߶�
	int m_ClientHeight;
};

#endif //D3DApp_HH