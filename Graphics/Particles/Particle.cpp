#include "Particles\Particle.h"

CParticle::CParticle(Vect3f Direction1, Vect3f Direction2, float Age, float Speed, CColor Color1, CColor Color2, Vect3f Position, float size)
  : CBillboard(size, Position)
  , m_Direction1(Direction1)
  , m_Direction2(Direction2)
  , m_Age(Age)
  , m_Speed(Speed)
  , m_Color2(Color2)
  , m_Dead(false) {
  m_Color1 = Color1;
}

CParticle::CParticle()
  : CBillboard(1, Vect3f(0, 0, 0))
  , m_Direction1(Vect3f(0, 1, 0))
  , m_Direction2(Vect3f(0, -1, 0))
  , m_Age(5)
  , m_Speed(2)
  , m_Color2(CColor(colWHITE))
  , m_Dead(false) {
  m_Color1 = colBLACK;
}

CParticle::~CParticle() {}

void CParticle::Update(float dt) {
  //Gravedad de XML
  //VelocidadOndulacion Maxima y minima
  //VectorOndulacion Maximo y minimo
  Vect3f l_SumaOndulacion = m_VectorOndulacion * sin(dt*m_VelocidadOndulacion+m_InicioOndulacion);
  Vect3f l_DirGravity = Vect3f(0,-1,0);
  m_Direction1 = m_Direction1.Normalize() * m_Speed + l_DirGravity * m_Gravity * dt;
  m_position += m_Direction1.Normalize() * m_Speed * dt + l_SumaOndulacion * dt;
  //m_position += m_Direction1.Normalize() * m_Speed * dt + Vect3f(0.0,-1.0,0.0) * m_Gravity * dt*dt;
  
}

float CParticle::takeLife(float dt) {
  m_Age -= dt;
  if (m_Age <= 0)
    m_Dead = true;
  return m_Age;
}