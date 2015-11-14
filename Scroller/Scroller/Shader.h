#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class Graphics;

class CShader
{
	friend class Graphics;

private:
	//! Это пиксельный шейдер?
	bool m_bIsPS;
	//! Здесь ошибки омпиляции
	LPD3DXBUFFER m_pErrors;
	//! Здесь скопилированый шейдер
	LPD3DXBUFFER m_pShaderBuff;
	//! Таблица констант шейдера
	LPD3DXCONSTANTTABLE m_pConstTable;
	//! вертексный и писельный шейдер. экономим память
	union
	{
		LPDIRECT3DPIXELSHADER9 m_pPixelShader;
		LPDIRECT3DVERTEXSHADER9 m_pVertexShader;

	} m_pShader;

	//! Девайс, в котором создан шейдер
	LPDIRECT3DDEVICE9 m_pShaderDevice;

protected:
	//! Функция загрузки и компиляции шейдера
	HRESULT Load( const std::string& strFileName, const std::string& strFuncName, const std::string& profile);
	HRESULT Release();
	//! Получение пиксельного шейдера
	LPDIRECT3DPIXELSHADER9 GetPixelShader();
	//! Получение вертексного шейдера
	LPDIRECT3DVERTEXSHADER9 GetVertexShader();
	//! Получение буфера скомпилированого шейдера (нужно для создания HW-шейдера)
	LPD3DXBUFFER GetCompileBuffer();
	//! Установка HW-шейдера (пиксельного)
	void SetHardwareShader(LPDIRECT3DDEVICE9 pDev, LPDIRECT3DPIXELSHADER9 pHWShader);
	//! Установка HW-шейдера (вертексного)
	void SetHardwareShader(LPDIRECT3DDEVICE9 pDev, LPDIRECT3DVERTEXSHADER9 pHWShader);
	CShader(void);
	~CShader(void);

public:
	bool IsPS(); // это пиксельный шейдер?
	bool IsVS(); // или это вертексный шейдер?
	//! Функция задания матрицы
	HRESULT SetFloat(D3DXHANDLE hConstant, CONST FLOAT Value);
	HRESULT SetVector(D3DXHANDLE hConstant, CONST D3DXVECTOR4* pVec4);
	HRESULT SetMatrix(D3DXHANDLE hConstant, CONST D3DXMATRIX* pMatrix);
};

typedef CShader* CShaderPtr; // тип указателей на текстуру