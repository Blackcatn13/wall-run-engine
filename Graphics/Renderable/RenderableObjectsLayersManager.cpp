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
}

CRenderableObjectsManager  *CRenderableObjectsLayersManager::GetRenderableObjectsManagerByStrAndRoom(std::string layerName, int roomNumber) {
  std::stringstream ss;
  ss << roomNumber;
  return GetResource(ss.str())->GetResource(layerName);
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime) {
  for (size_t i = GetPreviousZone() ; i < GetNextZone() + 1; ++i) {
    // m_ResourcesVector[i]->Update(ElapsedTime);
    for (size_t j = 0; j < m_ResourcesVector[i]->GetResourcesVector().size(); ++j) {
      m_ResourcesVector[i]->GetResourcesVector()[j]->  Update(ElapsedTime);
    }
  }
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM) {
  for (size_t i = GetPreviousZone(); i < GetNextZone() + 1; ++i) {
    // m_ResourcesVector[i]->Update(ElapsedTime);
    for (size_t j = 0; j < m_ResourcesVector[i]->GetResourcesVector().size(); ++j) {
      m_ResourcesVector[i]->GetResourcesVector()[j]->Render(RM);
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
  for (size_t i = GetPreviousZone(); i < GetNextZone() + 1; ++i) {
    // m_ResourcesVector[i]->Update(ElapsedTime);
    for (size_t j = 0; j < m_ResourcesVector[i]->GetResourcesVector().size(); ++j) {
      CRenderableObjectsManager *l_managerInstance = m_ResourcesVector[i]->GetResource(LayerName);
      if (l_managerInstance != NULL) {
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

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node, std::string roomNumber) {
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

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetRenderableObjectsManagerByStr(std::string layerName) {
  std::string l_LayerName = layerName;
  //devuelve el renderable en funcion del nombre de layer y sala del player
  std::stringstream ss;
  ss << PLAYC->getRoom();

  return GetResource(ss.str())->GetResource(l_LayerName);
}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetDefaultRenderableObjectManager(std::string roomName) {
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
          CXMLTreeNode nodeChild = m(j);
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

        for (int i = 0; i < count; ++i) {
          CXMLTreeNode nodeChild = m(i);
          std::string name = nodeChild.GetName();
          //if ((name == "mesh_instance") || (name == "renderable_script")){
          if (name == "mesh_instance" || name == "animated_model" || name == "platform" || name == "switch_instance" || name == "door") {
            if (nodeChild.GetIntProperty("room", 0) == numRoom/*it - l_Vector.begin()*/) {
              (GetRenderableObjectManager(nodeChild, ss.str()))->Load(nodeChild);
            }
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
    }
    if (m2.Exists()) {
      m_FileName2 = FileName;
      int count = m2.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        std::string name = m2(i).GetName();
        if (name == "renderable_script") {
          GetDefaultRenderableObjectManager("0")->Load(m2(i));
        }
      }
    }
  }
}

void CRenderableObjectsLayersManager::DeactivateObjectsByStr(std::string layerName) {

  for (size_t i = 0 ; i < MAX_ROOMS ; ++i) {
    CRenderableObjectsManager *manager = m_ResourcesVector[i]->GetResource(layerName);

    for (int j = 0; j < manager->GetResourcesVector().size(); j++) {
      std::string name = manager->GetResourcesVector().at(j)->getName();
      std::stringstream str;
      str << "deactivate_invisible_wall(\"" << name << "\")";
      std::string funct  = str.str();

      SCRIPTM->RunCode(funct);
    }
  }
}