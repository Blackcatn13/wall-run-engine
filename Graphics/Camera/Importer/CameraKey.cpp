#include "CameraKey.h"
#include "CameraInfo.h"
#include "Utils\Defines.h"

CCameraKey::CCameraKey(CCameraInfo *CameraInfo, float Time)
{
    m_CameraInfo = CameraInfo;
    m_Time = Time;
}

CCameraKey::~CCameraKey()
{
    CHECKED_DELETE(m_CameraInfo);
}
