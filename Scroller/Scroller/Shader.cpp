#include "StdAfx.h"
#include "Shader.h"
#include "Graphics.h"

std::string GetFileAsString(const std::string& strFileName);

CShader::CShader(void)
: m_bIsPS(true),
m_pErrors(NULL),
m_pShaderBuff(NULL),
m_pConstTable(NULL)
{
	m_pShader.m_pPixelShader = NULL;
	m_pShader.m_pVertexShader = NULL;

}

CShader::~CShader(void)
{
	Release();
}

HRESULT CShader::Release()
{
	if (m_pConstTable)
		m_pConstTable->Release();
	m_pConstTable = NULL;

	if (m_pShaderBuff)
		m_pShaderBuff->Release();
	m_pShaderBuff = NULL;

	if (m_pErrors)
		m_pErrors->Release();
	m_pErrors = NULL;

	return D3D_OK;
}

HRESULT CShader::Load( const std::string& strFileName, const std::string& strFuncName, const std::string& profile)
{
	// загружаем шейдер из файла
	std::string src = GetFileAsString(strFileName);

	// компилируем шейдер
	HRESULT hr = D3DXCompileShader(src.c_str(), src.length(), NULL, NULL, strFuncName.c_str(), profile.c_str(), D3DXSHADER_OPTIMIZATION_LEVEL3, &m_pShaderBuff, &m_pErrors, &m_pConstTable);
	if (FAILED(hr))
		return hr;

	if (profile[0]=='p')
		m_bIsPS = true;
	else
		m_bIsPS = false;

	return D3D_OK;
}

void CShader::SetHardwareShader( LPDIRECT3DDEVICE9 pDev, LPDIRECT3DPIXELSHADER9 pHWShader )
{
	m_pShaderDevice = pDev;
	m_pShader.m_pPixelShader = pHWShader;
}

void CShader::SetHardwareShader( LPDIRECT3DDEVICE9 pDev, LPDIRECT3DVERTEXSHADER9 pHWShader )
{
	m_pShaderDevice = pDev;
	m_pShader.m_pVertexShader = pHWShader;
}


HRESULT CShader::SetFloat( D3DXHANDLE hConstant, CONST FLOAT Value)
{
	return m_pConstTable->SetFloat(m_pShaderDevice, hConstant, Value);
}

HRESULT CShader::SetVector( D3DXHANDLE hConstant, CONST D3DXVECTOR4* pVec4 )
{
	return m_pConstTable->SetVector(m_pShaderDevice, hConstant, pVec4);
}

HRESULT CShader::SetMatrix( D3DXHANDLE hConstant, CONST D3DXMATRIX* pMatrix )
{
	return m_pConstTable->SetMatrix(m_pShaderDevice, hConstant, pMatrix);
}

LPDIRECT3DPIXELSHADER9 CShader::GetPixelShader()
{
	return m_pShader.m_pPixelShader;
}

LPDIRECT3DVERTEXSHADER9 CShader::GetVertexShader()
{
	return m_pShader.m_pVertexShader;
}

bool CShader::IsPS()
{
	return m_bIsPS;
}

bool CShader::IsVS()
{
	return !m_bIsPS;
}

LPD3DXBUFFER CShader::GetCompileBuffer()
{
	return m_pShaderBuff;
}
