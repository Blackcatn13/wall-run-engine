#include "Renderable\RenderableObjectsLayersManager.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include <assert.h>
#include "AnimatedModels\AnimatedModelManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Lights\LightManager.h"
#include "Core\ScriptManager.h"
#include "Enemies\EnemyManager.h"
#include "Collectibles\CollectibleManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Core\PlayerController.h"
#include "Renderable\RenderableObject.h"


CRenderableObjectsLayersManager ::CRenderableObjectsLayersManager() {
  m_DefaultRenderableObjectManager = NULL;
  m_CurrentLayer = "solid";
}

CRenderableObjectsLayersManager ::~CRenderableObjectsLayersManager() {
  DestroyRenderableObjectManagers();
}

void CRenderableObjectsLayersManager::DestroyRenderableObjectManagers() {
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    // m_ResourcesVector[i]->Update(ElapsedTime);
    for (size_t j = 0; j < m_ResourcesVector[i]->GetResourcesVector().size(); ++j) {
      m_ResourcesVector[i]->Destroy();
    }
  }
  Destroy();
}

CRenderableObjectsManager  *CRenderableObjectsLayersManager::GetRenderableObjectsManagerByStrAndRoom(const std::string &layerName, int roomNumber) {
  std::stringstream ss;
  ss << roomNumber;
  return GetResource(ss.str())->GetResource(layerName);
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime) {
  if (PLAYC->getRoom() != 7) {
    int l_NextZone = GetNextZone() + 1;
    if (l_NextZone > m_ResourcesVector.size())
      l_NextZone = GetNextZone();
    for (size_t i = GetPreviousZone() ; i < l_NextZone; ++i) {
      // m_ResourcesVector[i]->Update(ElapsedTime);
      CTemplatedVectorMapManager<CRenderableObjectsManager> *aux = m_ResourcesVector[i];
      size_t maxSize = aux->GetSize();
      std::vector<CRenderableObjectsManager *> resources = aux->GetResourcesVector();
      for (size_t j = 0; j < maxSize; ++j) {
        if (resources[j]->GetSize() > 0)
          resources[j]->  Update(ElapsedTime);
      }
    }
  } else {
    int rooms[3] = {6, 7, 0};
    for (size_t i = 0; i < 3; ++i) {
      CTemplatedVectorMapManager<CRenderableObjectsManager> *aux = m_ResourcesVector[rooms[i]];
      size_t maxSize = aux->GetSize();
      std::vector<CRenderableObjectsManager *> resources = aux->GetResourcesVector();
      for (size_t j = 0; j < maxSize; ++j) {
        if (resources[j]->GetSize() > 0)
          resources[j]->  Update(ElapsedTime);
      }
    }
  }
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM) {
  if (PLAYC->getRoom() != 7) {
    int l_NextZone = GetNextZone() + 1;
    if (l_NextZone > m_ResourcesVector.size())
      l_NextZone = GetNextZone();
    for (size_t i = GetPreviousZone(); i < GetNextZone(); ++i) {
      // m_ResourcesVector[i]->Update(ElapsedTime);
      CTemplatedVectorMapManager<CRenderableObjectsManager> *aux = m_ResourcesVector[i];
      size_t maxSize = aux->GetSize();
      std::vector<CRenderableObjectsManager *> resources = aux->GetResourcesVector();
      for (size_t j = 0; j < maxSize; ++j) {
        if (resources[j]->GetSize() > 0)
          resources[j]->Render(RM);
      }
    }
  } else {
    int rooms[3] = {6, 7, 0};
    for (size_t i = 0; i < 3; ++i) {
      CTemplatedVectorMapManager<CRenderableObjectsManager> *aux = m_ResourcesVector[rooms[i]];
      size_t maxSize = aux->GetSize();
      std::vector<CRenderableObjectsManager *> resources = aux->GetResourcesVector();
      for (size_t j = 0; j < maxSize; ++j) {
        if (resources[j]->GetSize() > 0)
          resources[j]->  Render(RM);
      }
    }
  }
}

int CRenderableObjectsLayersManager::GetPreviousZone() {
  if (PLAYC->getRoom() - 1 < 0)
    return 0;
  return PLAYC->getRoom() - 1;
}

int CRenderableObjectsLayersManager::GetNextZone() {
  if (PLAYC->getRoom() + 1 > m_ResourcesVector.size())
    return PLAYC->getRoom();
  return PLAYC->getRoom() + 1;
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM, const std::string &LayerName) {
  if (PLAYC->getRoom() != 7) {
    int l_NextZone = GetNextZone() + 1;
    if (l_NextZone > m_ResourcesVector.size())
      l_NextZone = GetNextZone();
    for (size_t i = GetPreviousZone(); i < l_NextZone; ++i) {
      CRenderableObjectsManager *l_managerInstance = m_ResourcesVector[i]->GetResource(LayerName);
      if (l_managerInstance != NULL && l_managerInstance->GetSize() > 0) {
        l_managerInstance->Render(RM);
      }
    }
  } else {
    int rooms[3] = {6, 7, 0};
    for (size_t i = 0; i < 3; ++i) {
      CRenderableObjectsManager *l_managerInstance = m_ResourcesVector[rooms[i]]->GetResource(LayerName);
      if (l_managerInstance != NULL && l_managerInstance->GetSize() > 0) {
        l_managerInstance->Render(RM);
      }
    }
  }
}

/*
void CRenderableObjectsLayersManager::Destroy()
{
	for (int i = 0; i < m_ResourcesVector.size(); ++i) {
        m_ResourcesVector[i]->Destroy();
    }
    Destroy();
}
*/
void CRenderableObjectsLayersManager::Reload() {
  DestroyRenderableObjectManagers();
  SMESHM->Reload();
  Load(m_FileName);
  Load(m_FileName2);
  LIGHTM->ReloadRO();
  ENEMYM->Reload();
  SCRIPTM->Reload();
  COLM->Reload();
}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node, const std::string &roomNumber) {
  std::string l_LayerName = Node.GetPszProperty("layer", m_DefaultLayerName.c_str(), false);

  return GetResource(roomNumber)->GetResource(l_LayerName);
  /*
    	CXMLTreeNode  m = Node;
        if (Node.Exists()) {
    		int count = m.GetNumChildren();
    		for (int i = 0; i < count; ++i) {
    				std::string name = m(i).GetName();
    				if (name == "mesh_instance")
    				{
    					std::string layerName = m(i).GetPszISOProperty("layer", m_DefaultLayerName);
    					return GetResource(layerName);
    			 }
    		}
    	}*/

  return NULL;
}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetRenderableObjectsManagerByStr(const std::string &layerName) {
  std::string l_LayerName = layerName;
  //devuelve el renderable en funcion del nombre de layer y sala del player
  std::stringstream ss;
  ss << PLAYC->getRoom();

  return GetResource(ss.str())->GetResource(l_LayerName);
}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetDefaultRenderableObjectManager(const std::string &roomName) {
  return GetResource(roomName)->GetResource( m_DefaultLayerName.c_str());
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName) {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["renderable_objects"];
    CXMLTreeNode  m2 = newFile["lua_scripts"];
    if (m.Exists()) {
      m_FileName = FileName;
      int count = m.GetNumChildren();
      // std::vector <CTemplatedVectorMapManager<CRenderableObjectsManager>> l_Vector;
      //  CTemplatedVectorMapManager<CTemplatedVectorMapManager<CRenderableObjectsManager>>::TVectorResources::iterator  it =  m_ResourcesVector.begin();
      // GetResourcesVector().resize(MAX_ROOMS);
      for (int numRoom = 0; numRoom < MAX_ROOMS; ++numRoom) {
        CTemplatedVectorMapManager<CRenderableObjectsManager> *l_ResourcesVector = new  CTemplatedVectorMapManager<CRenderableObjectsManager>();
        for (int j = 0; j < count; ++j) {
          CXMLTreeNode nodeChild = m.getNextChild();
          std::string name = nodeChild.GetName();
          if (name == "layer") {
            std::string layerName = nodeChild.GetPszISOProperty("name", "");
            bool isDefault = nodeChild.GetBoolProperty("default", false, false);
            CRenderableObjectsManager *l_managerInstance = new CRenderableObjectsManager();
            //CMeshInstance* l_meshInstance = new CMeshInstance(nodeChild);

            l_ResourcesVector->AddResource(layerName, l_managerInstance);
            if (isDefault) {
              m_DefaultLayerName = layerName;
              m_DefaultRenderableObjectManager = l_managerInstance;
            }
          }
        }
        std::stringstream ss;
        ss << numRoom;
        AddResource(ss.str(), l_ResourcesVector);
      }
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m.getNextChild();
        std::string name = nodeChild.GetName();
        //if ((name == "mesh_instance") || (name == "renderable_script")){
        if (name == "mesh_instance" || name == "animated_model" || name == "platform" || name == "switch_instance" || name == "door" || name == "wall_trap") {
          int roomNum = nodeChild.GetIntProperty("room", 0);/*it - l_Vector.begin()*/
          std::stringstream ss;
          ss << roomNum;
          (GetRenderableObjectManager(nodeChild, ss.str()))->Load(nodeChild);

          /*std::string layerAssigned = nodeChild.GetPszISOProperty("layer", "box1");
          if (layerAssigned == "box1")
          {
            m_DefaultRenderableObjectManager->Load(nodeChild);
          }
          else
          {
            //FIND layerAssigned, load nodeChild
            CRenderableObjectsManager* l_managerInstance = GetResource(layerAssigned);
            if (l_managerInstance != NULL)
            {
              l_managerInstance->Load(nodeChild);
            }
          }*/
        }
      }
    }
    if (m2.Exists()) {
      m_FileName2 = FileName;
      int count = m2.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode child = m2.getNextChild();
        std::string name = child.GetName();
        if (name == "renderable_script") {
          GetDefaultRenderableObjectManager("0")->Load(child);
        }
      }
    }
  }
}

void CRenderableObjectsLayersManager::DeactivateObjectsByStr(const std::string &layerName) {

  for (size_t i = 0 ; i < MAX_ROOMS ; ++i) {
    CRenderableObjectsManager *manager = m_ResourcesVector[i]->GetResource(layerName);

    for (int j = 0; j < manager->GetResourcesVector().size(); j++) {
      std::string name = manager->GetResourcesVector().at(j)->getName();
      std::stringstream str;
      str << "deactivate_invisible_wall(\"" << name << "\", \"" << i << "\")";
      std::string funct  = str.str();

      SCRIPTM->RunCode(funct);
    }
  }
}

void CRenderableObjectsLayersManager::ChangeBetweenVectors( const std::string &layer, const std::string &objName, int room ) {
  if (PLAYC->getRoom() != room) {
    CRenderableObjectsManager *l_Rolm = m_ResourcesVector[PLAYC->getRoom()]->GetResource(layer);
    CRenderableObjectsManager *l_RolmAux = m_ResourcesVector[room]->GetResource(layer);
    CRenderableObject *l_Ro = l_Rolm ->GetResource(objName);
    l_Ro->setRoom(room);
    l_RolmAux->AddResource(objName, l_Ro);
    l_Rolm->RemoveFromResource(objName);
  }
}

void CRenderableObjectsLayersManager::ChangeBetweenVectors( const std::string &layer, const std::string &objName, int current_room, int room ) {
  if (current_room != room) {
    CRenderableObjectsManager *l_Rolm = m_ResourcesVector[current_room]->GetResource(layer);
    CRenderableObjectsManager *l_RolmAux = m_ResourcesVector[room]->GetResource(layer);
    CRenderableObject *l_Ro = l_Rolm ->GetResource(objName);
    l_Ro->setRoom(room);
    l_RolmAux->AddResource(objName, l_Ro);
    l_Rolm->RemoveFromResource(objName);
  }
}

void  CRenderableObjectsLayersManager::ChangeBetweenLayers(std::string originalLayer, std::string finalLayer, int room, std::string objName) {

  CRenderableObject *rend =   RENDLM->GetRenderableObjectsManagerByStrAndRoom(originalLayer, room)->GetResource(objName);
  if (rend != NULL) {

    RENDLM->GetRenderableObjectsManagerByStrAndRoom(finalLayer, room)->AddResource(objName, rend);
    RENDLM->GetRenderableObjectsManagerByStrAndRoom(originalLayer, room)->RemoveFromResource(objName);
  }
}

CRenderableObject *CRenderableObjectsLayersManager::GetResourceFromLayersAndRoom(const std::string &layerName, const std::string layerName2, const std::string name, int roomNumber) {
  CRenderableObject *l_Object = GetRenderableObjectsManagerByStrAndRoom(layerName, roomNumber)->GetResourceWithoutLog(name);
  if (l_Object == NULL)
    l_Object = GetRenderableObjectsManagerByStrAndRoom(layerName2, roomNumber)->GetResource(name);
  return l_Object;
}