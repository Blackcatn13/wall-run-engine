#pragma once
#ifndef SET_TEXTURE_IN_SPECIFIC_STAGE_COMMAND_H
#define SET_TEXTURE_IN_SPECIFIC_STAGE_COMMAND_H


#include "XML\XMLTreeNode.h"
#include "SceneRendererCommand.h"
#include <string>

class CEffectTechnique;
class CTexture;
class CGraphicsManager;

class CSetTextureInSpecificStageCommand : public CSceneRendererCommand
{
protected:
	std::string m_TextureName;
	size_t			m_Stage;
public:
    CSetTextureInSpecificStageCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

//explicaci�n pagina 22:: Esta clase nos permitir� renderizar un cuadro en 2D seg�n un color. El c�digo
//necesario para poder renderizar un quad 2D con una CEffectTechnique ser�a el
//siguiente. + CODIGO EJEMPLO

#endif