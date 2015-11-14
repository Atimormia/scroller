#pragma once
#include "liveobject.h"
#include "Graphics.h"

class CPlayerShip :
	public CLiveObject
{
	//! Это текстура со спрайтом корабля
	CTexturePtr m_pTex;
	int m_iScore;

public:
	CPlayerShip(void);
	~CPlayerShip(void);
	//! функция отрисовки
	void Draw();
	void Draw(int x, int y);
	//! Функция движения (управления) корабля
	void Scroll(float dx, float dy);
	//! сброс значений для начала новой игры 
	void NewGame();
	void AddScore(int count);
	int GetScore();
};
