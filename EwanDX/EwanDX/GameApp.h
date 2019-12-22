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
		float Pad;  // �����֤16�ֽڶ���
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
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush; // ��ɫ��ˢ
	ComPtr<IDWriteFont>m_pFont; // ����
	ComPtr<IDWriteTextFormat> m_pTextFormat; // �ı���ʽ

	ComPtr<ID3D11InputLayout> m_pVertexLayout2D; // 2D�������벼��
	ComPtr<ID3D11InputLayout> m_pVertexLayout3D; // 3D�������벼��
	ComPtr<ID3D11Buffer> m_pVertexBuffer; // ���㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer; // ����������
	ComPtr<ID3D11Buffer>m_pConstantBuffers[2]; // ����������
	UINT m_IndexCount; // ������������������С
	int m_CurrFrame; // ��ǰ֡��
	ShowMode m_CurrMode; // ��ǰģʽ

	ComPtr<ID3D11ShaderResourceView> m_pWoodCrate; // ľ������
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pFireAnims; // ��������
	ComPtr<ID3D11SamplerState> m_pSamplerState; // ������״̬

	ComPtr<ID3D11VertexShader> m_pVertexShader3D; // 3D������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader3D; // 3D������ɫ��
	ComPtr<ID3D11VertexShader> m_pVertexShader2D; // 2D������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader2D; // 2D������ɫ��

	VSConstantBuffer m_VSConstantBuffer;			// �����޸�����VS��GPU�����������ı���
	PSConstantBuffer m_PSConstantBuffer;			// �����޸�����PS��GPU�����������ı���

	DirectionalLight m_DirLight;					// Ĭ�ϻ�����
	PointLight m_PointLight;						// Ĭ�ϵ��
	SpotLight m_SpotLight;						    // Ĭ�ϻ�۹�

	ComPtr<ID3D11RasterizerState> m_pRSWireframe;	// ��դ��״̬: �߿�ģʽ
	bool m_IsWireframeMode;							// ��ǰ�Ƿ�Ϊ�߿�ģʽ

};


#endif

