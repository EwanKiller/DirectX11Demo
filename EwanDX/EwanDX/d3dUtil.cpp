#include "d3dUtil.h"


HRESULT CreateShaderFromFile(const WCHAR* csoFileName, const WCHAR * hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{

	HRESULT hr = S_OK;

	// 寻找是否又编译好的顶点着色器
	if (csoFileName && D3DReadFileToBlob(csoFileName, ppBlobOut) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息
		// 但仍然允许着色器进行优化操作
		dwShaderFlag |= D3DCOMPILE_DEBUG;

		// 在Debug环境下禁止优化以避免出现不合理情况
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

		// 如果指定了输出文件名，则将着色器二进制信息输出
		if (csoFileName)
		{
			return D3DWriteBlobToFile(*ppBlobOut, csoFileName, FALSE);
		}
	}

	return hr;
}

