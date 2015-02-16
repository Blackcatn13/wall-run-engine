#ifndef CLIGHT_H
#define CLIGHT_H

#include "Object\Object3D.h"
#include "Math\Color.h"
#include <string>
#include <vector>
#include "Utils\Defines.h"
#include <d3dx9.h>

class CTexture;
class CRenderableObjectsManager;
class CEffect;
class CTexture;
class CGraphicsManager;

class CLight : public CObject3D
{
public:
    enum TLightType {
        OMNI = 0,
        DIRECTIONAL,
        SPOT
    };
protected:
    CColor m_Color;
    TLightType m_Type;
    std::string m_Name;
    bool m_RenderShadows;
    float m_StartRangeAttenuation;
    float m_EndRangeAttenuation;
    float m_Intensity;
    static TLightType GetLightTypeByName(const std::string &StrLightType);

    /**ADVANCED SHADERS**/
    int m_ShadowMapWidth, m_ShadowMapHeigth;
    bool m_GenerateDynamicShadowMap;
    bool m_GenerateStaticShadowMap;
    bool m_MustUpdateStaticShadowMap;
    CTexture *m_StaticShadowMap, *m_DynamicShadowMap, *m_ShadowMaskTexture;
    std::vector<CRenderableObjectsManager *> m_StaticShadowMapRenderableObjectsManagers,
        m_DynamicShadowMapRenderableObjectsManagers;
    Mat44f m_ViewShadowMap, m_ProjectionShadowMap;
    std::string m_FormatType;
public:
    CLight();
    virtual ~CLight();
    void Init();

    void SetName(const std::string &Name)
    {
        m_Name = Name;
    }
    const std::string &GetName()
    {
        return m_Name;
    }
    void SetColor(const CColor &Color)
    {
        m_Color = Color;
    }
    const CColor & GetColor() const
    {
        return m_Color;
    }
    void SetStartRangeAttenuation(const float StartRangeAttenuation)
    {
        m_StartRangeAttenuation = StartRangeAttenuation;
    }
    float GetStartRangeAttenuation() const
    {
        return m_StartRangeAttenuation;
    }
    void SetEndRangeAttenuation(const float EndRangeAttenuation)
    {
        m_EndRangeAttenuation = EndRangeAttenuation;
    }
    float GetEndRangeAttenuation() const
    {
        return m_EndRangeAttenuation;
    }

    void SetIntensity(const float Intensity)
    {
        m_Intensity = Intensity;
    }
    float GetIntensity() const
    {
        return m_Intensity;
    }

    bool RenderShadows() const
    {
        return m_RenderShadows;
    }
    void SetType(const TLightType Type)
    {
        m_Type = Type;
    }
    TLightType GetType() const
    {
        return m_Type;
    }

    virtual void Render(CGraphicsManager *RM);

    /**ADVANCED SHADERS**/
    virtual void SetShadowMap(CGraphicsManager *RM) = 0; //<= Si se deja como virtual puro peta al registrarlo en Lua

    void SetGenerateDynamicShadowMap(bool GenerateDynamicShadowMap)
    {
        m_GenerateDynamicShadowMap = GenerateDynamicShadowMap;
    }

    bool GetGenerateDynamicShadowMap() const
    {
        return m_GenerateDynamicShadowMap;
    }
    void SetGenerateStaticShadowMap(bool GenerateStaticShadowMap)
    {
        m_GenerateStaticShadowMap = GenerateStaticShadowMap;
    }

    bool GetGenerateStaticShadowMap() const
    {
        return m_GenerateStaticShadowMap;
    }

    void SetMustUpdateStaticShadowMap(bool MustUpdateStaticShadowMap)
    {
        m_MustUpdateStaticShadowMap = MustUpdateStaticShadowMap;
    }

    bool GetMustUpdateStaticShadowMap() const
    {
        return m_MustUpdateStaticShadowMap;
    }

    CTexture * GetStaticShadowMap() const
    {
        return m_StaticShadowMap;
    }
    CTexture * GetDynamicShadowMap() const
    {
        return m_DynamicShadowMap;
    }
    CTexture * GetShadowMaskTexture() const
    {
        return m_ShadowMaskTexture;
    }

    void SetShadowMaskTexture(std::string textureName);

    std::vector<CRenderableObjectsManager *> & GetStaticShadowMapRenderableObjectsManagers()
    {
        return m_StaticShadowMapRenderableObjectsManagers;
    }
    std::vector<CRenderableObjectsManager *> & GetDynamicShadowMapRenderableObjectsManagers()
    {
        return m_DynamicShadowMapRenderableObjectsManagers;
    }

    void GenerateShadowMap(CGraphicsManager *RM);

    const Mat44f & GetViewShadowMap() const
    {
        return m_ViewShadowMap;
    }
    const Mat44f & GetProjectionShadowMap() const
    {
        m_ProjectionShadowMap;
    }

    void SetFormatType(std::string FormatType)
    {
        m_FormatType = FormatType;
    }

    GET_SET(int, ShadowMapWidth);
    GET_SET(int, ShadowMapHeigth);
    void BeginRenderEffectManagerShadowMap(CEffect *Effect);

};


#endif
