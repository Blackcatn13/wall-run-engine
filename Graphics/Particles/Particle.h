#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Math\Vector3.h"
#include "Particles\Billboard.h"
#include "Math\Color.h"

class CGraphicsManager;

class CParticle : public CBillboard {
 public:
  CParticle(Vect3f Direction1, Vect3f Direction2, float Age, float Speed, CColor Color1, CColor Color2, Vect3f Position, float size);
  CParticle();
  ~CParticle();
  void Update(float dt);
  bool isDead() { return m_Dead; }
  float takeLife(float dt);
  void setDirection1(Vect3f Direction) { m_Direction1 = Direction; }
  void setDirection2(Vect3f Direction) { m_Direction2 = Direction; }
  void setAge(float Age) { m_Age = Age; }
  void setSpeed(float Speed) { m_Speed = Speed; }
  void setColor1(CColor Color) { m_Color1 = Color; }
  void setColor2(CColor Color) { m_Color2 = Color; }
  void setSize(float size) { m_size = size; }
  void setPosition(Vect3f Position) { m_position = Position; }
 private:
  Vect3f	m_Direction1;
  Vect3f	m_Direction2;
  float	  m_Age;
  float	  m_Speed;
  CColor	m_Color1;
  CColor	m_Color2;
  bool    m_Dead;
};

#endif // PARTICLE_H_