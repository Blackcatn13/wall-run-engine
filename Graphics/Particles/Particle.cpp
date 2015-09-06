#include "Particles\Particle.h"
#include "Math\MathUtils.h"

CParticle::CParticle(Vect3f Direction1, Vect3f Direction2, float Age, float Speed, CColor Color1, CColor Color2, Vect3f Position, float size, float endSize)
  : CBillboard(size, Position)
  , m_Direction1(Direction1)
  , m_Direction2(Direction2)
  , m_Age(Age)
  , m_StartAge(Age)
  , m_Speed(Speed)
  , m_Color2(Color2)
  , m_Dead(false)
  , m_StartSize(size)
  , m_EndAlpha(1.f)
  , m_Rotation(0.f)
  , m_currentRotation(0.f)
  , m_UpdateType(NORMAL)
  , m_EndSize(endSize) {
  m_Color1 = Color1;

}

CParticle::CParticle()
  : CBillboard(1, Vect3f(0, 0, 0))
  , m_Direction1(Vect3f(0, 1, 0))
  , m_Direction2(Vect3f(0, -1, 0))
  , m_Age(5.f)
  , m_StartAge(5.f)
  , m_Speed(2.f)
  , m_EndAlpha(1.f)
  , m_Rotation(0.f)
  , m_currentRotation(0.f)
  , m_StartSize(1.f)
  , m_Color2(CColor(colWHITE))
  , m_UpdateType(NORMAL)
  , m_Dead(false) {
  m_Color1 = colBLACK;
}

CParticle::~CParticle() {}

void CParticle::Update(float dt) {
  //Gravedad de XML
  //VelocidadOndulacion Maxima y minima
  //VectorOndulacion Maximo y minimo
  if (m_UpdateType == CONE) {
    float h = m_StartAge - m_Age;
    m_position = m_Direction2 + Vect3f(m_Direction1.z * h * mathUtils::Cos(m_Direction1.x * h), h, m_Direction1.z * h * mathUtils::Sin(m_Direction1.x * h));
  } else {
    Vect3f l_SumaOndulacion = m_VectorOndulacion * sin(dt * m_VelocidadOndulacion + m_InicioOndulacion);
    Vect3f l_DirGravity = Vect3f(0, -1, 0);
    m_Direction1 = m_Direction1 * m_Speed + l_DirGravity * m_Gravity * dt;
    m_Direction1.Normalize();
    m_position += m_Direction1 * m_Speed * dt + l_SumaOndulacion * dt;
    m_size = (1 - m_Age) * m_EndSize + m_Age * m_StartSize;
    m_currentRotation += m_Rotation * dt;
  }
  //m_position += m_Direction1.Normalize() * m_Speed * dt + Vect3f(0.0,-1.0,0.0) * m_Gravity * dt*dt;

}

float CParticle::takeLife(float dt) {
  m_Age -= dt;
  if (m_Age <= 0)
    m_Dead = true;
  return m_Age;
}

float CParticle::getAlpha() {
  return (1 - m_Age) * m_EndAlpha + m_Age;
}