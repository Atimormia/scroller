#include "StdAfx.h"
#include "PlayerShip.h"
#include "Graphics.h"

CPlayerShip::CPlayerShip()
: CLiveObject(D3DXVECTOR2(0, 200)/*старт. поз.*/, 150/*нач. жизни*/)
{
	m_pTex = Graphics::get().LoadTexture(L"Data/ship-02.png");
	this->m_iScore = 0;
}

CPlayerShip::~CPlayerShip(void)
{
}

void CPlayerShip::Draw()
{
	// рисуем спрайт
	D3DXVECTOR2 pos = Pos();
	Graphics::get().SetTexture(0, m_pTex);
	Graphics::get().DrawSprite(m_pTex, pos - D3DXVECTOR2(32, 32)/*центрируем спрайт*/,
		D3DXVECTOR2(64, 64));

}

void CPlayerShip::Draw(int x, int y)
{
	Graphics::get().SetTexture(0, m_pTex);
	Graphics::get().DrawSprite(m_pTex, D3DXVECTOR2(x, y)/*центрируем спрайт*/,
		D3DXVECTOR2(16, 16));

}

void CPlayerShip::Scroll( float dx, float dy )
{
	D3DXVECTOR2 pos = Pos();
	pos.x += dx;
	pos.y += dy;
	pos.x = min(max(pos.x, -350), 350);
	pos.y = min(max(pos.y, -250), 250);
	MoveTo( D3DXVECTOR2(pos.x, pos.y) );
}

void CPlayerShip::NewGame()
{
	CLiveObject::HealthUpdate(150);
	CLiveObject::MoveTo(D3DXVECTOR2(0, 200));
	this->m_iScore = 0;
	this->InvDecHealth();
}

int CPlayerShip::GetScore()
{
	return this->m_iScore;
}

void CPlayerShip::AddScore(int count)
{
	this->m_iScore += count;
}
