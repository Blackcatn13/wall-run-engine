#include <assert.h>
#include "LerpAnimator1D.h"
#include "Math\MathUtils.h"

void CLerpAnimator1D::SetValues (float initValue, float endValue, float totalTime, ETypeFunction type)
{
    assert( totalTime > 0);
    m_fInitValue = initValue;
    m_fEndValue = endValue;
    m_fTotalTime = totalTime;
    m_fElapsedTime = 0.f;
    m_eFunction = type;
}


bool CLerpAnimator1D::Update (float deltaTime, float &value)
{
    m_fElapsedTime += deltaTime;
    bool finish = false;
    if (m_fElapsedTime >= m_fTotalTime) {
        finish = true;
        m_fElapsedTime = m_fTotalTime;
    }
    //mu [0-1]
    float mu = 0.f;
    mu = m_fElapsedTime / m_fTotalTime;
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
            mu = sqrt(mu);
        }
        break;
    }
    float progress = mathUtils::Lerp(m_fInitValue, m_fEndValue, mu);
    if (m_fEndValue > m_fInitValue) {
        value = m_fInitValue + progress;
    } else {
        value = m_fEndValue + progress;
    }
    return finish;
}