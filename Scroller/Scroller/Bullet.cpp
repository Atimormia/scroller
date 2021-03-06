#include "StdAfx.h"
#include "Bullet.h"

CBullet::CBullet( D3DXVECTOR2 pos, D3DXVECTOR2 speed, const std::wstring& strTexFile)
: m_pos(pos), m_speed(speed), m_iDamage(50)
{
	m_pTex = Graphics::get().LoadTexture(strTexFile);
}

CBullet::~CBullet(void)
{
}

D3DXVECTOR2 CBullet::Pos() const
{
	return m_pos;
}

void CBullet::Update( float fDeltaTime )
{
	m_pos += m_speed*fDeltaTime;
}

bool CBullet::TestCollision( const CLiveObject& obj ) const
{
	// ������ �� ������ ������� �� �������
	D3DXVECTOR2 dist = obj.Pos() - Pos();

	// ���� ���������� ������ ��� ����� ����� ��������
	if (obj.Radius()>=D3DXVec2Length(&dist))
		return true; // ������������

	return false; // ����� ���
}

int CBullet::Damage() const
{
	return m_iDamage;
}

void CBullet::Draw() const
{
	// ������ ������
	D3DXVECTOR2 pos = Pos();
	Graphics::get().SetTexture(0, m_pTex);
	Graphics::get().DrawSprite(m_pTex, pos - D3DXVECTOR2(8, 8)/*���������� ������*/,
		D3DXVECTOR2(16, 32));
}
