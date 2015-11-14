#include "StdAfx.h"
#include "StarField.h"

CStarField::CStarField(void)
{
	m_fDT = .0f;
	m_pTex = Graphics::get().LoadTexture(L"Data/star.dds");
}

CStarField::~CStarField(void)
{
}

void CStarField::Update( float fDeltaTime )
{
	// двигаем звёзды
	// чем больше звезда - тем быстрее она движется
	for (size_t i=0; i<vecStars.size(); i++)
	{
		vecStars[i].m_pos.y += vecStars[i].m_size*fDeltaTime*50;
		if (vecStars[i].m_pos.y>300) // если звезда вышла за экран
		{
			// убираем её из массива
			// перемещаем последнюю в массиве на место этой
			vecStars[i] = vecStars[vecStars.size()-1];
			// и уменьшаем размер массива
			vecStars.pop_back();
		}
	}
	
	// генерируем новые звёзды, если прошло больше,
	// чем 0.1 сек
	m_fDT += fDeltaTime;
	while(m_fDT>.1f)
	{
		vecStars.push_back(Star(D3DXVECTOR2( (rand()%800)-400, -300), (rand()%5)+1));
		m_fDT-=.1f;
	}
}

void CStarField::Draw()
{
	// устанвливаем текстуру звёзды
	Graphics::get().SetTexture(0, m_pTex);
	// рисуем всё звёзды по очереди
	for (size_t i=0; i<vecStars.size(); i++)
	{
		D3DXVECTOR2 pos = vecStars[i].m_pos;
		float size = float(vecStars[i].m_size);
		Graphics::get().DrawSprite(m_pTex, pos, D3DXVECTOR2(size, size));
	}
}

void CStarField::Release()
{
	vecStars.clear();
}

CStarField::Star::Star( D3DXVECTOR2 pos, int size )
: m_pos(pos), m_size(size)
{
}
