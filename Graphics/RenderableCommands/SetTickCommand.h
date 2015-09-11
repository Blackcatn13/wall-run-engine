#pragma once
#ifndef SET_TICK_COMMAND_H_
#define SET_TICK_COMMAND_H_

#include "RenderableCommands\SceneRendererCommand.h"
#include <string>

class CXMLTreeNode;
class CGraphicsManager;

class CSetTickCommand : public CSceneRendererCommand {
 public:
  CSetTickCommand(CXMLTreeNode &atts);
  ~CSetTickCommand();
  void Execute(CGraphicsManager &RM);
 private:
  std::string name;
  std::string m_technique;
  CEffectTechnique *m_effectTechnique;
};

#endif