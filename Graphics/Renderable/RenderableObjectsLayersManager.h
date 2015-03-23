#pragma once
#ifndef RENDERABLE_OBJECTS_LAYERS_MANAGER_H
#define RENDERABLE_OBJECTS_LAYERS_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "Renderable\RenderableObjectsManager.h"

class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager<CRenderableObjectsManager> {
 private:
  std::string m_FileName, m_FileName2, m_CurrentLayer;
  std::string m_DefaultLayerName;
  CRenderableObjectsManager *m_DefaultRenderableObjectManager;
  CRenderableObjectsManager *GetRenderableObjectManager(CXMLTreeNode &Node);
 public:
  CRenderableObjectsLayersManager();
  ~CRenderableObjectsLayersManager();
  CRenderableObjectsManager *GetDefaultRenderableObjectManager();
  CRenderableObjectsManager *GetRenderableObjectsManagerByStr(std::string layerName);
  //void Destroy();
  void Load(const std::string &FileName);
  void Reload();
  void Update(float ElapsedTime);
  void Render(CGraphicsManager *RM);
  void Render(CGraphicsManager *RM, const std::string &LayerName);
  GET_SET(std::string, CurrentLayer);
};


#endif