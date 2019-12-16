#include "d3dUtil.h"


HRESULT CreateShaderFromFile(const WCHAR* csoFileName, const WCHAR * hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{

	HRESULT hr = S_OK;

	// Ѱ���Ƿ��ֱ���õĶ�����ɫ��
	if (csoFileName && D3DReadFileToBlob(csoFileName, ppBlobOut) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// ���� D3DCOMPILE_DEBUG ��־���ڻ�ȡ��ɫ��������Ϣ
		// ����Ȼ������ɫ�������Ż�����
		dwShaderFlag |= D3DCOMPILE_DEBUG;

		// ��Debug�����½�ֹ�Ż��Ա�����ֲ��������
		dwShaderFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG
		ID3DBlob* errorBlob = nullptr;
		hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel
			, dwShaderFlag, 0, ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferSize()));
			}
			SAFE_RELEASE(errorBlob);
			return hr;
		}

		// ���ָ��������ļ���������ɫ����������Ϣ���
		if (csoFileName)
		{
			return D3DWriteBlobToFile(*ppBlobOut, csoFileName, FALSE);
		}
	}

	return hr;
}

