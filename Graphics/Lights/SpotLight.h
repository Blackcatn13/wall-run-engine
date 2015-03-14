#ifndef CSPOTLIGHT_H
#define CSPOTLIGHT_H
#include "DirectionalLight.h"
#include "Camera\FPSCamera.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;
//class CFPSCamera;

class CSpotLight : public CDirectionalLight
{
protected:
    float m_Angle;
    float m_FallOff;
    CFPSCamera m_Camera;
public:
    CSpotLight();
    CSpotLight(CXMLTreeNode &Node);
    void SetShadowMap(CGraphicsManager *RM);
    void SetAngle(float Angle)
    {
        m_Angle = Angle;
    }
    float GetAngle() const
    {
        return m_Angle;
    }
    void SetFallOff(const float FallOff)
    {
        m_FallOff = FallOff;
    }
    float GetFallOff() const
    {
        return m_FallOff;
    }
};


#endif
