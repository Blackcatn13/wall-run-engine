#include "CollectibleManager.h"
#include "Core\Core.h"
#include "TriggerManager\TriggerManager.h"
#include "XML\XMLTreeNode.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Mesh\MeshInstance.h"
#include "Pixelite.h"
#include "Cromo.h"
#include "Core\ScriptManager.h"


CCollectibleManager::CCollectibleManager():
  m_FileName("") ,
  m_Layer("collectible") {

}

CCollectibleManager::~CCollectibleManager() {
  DeInit();
}

void CCollectibleManager::InitCollectibles(std::string layerName) {
  CRenderableObjectsManager *l_Rom = RENDLM->GetRenderableObjectsManagerByStr(layerName);
  for (int i = 0; i < l_Rom->GetResourcesVector().size(); ++i) {
    for (int j = 0; j < m_VectorCollectibleTypes.size(); ++j) {
      if (((CMeshInstance *)l_Rom->GetResourcesVector()[i])->GetCoreName() == m_VectorCollectibleTypes[j].CoreMesh) {
        std::string l_CardName = "";
		bool l_Visible = l_Rom->GetResourcesVector()[i]->getVisible();
        if (m_VectorCollectibleTypes[j].Name == "card")
          l_CardName = m_Unlockables.find(l_Rom->GetResourcesVector()[i]->getName())->first;

        CCollectible *l_Collectible = new CCollectible(l_Rom->GetResourcesVector()[i], m_Layer, m_VectorCollectibleTypes[j].MeshLuaFunction, m_VectorCollectibleTypes[j].TriggerFunction, m_VectorCollectibleTypes[j].TriggerSize, l_CardName, l_Visible  );
        AddResource(l_Rom->GetResourcesVector()[i]->getName(), l_Collectible);

        /*  } else if ((m_VectorCollectibleTypes[j].Name == "sticker")) {
            CCromo *l_Collectible  = new CCromo(l_Rom->GetResourcesVector()[i], m_Layer, m_VectorCollectibleTypes[j].MeshLuaFunction, m_VectorCollectibleTypes[j].TriggerFunction  );
            AddResource(l_Rom->GetResourcesVector()[i]->getName(), l_Collectible);
          }*/
      }
    }
  }
}

void CCollectibleManager::Load(std::string &FileName) {
  m_FileName = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["collectibles"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m(i);
        std::string l_name = nodeChild.GetName();
        if (l_name == "collectible-type") {
          StrCollectibles l_CollectibleType;
          l_CollectibleType.Name = nodeChild.GetPszISOProperty("name", "", false);
          l_CollectibleType.CoreMesh = nodeChild.GetPszISOProperty("core_mesh", "", false);
          l_CollectibleType.MeshLuaFunction = nodeChild.GetPszISOProperty("mesh_lua_function", "", false);
          l_CollectibleType.TriggerFunction = nodeChild.GetPszISOProperty("trigger_function", "", false);
          l_CollectibleType.TriggerSize = nodeChild.GetVect3fProperty("trigger_size", v3fZERO, false);
          m_VectorCollectibleTypes.push_back(l_CollectibleType);
        } else if (l_name == "layer") {
          m_Layer = nodeChild.GetPszISOProperty("name", "collectible", false);
        } else if (l_name == "unlockable-card") {
          std::string l_meshName = nodeChild.GetPszISOProperty("mesh_name", "", false);
          std::string l_imgSrc = nodeChild.GetPszISOProperty("img_src", "", false);
          m_Unlockables.insert(std::pair<std::string, std::string>(l_meshName, l_imgSrc));
          std::stringstream ss;
          ss << "Card.new(\"" << l_meshName << "\"," << "\"" << l_imgSrc << "\")";
          std::string l_Code = ss.str();
          SCRIPTM->RunCode(l_Code);
        }
      }
    }
  }
}

void CCollectibleManager::Load() {
  Load (m_FileName);
}

void CCollectibleManager::Reload() {
  DeInit();
  TRIGGM->Reload();
  Load (m_FileName);
  InitCollectibles(m_Layer);
}

void CCollectibleManager::DeInit() {
  m_Unlockables.clear();
  m_VectorCollectibleTypes.clear();
  Destroy();
}

void CCollectibleManager::Update(float dt) {
  for (int i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(dt);
  }
}