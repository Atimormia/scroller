#pragma once
#include "Graphics.h"
#include "LiveObject.h"

class CBullet
{
	int m_iDamage; // урон
	D3DXVECTOR2 m_pos; // текущая позиция
	D3DXVECTOR2 m_speed; // текущая скорость

	//! Текстура со спрайтом снаряда
	CTexturePtr m_pTex;

public:
	CBullet(D3DXVECTOR2 pos=D3DXVECTOR2(0,0),
		D3DXVECTOR2 speed=D3DXVECTOR2(0,0), const std::wstring& strTexFile=L"Data/bullet.dds");
	virtual ~CBullet(void);
	D3DXVECTOR2 Pos() const;
	void Draw() const;
	int Damage() const;
	void Update(float fDeltaTime); // апдейт снаряда
	bool TestCollision(const CLiveObject& obj) const;
};
