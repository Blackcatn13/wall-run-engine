#include "RenderableCommands\StagedTexturedRendererCommand.h"
#include "GraphicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Texture\Texture.h"
#include "Utils\Defines.h"


CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &atts)
{
}
void CStagedTexturedRendererCommand::ActivateTextures()
{
    for (int i = 0; i < m_StageTextures.size(); ++i) {
        m_StageTextures[i].m_Texture->Activate(m_StageTextures[i].m_StageId);
    }
}

CStagedTexturedRendererCommand::~CStagedTexturedRendererCommand()
{
    /*for (int i = 0; i < m_StageTextures.size(); ++i) {
        CHECKED_DELETE (m_StageTextures[i].m_Texture);
    }*/
    m_StageTextures.clear();
}
void CStagedTexturedRendererCommand::AddStageTexture(int StageId, CTexture *Texture)
{
    if (Texture != NULL)	{
        //  CKGStageTexture l_CKGStageTexture = new CKGStageTexture(StageId, Texture);
        m_StageTextures.push_back(CKGStageTexture (StageId, Texture));
    }
}

void CStagedTexturedRendererCommand::CKGStageTexture::Activate()
{
    m_Texture->Activate(m_StageId);
}