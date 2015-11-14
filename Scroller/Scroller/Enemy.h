#pragma once
#include "liveobject.h"
#include "Graphics.h"

class CEnemy :
	public CLiveObject
{
	//! ��� �������� �� �������� �����
	CTexturePtr m_pTex;

	//! ������ ��������. ����� �� ������ ���� ����� ��������.
	float fFireTimeOut;

public:
	CEnemy(void);
	virtual ~CEnemy(void);
	//! ������� ���������
	void Draw();
	//! ������� �������
	void Update(float fDeltaTime);
};
