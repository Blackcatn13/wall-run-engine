#pragma once
#ifndef DRAW_QUAD_RENDERER_COMMAND_H
#define DRAW_QUAD_RENDERER_COMMAND_H


#include "XML\XMLTreeNode.h"
#include "StagedTexturedRendererCommand.h"
#include <string>

class CEffectTechnique;
class CTexture;
class CGraphicsManager;

class CDrawQuadRendererCommand : public CStagedTexturedRendererCommand {
 protected:
  /* CColor m_Color;
   std::string m_TechniqueName;*/
 public:
  CDrawQuadRendererCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

//explicación pagina 22:: Esta clase nos permitirá renderizar un cuadro en 2D según un color. El código
//necesario para poder renderizar un quad 2D con una CEffectTechnique sería el
//siguiente. + CODIGO EJEMPLO

#endif