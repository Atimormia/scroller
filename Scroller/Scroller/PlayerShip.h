#pragma once
#include "liveobject.h"
#include "Graphics.h"

class CPlayerShip :
	public CLiveObject
{
	//! ��� �������� �� �������� �������
	CTexturePtr m_pTex;
	int m_iScore;

public:
	CPlayerShip(void);
	~CPlayerShip(void);
	//! ������� ���������
	void Draw();
	void Draw(int x, int y);
	//! ������� �������� (����������) �������
	void Scroll(float dx, float dy);
	//! ����� �������� ��� ������ ����� ���� 
	void NewGame();
	void AddScore(int count);
	int GetScore();
};
