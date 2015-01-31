#pragma once
#ifndef RENDERABLE_OBJECTS_LAYERS_MANAGER_H
#define RENDERABLE_OBJECTS_LAYERS_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "Renderable\RenderableObjectsManager.h"

class CRenderableObjectsLayersManager :public CTemplatedVectorMapManager<CRenderableObjectsManager>
{
	private:
		std::string m_FileName;
		std::string m_DefaultLayerName;
		CRenderableObjectsManager *m_DefaultRenderableObjectManager;
		CRenderableObjectsManager * GetRenderableObjectManager(CXMLTreeNode &Node);
	public:
		CRenderableObjectsLayersManager();
		~CRenderableObjectsLayersManager();
		void Destroy();
		void Load(const std::string &FileName);
		void Reload();
		void Update(float ElapsedTime);
		void Render(CGraphicsManager *RM);
		void Render(CGraphicsManager *RM, const std::string & LayerName);
};


#endif