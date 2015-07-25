#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Math\Vector3.h"
#include "Particles\Billboard.h"
#include "Math\Color.h"

class CGraphicsManager;

class CParticle : public CBillboard {
 public:
  CParticle(Vect3f Direction1, Vect3f Direction2, float Age, float Speed, CColor Color1, CColor Color2, Vect3f Position, float size, float endSize);
  CParticle();
  ~CParticle();
  void Update(float dt);
  bool isDead() { return m_Dead; }
  float takeLife(float dt);
  void setDirection1(Vect3f Direction) { m_Direction1 = Direction; }
  void setDirection2(Vect3f Direction) { m_Direction2 = Direction; }
  void setAge(float Age) { m_Age = Age; m_StartAge = Age; }
  void setSpeed(float Speed) { m_Speed = Speed; }
  void setColor1(CColor Color) { m_Color1 = Color; }
  void setColor2(CColor Color) { m_Color2 = Color; }
  void setSize(float size) { m_size = size; m_StartSize = size; }
  void setPosition(Vect3f Position) { m_position = Position; }
  void setGravity(float Gravity) { m_Gravity = Gravity; }
  void setVectorOndulacion(Vect3f VectorOndulacion) { m_VectorOndulacion = VectorOndulacion; }
  void setVelocidadOndulacion(float VelocidadOndulacion) { m_VelocidadOndulacion = VelocidadOndulacion; }
  void setInicioOndulacion(float InicioOndulacion) { m_InicioOndulacion = InicioOndulacion; }
  void setEndSize(float endSize) {m_EndSize = endSize;}
  void setEndAlpha(float endAlpha) {m_EndAlpha = endAlpha;}
  void setRotation(float rotation) {m_Rotation = rotation;}
  float getCurrentRotation() {return m_currentRotation;}
  float getAlpha();
 private:
  Vect3f	m_Direction1;
  Vect3f	m_Direction2;
  float	  m_Age;
  float   m_StartAge;
  float	  m_Speed;
  CColor	m_Color2;
  bool    m_Dead;
  float		m_Gravity;
  Vect3f	m_VectorOndulacion;
  float		m_VelocidadOndulacion;
  float		m_InicioOndulacion;
  float   m_EndSize;
  float   m_StartSize;
  float   m_EndAlpha;
  float   m_Rotation;
  float   m_currentRotation;
};

#endif // PARTICLE_H_