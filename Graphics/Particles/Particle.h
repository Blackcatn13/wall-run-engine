#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Math\Vector3.h"
#include "Particles\Billboard.h"
#include "Math\Color.h"

class CGraphicsManager;

class CParticle : public CBillboard {
public:
	CParticle(Vect3f Direction1, Vect3f Direction2, float Age, float Speed, CColor Color1, CColor Color2, Vect3f Position, float size);
	~CParticle();
	void Update(float dt);
	bool isDead() {return m_Dead; }
private:
	Vect3f	m_Direction1;
	Vect3f	m_Direction2;
	float	m_Age;
	float	m_Speed;
	CColor	m_Color1;
	CColor	m_Color2;
	bool    m_Dead;
};

#endif // PARTICLE_H_