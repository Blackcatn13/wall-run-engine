#include "Camera.h"
#include "Assert.h"
#include "Math\MathUtils.h"
#include "Core_Utils/MemLeaks.h"

CCamera::CCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, ETypeCamera typeCamera)
    : m_pObject3D(object3D)
    ,	m_fFOV(fov)
    ,	m_fAspectRatio(aspect)
    ,	m_fZNear(zn)
    ,	m_fZFar(zf)
    , m_fView_d(2.f)
    , m_eTypeCamera(typeCamera)
{
    //assert(m_pObject3D);
}

CCamera::CCamera()
    : m_pObject3D(0)
    ,	m_fFOV( mathUtils::Deg2Rad(60.f) )
    ,	m_fAspectRatio(4.f / 3.f)
    ,	m_fZNear(0.1f)
    ,	m_fZFar(100.f)
    , m_fView_d(2.f)
    , m_eTypeCamera(TC_THPS)
{}

