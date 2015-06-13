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


CRenderableObjectsLayersManager ::CRenderableObjectsLayersManager() {
  m_DefaultRenderableObjectManager = NULL;
  m_CurrentLayer = "solid";
}

CRenderableObjectsLayersManager ::~CRenderableObjectsLayersManager() {
  Destroy();
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime) {
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(ElapsedTime);
  }
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM) {
  //int test = m_ResourcesVector.size();
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    //int test2 = i;
    m_ResourcesVector[i]->Render(RM);
  }
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *RM, const std::string &LayerName) {
  CRenderableObjectsManager *l_managerInstance = GetResource(LayerName);
  if (l_managerInstance != NULL) {
    l_managerInstance->Render(RM);
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
  Destroy();
  SMESHM->Reload();
  Load(m_FileName);
  Load(m_FileName2);
  std::stringstream ss;
  int var = 1;
  ss << CCORE->getLuaLoadLevelFunc() << "(" << var << ")";
  std::string toRun = ss.str();
  SCRIPTM->RunCode(toRun.c_str());
  LIGHTM->ReloadRO();
  ENEMYM->Reload();
  COLM->Reload();

}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node) {
  std::string l_LayerName = Node.GetPszProperty("layer", m_DefaultLayerName.c_str(), false);
  return GetResource(l_LayerName);
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
}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetRenderableObjectsManagerByStr(std::string layerName) {
  std::string l_LayerName = layerName;

  return GetResource(l_LayerName);

}

CRenderableObjectsManager *CRenderableObjectsLayersManager::GetDefaultRenderableObjectManager() {
  return GetResource( m_DefaultLayerName.c_str());
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName) {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  }
  CXMLTreeNode  m = newFile["renderable_objects"];
  CXMLTreeNode  m2 = newFile["lua_scripts"];
  if (m.Exists()) {
    m_FileName = FileName;
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i) {
      CXMLTreeNode nodeChild = m(i);
      std::string name = nodeChild.GetName();
      if (name == "layer") {
        std::string layerName = nodeChild.GetPszISOProperty("name", "");
        bool isDefault = nodeChild.GetBoolProperty("default", false, false);
        CRenderableObjectsManager *l_managerInstance = new CRenderableObjectsManager();
        //CMeshInstance* l_meshInstance = new CMeshInstance(nodeChild);
        AddResource(layerName, l_managerInstance);
        if (isDefault) {
          m_DefaultLayerName = layerName;
          m_DefaultRenderableObjectManager = l_managerInstance;
        }
      }
    }
    for (int i = 0; i < count; ++i) {
      CXMLTreeNode nodeChild = m(i);
      std::string name = nodeChild.GetName();
      //if ((name == "mesh_instance") || (name == "renderable_script")){
      if (name == "mesh_instance" || name == "animated_model" || name == "platform" || name == "switch_instance" || name == "door") {
        (GetRenderableObjectManager(nodeChild))->Load(nodeChild);
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
      std::string name = m2(i).GetName();
      if (name == "renderable_script") {
        GetDefaultRenderableObjectManager()->Load(m2(i));
      }
    }
  }
}