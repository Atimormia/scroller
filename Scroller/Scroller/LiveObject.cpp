#include "StdAfx.h"
#include "LiveObject.h"

CLiveObject::CLiveObject(D3DXVECTOR2 pos, int iHealth)
: m_vPos(pos),
m_iHealth(iHealth),
m_fRadius(28),
m_bDecHealth(false)
{
}

CLiveObject::~CLiveObject(void)
{
}

void CLiveObject::DoDamage( const int iDamage )
{
	m_iHealth -= iDamage; // уменьшаем жизни
}

bool CLiveObject::IsAlive() const
{
	return ( m_iHealth > 0 ); // жизней больше нуля?
}

bool CLiveObject::IsDecHealth() const
{
	return this->m_bDecHealth;
}

bool CLiveObject::InvDecHealth()
{
	this->m_bDecHealth = !this->m_bDecHealth;
	return this->m_bDecHealth;
}

void CLiveObject::Move( float dx, float dy )
{
	Move(D3DXVECTOR2(dx, dy));
}

void CLiveObject::Move( const D3DXVECTOR2 delta )
{
	m_vPos += delta;
}

void CLiveObject::MoveTo( const D3DXVECTOR2 pos )
{
	m_vPos = pos;
}

void CLiveObject::HealthUpdate( const int p )
{
	m_iHealth += p;
}

D3DXVECTOR2 CLiveObject::Pos() const
{
	return m_vPos;
}

float CLiveObject::Radius() const
{
	return m_fRadius;
}

bool CLiveObject::TestCollision( const CLiveObject& obj ) const
{
	 // вектор от одного объекта до другого
	D3DXVECTOR2 dist = obj.Pos()-Pos();

	// если расстояние меньше или равно сумме радиусов
	if (obj.Radius()+Radius()>=D3DXVec2Length(&dist))
		return true; // столкновение
	
	return false; // иначе нет
}

int CLiveObject::GetHealth()
{
	return this->m_iHealth;
}
