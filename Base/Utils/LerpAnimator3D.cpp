
#include "LerpAnimator3D.h"
#include <cmath>
#include <assert.h>

void CLerpAnimator3D::SetValues (Vect3f initValue, Vect3f endValue, float totalTime, ETypeFunction type) {
  assert( totalTime > 0);
  m_vInitValue		= initValue;
  m_vEndValue			= endValue;
  m_fTotalTime		= totalTime;
  m_fElapsedTime	= 0.f;
  m_eFunction			= type;
}


bool CLerpAnimator3D::Update (float deltaTime, Vect3f &value) {
  m_fElapsedTime += deltaTime;
  bool finish = false;
  if (m_fElapsedTime >= m_fTotalTime) {
    finish = true;
    m_fElapsedTime = m_fTotalTime;
  }
  //mu [0-1]
  float mu = m_fElapsedTime / m_fTotalTime;
  //En funcion del tiempo la siguiente funcion nos retorna un valor entre 0-1.
  switch (m_eFunction) {
    case FUNC_CONSTANT: {
      //Linear
      //nothing to do
    }
    break;
    case FUNC_INCREMENT: {
      mu = mathUtils::PowN(mu, m_uDegree);
    }
    break;
    case FUNC_DECREMENT: {
      mu = pow(mu, (float)(1 / m_uDegree));
    }
    break;
  }
  value = m_vInitValue.GetLerp(m_vEndValue, mu);
  return finish;
}