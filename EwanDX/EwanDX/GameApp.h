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

	// ����������
	struct Material 
	{
		Material() { memset(this, 0, sizeof(Material)); }
		
		DirectX::XMFLOAT4 Ambient;  // ������
		DirectX::XMFLOAT4 Diffuse;  // �����
		DirectX::XMFLOAT4 Specular; // w = ���淴��ǿ��
		DirectX::XMFLOAT4 Reflect;  // ����ǿ��
	};

	// �����
	struct DirectionalLight
	{
		DirectionalLight() { memset(this, 0, sizeof(DirectionalLight)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT3 Direction;
		float Pad;  //��һ�����������ʹ�ṹ���С����16������������HLSL��������
	};

	//���Դ
	struct PointLight
	{
		PointLight() { memset(this, 0, sizeof(PointLight)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		// �����4D���� (Position,Range)
		DirectX::XMFLOAT3 Position;
		float Range;

		// �����4D���� ��A0,A1,A2,Pad��
		DirectX::XMFLOAT3 Att; // Attenuation
		float Pad;
	};

	// �۹��
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
	ComPtr<ID3D11InputLayout> m_pVertexLayout; // �������벼��
	ComPtr<ID3D11Buffer> m_pVertexBuffer; // ���㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer; // ����������
	ComPtr<ID3D11Buffer>m_pConstantBuffer; // ����������

	ComPtr<ID3D11VertexShader> m_pVertexShader; // ������ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader; //������ɫ��

	ConstantBuffer m_CBuffer; // �����޸�GPU�����������ı���

};


#endif

