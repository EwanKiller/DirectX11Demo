// D3Dʵ�ù��߼�

#ifndef D3DUTIL_H
#define D3DUTIL_H

#include<d3d11_1.h> // �Ѱ���Windows.h
#include<DirectXCollision.h> // �Ѱ���DirectXMath.h
#include<DirectXPackedVector.h>
#include<DirectXColors.h>
#include<d3dcompiler.h>
#include<vector>
#include<string>

// �����

// Ĭ�Ͽ���ͼ�ε�����������
// �������Ҫ�ù��ܣ���ͨ��ȫ���ı��滻����ֵ����Ϊ0
#ifndef GRAPHICS_DEBUGGER_OBJECT_NAME
#define GRAPHICS_DEBUGGER_OBJECT_NAME (1)
#endif 

// ��ȫCOM����ͷź�
#define SAFE_RELEASE(p){if((p)){(p)->Release();(p)=nullptr;}}

// D3D11SetDebugObjectName����
//----------------------------
// ΪD3D�豸���������Ķ�����ͼ�ε����������ö�����
// [In] ID3D11DeviceChild D3D11�豸�������Ķ���
// [In] name ������
template<UINT TNameLength>
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// DXGISetDebugObjectName����
// ΪDXGI������ͼ�ε����������ö�����
// [In]IDXGIObject DXGI����
// [In]name ������
template<UINT TNameLength>
inline void DXGISetDebugObjectName(_In_ IDXGIObject* resource, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG))&&(GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// ��ɫ��������غ���
//
/*
	CreateShaderFromFile()
-----------------------------
	[csoFileName] ����õ���ɫ���������ļ���.cso)
	[hlslFileName] ��ɫ������
	[entryPoint] ��ڵ�
	[shaderModel] ��ɫ��ģ�� ��ʽΪ"*s_5_0" * ����Ϊc,b,g,h,p,v ֮һ
	[out-ppBlobOut] �����ɫ���������ļ�
*/
HRESULT CreateShaderFromFile(
	const WCHAR* csoFileName,
	const WCHAR	* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut
);
#endif // !D3DUTIL_H


