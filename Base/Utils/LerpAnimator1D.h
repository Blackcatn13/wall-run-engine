//-----------------------------------------------------------------------
// CInterpolation class
// ...
//-----------------------------------------------------------------------
#ifndef LERP_ANIMATOR_1D_H
#define LERP_ANIMATOR_1D_H

#include "Utils\Types.h"

typedef enum ETypeFunction { FUNC_CONSTANT, FUNC_INCREMENT, FUNC_DECREMENT};


class CLerpAnimator1D {
 public:
  CLerpAnimator1D ():	m_fElapsedTime(0.f), m_bPause(false), m_fInitValue(0.f),
    m_fEndValue(0.f), m_fTotalTime(0.f), m_eFunction(FUNC_CONSTANT), m_uDegree(2) {}

  virtual ~CLerpAnimator1D () {
    /*NOTHING*/
  }

  void		SetValues			(float initValue, float endValue, float totalTime, ETypeFunction type);
  void		SetDegree			(uint32 degree) {
    m_uDegree = degree;
  }
  bool		Update				(float deltaTime, float &value);
  void		Pause					(bool flag) {
    m_bPause = flag;
  }

 private:
  ETypeFunction	m_eFunction;
  bool					m_bPause;
  float					m_fInitValue;
  float					m_fEndValue;
  float					m_fTotalTime;
  float 				m_fElapsedTime;
  uint32				m_uDegree;
};

#endif //INTERPOLATION_H