#pragma once
#ifndef SET_POLY_PHISICS_COMMAND_H
#define SET_POLY_PHISICS_COMMAND_H

#include "RenderableCommands\SceneRendererCommand.h"

class CXMLTreeNode;

class CGraphicsManager;

class CSetPolyPhisicsCommand : public CSceneRendererCommand {
 private:
  std::string m_Room;
 public:
  CSetPolyPhisicsCommand(CXMLTreeNode &atts);
  void Execute(CGraphicsManager &RM);
};

#endif