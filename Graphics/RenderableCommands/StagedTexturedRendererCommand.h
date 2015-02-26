#pragma once
#ifndef STAGED_TEXTURED_RENDERER_COMMAND_H
#define STAGED_TEXTURED_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include <string>

class CGraphicsManager;
class CTexture;
class CXMLTreeNode;

class CStagedTexturedRendererCommand : public CSceneRendererCommand
{
protected:
    CColor m_Color;
    std::string m_TechniqueName;
    class CKGStageTexture
    {
    public:
        int m_StageId;
        CTexture *m_Texture;
        CKGStageTexture(int StageId, CTexture *Texture)
        {
            m_StageId = StageId;
            m_Texture = Texture;
        }
        void Activate();
    };
    std::vector<CKGStageTexture> m_StageTextures;
public:
    CStagedTexturedRendererCommand() {}
    CStagedTexturedRendererCommand(CXMLTreeNode &atts);
    virtual ~CStagedTexturedRendererCommand();
    void ActivateTextures();
    void AddStageTexture(int StageId, CTexture *Texture);
    virtual void Execute(CGraphicsManager &RM) = 0;
    void DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CGraphicsManager *RM, CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture,
            float U0 = 0.0f, float V0 = 0.0f, float U1 = 1.0f, float V1 = 1.0f);
};

#endif