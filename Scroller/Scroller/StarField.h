#pragma once
#include "Graphics.h"

class CStarField
{
	//! �������� �� �������� ������
	CTexturePtr m_pTex;

	//! ������� ������� ��� �������� ����� ����
	float m_fDT;

	struct Star // ���� ������
	{
		D3DXVECTOR2 m_pos; // ������� �� ������
		int m_size; // ������
		Star(D3DXVECTOR2 pos=D3DXVECTOR2(0,0), int size=8);
	};
	std::vector<Star> vecStars;
public:
	CStarField(void);
	~CStarField(void);
	void Update(float fDeltaTime);
	void Draw();
	void Release();
};
