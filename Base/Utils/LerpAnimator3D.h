//-----------------------------------------------------------------------
// CInterpolation class
// ...
//-----------------------------------------------------------------------
#ifndef LERP_ANIMATOR_3D_H
#define LERP_ANIMATOR_3D_H

#include "Math/Vector3.h"

typedef enum ETypeFunction { FUNC_CONSTANT, FUNC_INCREMENT, FUNC_DECREMENT};


class CLerpAnimator3D
{
public:
    CLerpAnimator3D ():	m_fElapsedTime(0.f), m_bPause(false), m_vInitValue(v3fZERO),
        m_vEndValue(v3fZERO), m_fTotalTime(0.f), m_eFunction(FUNC_CONSTANT), m_uDegree(2) {}

    virtual ~CLerpAnimator3D ()
    {
        /*NOTHING*/
    }

    void		SetValues			(Vect3f initValue, Vect3f endValue, float totalTime, ETypeFunction type);
    void		SetDegree			(uint32 degree)
    {
        m_uDegree = degree;
    }
    bool		Update				(float deltaTime, Vect3f &value);
    void		Pause					(bool flag)
    {
        m_bPause = flag;
    }

private:
    ETypeFunction	m_eFunction;
    bool				m_bPause;
    Vect3f			m_vInitValue;
    Vect3f			m_vEndValue;
    float				m_fTotalTime;
    float 			m_fElapsedTime;
    uint32			m_uDegree;
};

#endif //INTERPOLATION_H