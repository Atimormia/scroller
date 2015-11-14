#pragma once
#include <d3d9.h>
#include <string>

class Graphics;

class CTexture
{
	// класс Graphics будет иметь доступ к protected-членам
	friend class Graphics;

private:
	//! файл текстуры
	std::wstring m_strTexSrc;
	//! это сама текстура
	LPDIRECT3DTEXTURE9 m_pTex;

protected:
	// доступ только для класса Graphics
	CTexture(void);
	~CTexture(void);
	HRESULT Release();
	void SetTexture(LPDIRECT3DTEXTURE9 pTex);
	void SetSrc(const std::wstring& src);
	LPDIRECT3DTEXTURE9 GetTexture() const;
	HRESULT Reload(LPDIRECT3DDEVICE9 pDev);

public:
	std::wstring GetSrc() const;
};

typedef CTexture* CTexturePtr; // тип указателей на текстуру