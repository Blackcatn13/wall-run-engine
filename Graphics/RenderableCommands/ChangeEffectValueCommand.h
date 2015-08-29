#pragma once
#ifndef CHANGE_EFFECT_VALUE_COMMAND_H_
#define CHANGE_EFFECT_VALUE_COMMAND_H_

#include "RenderableCommands\SceneRendererCommand.h"
#include "Effects\EffectTechnique.h"
#include <string>

class CXMLTreeNode;
class CGraphicsManager;

class CChangeEffectValueCommand : public CSceneRendererCommand {
 public:
  CChangeEffectValueCommand(CXMLTreeNode &atts);
  ~CChangeEffectValueCommand();
  void Execute(CGraphicsManager &RM);
 private:
  std::string m_value;
  float m_time;
  std::string m_technique;
  CEffectTechnique *m_effectTechnique;
  float m_currentTime;
  bool m_lastValue;
};

#endif