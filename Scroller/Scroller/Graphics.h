#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "singleton.h"
#include "Texture.h"
#include "Shader.h"

class Graphics : public singleton<Graphics>
{
	friend class singleton<Graphics>;
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;

	// структура с параметрами девайса
	D3DPRESENT_PARAMETERS m_d3dpp;

	typedef std::vector<CTexturePtr> vecTextures;
	vecTextures m_vecTextures;

	typedef std::vector<CShaderPtr> vecShaders;
	vecShaders m_vecShaders;

protected:
	Graphics(void);
	~Graphics(void);
	LPDIRECT3DDEVICE9 GetDevice();

public:
	bool Init(HWND hRenderWnd, int width, int height);
	void Cleanup();
	HRESULT Reset();
	bool StartRender(D3DCOLOR bgColor=0xff808080);
	
	HRESULT SetFVF(DWORD FVF);
	HRESULT SetPixelShader(CShaderPtr shader);
	HRESULT SetVertexShader(CShaderPtr shader);
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	HRESULT SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
	HRESULT SetTexture(DWORD stage, CTexturePtr pTex);
	HRESULT DPUP( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
	//! Отрисовка спрайта в заданных координатах и с заданными размерами
	HRESULT DrawSprite(CTexturePtr pTex, D3DXVECTOR2 pos, D3DXVECTOR2 size);

	bool EndRender();
	CTexturePtr LoadTexture(const std::wstring& szFileName);
	CShaderPtr LoadShader( const std::string& strFileName, const std::string& strFuncName, const std::string& profile);

	VOID DrawScore(LPCWSTR TextString, int x, int y, int x1, int y1, D3DCOLOR MyColor);

};

struct VertPosDiffuse 
{
	D3DXVECTOR3 m_pos;
	D3DCOLOR m_color;
	VertPosDiffuse(D3DXVECTOR3 pos, D3DCOLOR color)
		: m_pos(pos), m_color(color)
	{}
};

struct VertPosTc 
{
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_tc;
	VertPosTc(D3DXVECTOR3 pos, D3DXVECTOR2 tc)
		: m_pos(pos), m_tc(tc)
	{}
};
