#include "StdAfx.h"
#include "Enemy.h"

// форвард декларация функции выстрела врага
void FireEnemy(D3DXVECTOR2 pos, D3DXVECTOR2 speed);

CEnemy::CEnemy(void)
: CLiveObject(D3DXVECTOR2((rand()%700)-400.0f, -400.0f)/*старт. поз.*/, 50/*нач. жизни*/)
{
	m_pTex = Graphics::get().LoadTexture(L"Data/enemy-01.png");
}

CEnemy::~CEnemy(void)
{
}


void CEnemy::Draw()
{
	// рисуем спрайт
	D3DXVECTOR2 pos = Pos();
	Graphics::get().SetTexture(0, m_pTex);
	Graphics::get().DrawSprite(m_pTex, pos - D3DXVECTOR2(32, 32)/*центрируем спрайт*/,
		D3DXVECTOR2(64, 64));
}

void CEnemy::Update( float fDeltaTime )
{
	Move(0, 100*fDeltaTime);
	
	// стрельба
	fFireTimeOut -= fDeltaTime;
	if (fFireTimeOut<0 && (rand()&127)==127)
	{
		FireEnemy(Pos(), D3DXVECTOR2(0, 200));
		fFireTimeOut = 1.0f; // не сможем стрелять ещё секунду
	}
}
