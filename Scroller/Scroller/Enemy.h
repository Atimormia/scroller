#pragma once
#include "liveobject.h"
#include "Graphics.h"

class CEnemy :
	public CLiveObject
{
	//! Это текстура со спрайтом врага
	CTexturePtr m_pTex;

	//! Таймер стрельбы. Когда он меньше нуля можно стрелять.
	float fFireTimeOut;

public:
	CEnemy(void);
	virtual ~CEnemy(void);
	//! функция отрисовки
	void Draw();
	//! Функция апдейта
	void Update(float fDeltaTime);
};
