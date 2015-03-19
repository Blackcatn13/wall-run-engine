#include "Particles\Particle.h"

CParticle::CParticle(Vect3f Direction1, Vect3f Direction2, float Age, float Speed, CColor Color1, CColor Color2, Vect3f Position, float size)
	: CBillboard(size, Position)
	, m_Direction1(Direction1)
	, m_Direction2(Direction2)
	, m_Age(Age)
	, m_Speed(Speed)
	, m_Color1(Color1)
	, m_Color2(Color2)
	, m_Dead(false)
{
}

void CParticle::Update(float dt)
{
}