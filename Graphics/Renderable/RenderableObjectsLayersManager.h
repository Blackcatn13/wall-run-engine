#pragma once
#ifndef RENDERABLE_OBJECTS_LAYERS_MANAGER_H
#define RENDERABLE_OBJECTS_LAYERS_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "Renderable\RenderableObjectsManager.h"

#define MAX_ROOMS	11

class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager <CTemplatedVectorMapManager<CRenderableObjectsManager>> {
 private:
  std::string m_FileName, m_FileName2, m_CurrentLayer;
  std::string m_DefaultLayerName;
  CRenderableObjectsManager *m_DefaultRenderableObjectManager;

  CRenderableObjectsManager *GetRenderableObjectManager(CXMLTreeNode &Node, std::string roomNumber);
 public:
  CRenderableObjectsLayersManager();
  ~CRenderableObjectsLayersManager();
  CRenderableObjectsManager *GetDefaultRenderableObjectManager(std::string roomNumber);
  CRenderableObjectsManager *GetRenderableObjectsManagerByStr(std::string layerName);
  CRenderableObjectsManager *GetRenderableObjectsManagerByStrAndRoom(std::string layerName, int roomNumber);
  void ChangeBetweenVectors( std::string layer, std::string objName, int room );
  void DeactivateObjectsByStr(std::string layerName);
  void DestroyRenderableObjectManagers();
  int GetPreviousZone();
  int GetNextZone();
  //void Destroy();
  void Load(const std::string &FileName);
  void Reload();
  void Update(float ElapsedTime);
  void Render(CGraphicsManager *RM);
  void Render(CGraphicsManager *RM, const std::string &LayerName);
  GET_SET(std::string, CurrentLayer);
};


#endif