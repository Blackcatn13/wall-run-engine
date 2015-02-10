#pragma once
#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include <string>
#include "Utils\MapManager.h"
#include "SceneRendererCommand.h"



class CSceneRendererCommandManager {
	private:
		//CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;
		CMapManager<CSceneRendererCommand> m_SceneRendererCommands;
		void CleanUp();
		std::string GetNextName();
	public:
		CSceneRendererCommandManager();
		~CSceneRendererCommandManager();
		void Load(const std::string &FileName);
	void Execute(CGraphicsManager& RM);
};
//XML a parsear scene_renderer_commands.xml pag 17 pdf


#endif