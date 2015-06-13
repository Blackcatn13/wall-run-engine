#pragma once
#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include <string>
#include "SceneRendererCommand.h"
#include "Utils/TemplatedVectorMapManager.h"

class CSetRenderTargetSceneRendererCommand;
class CSceneRendererCommand;
//class CTemplatedVectorMapManager<CSceneRendererCommand>;
class CGraphicsManager;

class CSceneRendererCommandManager {
 private:
  CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;
  CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommandsGUI;
  std::string m_FileName;
  void CleanUp();
  std::string GetNextName();
  bool m_needReload;
  int m_CommandNumber;
  void GetSetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SceneRendererCommand);
 public:
  CSceneRendererCommandManager();
  ~CSceneRendererCommandManager();
  void Load(const std::string &FileName);
  void Execute(CGraphicsManager &RM);
  void Reload();
  void setReload() {m_needReload = true;}

};
//XML a parsear scene_renderer_commands.xml pag 17 pdf


#endif