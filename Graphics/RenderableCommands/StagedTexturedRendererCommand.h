#pragma once
#ifndef STAGED_TEXTURED_RENDERER_COMMAND_H
#define STAGED_TEXTURED_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "Texture\Texture.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

class CStagedTexturedRendererCommand : public CSceneRendererCommand
{
protected:
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
};

#endif