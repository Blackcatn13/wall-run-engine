#pragma once
#ifndef RENDERABLE_OBJECTS_LAYERS_MANAGER_H
#define RENDERABLE_OBJECTS_LAYERS_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "Renderable\RenderableObjectsManager.h"

#define MAX_ROOMS	15

class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager <CTemplatedVectorMapManager<CRenderableObjectsManager>> {
 private:
  std::string m_FileName, m_FileName2, m_CurrentLayer;
  std::string m_DefaultLayerName;
  CRenderableObjectsManager *m_DefaultRenderableObjectManager;

  CRenderableObjectsManager *GetRenderableObjectManager(CXMLTreeNode &Node, const std::string &roomNumber);
 public:
  CRenderableObjectsLayersManager();
  ~CRenderableObjectsLayersManager();
  CRenderableObjectsManager *GetDefaultRenderableObjectManager(const std::string &roomNumber);
  CRenderableObjectsManager *GetRenderableObjectsManagerByStr(const std::string &layerName);
  CRenderableObjectsManager *GetRenderableObjectsManagerByStrAndRoom(const std::string &layerName, int roomNumber);
  void ChangeBetweenVectors( const std::string &layer, const std::string &objName, int room );
  void ChangeBetweenVectors( const std::string &layer, const std::string &objName, int current_room, int room );
  void ChangeBetweenLayers(std::string originalLayer, std::string finalLayer, int room,  std::string objName);
  void DeactivateObjectsByStr(const std::string &layerName);
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