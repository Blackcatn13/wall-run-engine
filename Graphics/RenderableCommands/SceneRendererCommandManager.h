#pragma once
#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include <string>
#include <map>
#include "SceneRendererCommand.h"
#include "Utils/TemplatedVectorMapManager.h"

class CSetRenderTargetSceneRendererCommand;
class CSceneRendererCommand;
//class CTemplatedVectorMapManager<CSceneRendererCommand>;
class CGraphicsManager;

class CSceneRendererCommandManager {
 private:
  std::map<std::string, CTemplatedVectorMapManager<CSceneRendererCommand>> m_commandsMaps;
  //CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;
  //CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommandsGUI;
#ifdef _PARTICLEVIEWER
  //CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommandsParticle;
#endif
  std::string m_FileName;
  void CleanUp();
  std::string GetNextName();
  bool m_needReload;
  bool m_needReloadGUI;
  int m_CommandNumber;
  void GetSetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SceneRendererCommand);
 public:
  CSceneRendererCommandManager();
  ~CSceneRendererCommandManager();
  void Load(const std::string &FileName);
  void Execute(CGraphicsManager &RM);
  void Reload();
  void setReload() {m_needReload = true; m_needReloadGUI = true;}
  std::string m_activeRenderer;

};
//XML a parsear scene_renderer_commands.xml pag 17 pdf


#endif