#include "Renderable\RenderableObjectsManager.h"
#include "GraphicsManager.h"
#include "Mesh\MeshInstance.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Core\ScriptedController.h"
#include "SceneElements\BreakablePlatform.h"
#include "SceneElements\StaticPlatform.h"
#include "SceneElements\MovingPlatform.h"
#include "SceneElements\PinchosPlatform.h"
#include "SceneElements\PolyPlatform.h"
#include "Core\ScriptManager.h"
#include "Collectibles\Cromo.h"
#include "Collectibles\Pixelite.h"
#include "SceneElements\Switch.h"
#include "SceneElements\Door.h"


#include <assert.h>

#include "Core_Utils/MemLeaks.h"

CRenderableObjectsManager ::CRenderableObjectsManager() {
  m_FileName = "";
  m_FileName2 = "";
}

CRenderableObjectsManager ::~CRenderableObjectsManager() {
  Destroy();
}

void CRenderableObjectsManager::Update(float ElapsedTime) {
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(ElapsedTime);
  }
}

void CRenderableObjectsManager::Render(CGraphicsManager *RM) {
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Render(RM);
  }
}

CRenderableObject *CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position) {
  CMeshInstance *newInstance = new CMeshInstance(InstanceName, CoreMeshName);
  newInstance->SetPosition(Position);
  AddResource(InstanceName, newInstance);
  return newInstance;
}

CRenderableObject *CRenderableObjectsManager::AddMeshInstance(CXMLTreeNode &Node) {
  CMeshInstance *newInstance = new CMeshInstance(Node);
  AddResource(newInstance->getName(), newInstance);
  return newInstance;
}

CRenderableObject *CRenderableObjectsManager::AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position) {
  assert (!"Not yet implemented");
  return NULL;
}

CRenderableObject *CRenderableObjectsManager::AddAnimatedInstanceModel(CXMLTreeNode &Node) {
  assert (!"Not yet implemented");
  return NULL;
}

void CRenderableObjectsManager::Load(const std::string &FileName) {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  }
  CXMLTreeNode  m = newFile["renderable_objects"];
  CXMLTreeNode m2 = newFile["lua_scripts"];
  if (m.Exists()) {
    m_FileName = FileName;
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i) {
      CXMLTreeNode nodeChild = m(i);
      std::string name = nodeChild.GetName();
      if (name == "mesh_instance") {
        CMeshInstance *l_meshInstance = new CMeshInstance(nodeChild);
        AddResource(l_meshInstance->getName(), l_meshInstance);
      } else if (name == "animated_model") {
        std::string meshName = nodeChild.GetPszISOProperty("name", "box1");
        std::string core = nodeChild.GetPszISOProperty("core", "box");
        Vect3f pos = nodeChild.GetVect3fProperty("pos", v3fZERO);
        float yaw = nodeChild.GetFloatProperty("yaw");
        float pitch = nodeChild.GetFloatProperty("pitch");
        float roll = nodeChild.GetFloatProperty("roll");
        float scale = nodeChild.GetFloatProperty("scale");
        CAnimatedInstanceModel *l_AnimatedInstanceModel  = ANIMAN->GetInstance(core);
        l_AnimatedInstanceModel->SetYaw(yaw);
        l_AnimatedInstanceModel->SetPosition(pos);
        l_AnimatedInstanceModel->SetPitch(pitch);
        l_AnimatedInstanceModel->SetRoll(roll);
        l_AnimatedInstanceModel->SetScale(scale);
        l_AnimatedInstanceModel->setName(meshName);
        AddResource(meshName, l_AnimatedInstanceModel);
      } else if (name == "switch_instance") {
        CSwitch *l_Switch  = new CSwitch(nodeChild);
        AddResource(l_Switch->getName(), l_Switch);
      } else if (name == "door") {
        CDoor *l_Door  = new CDoor(nodeChild);
        AddResource(l_Door->getName(), l_Door);
      }  else  if (name == "platform") {
        std::string type = nodeChild.GetPszISOProperty("type", "", false);
        if (type == "static") {
          CStaticPlatform *l_StaticPlatform = new CStaticPlatform(nodeChild);
          AddResource(l_StaticPlatform->getName(), l_StaticPlatform);
        } else if (type == "breakable") {
          CBreakablePlatform *l_BreakablePlatform = new CBreakablePlatform(nodeChild);
          AddResource(l_BreakablePlatform->getName(), l_BreakablePlatform);
        } else if (type == "moving") {
          CMovingPlatform *l_MovingPlatform = new CMovingPlatform(nodeChild);
          AddResource(l_MovingPlatform->getName(), l_MovingPlatform);
        } else if (type == "pinchos") {
          CPinchosPlatform *l_PinchosPlatform = new CPinchosPlatform(nodeChild);
          AddResource(l_PinchosPlatform->getName(), l_PinchosPlatform);
        } else if (type == "poly") {
          CPolyPlatform *l_PolyPlatform = new CPolyPlatform(nodeChild);
          AddResource(l_PolyPlatform->getName(), l_PolyPlatform);
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
        std::string l_name = m2(i).GetPszISOProperty("name", "");
        //std::string l_file = m2(i).GetPszISOProperty("file", "");
        if (l_name == "scriptedController") {
          CScriptedController *l_ScriptedController = new CScriptedController();
          AddResource(l_name, l_ScriptedController);
        }
      }
    }
  }
}


void CRenderableObjectsManager::Load(CXMLTreeNode &Node) {
  CXMLTreeNode  m = Node;
  if (Node.Exists()) {
    std::string name = m.GetName();
    if (name == "mesh_instance") {
      CMeshInstance *l_meshInstance = new CMeshInstance(Node);
      AddResource(l_meshInstance->getName(), l_meshInstance);
    } else if (name == "animated_model") {
      std::string meshName = Node.GetPszISOProperty("name", "box1");
      std::string core = Node.GetPszISOProperty("core", "box");
      Vect3f pos = Node.GetVect3fProperty("pos", v3fZERO);
      float yaw = Node.GetFloatProperty("yaw");
      float pitch = Node.GetFloatProperty("pitch");
      float roll = Node.GetFloatProperty("roll");
      float scale = Node.GetFloatProperty("scale");
      CAnimatedInstanceModel *l_AnimatedInstanceModel  = ANIMAN->GetInstance(core);
      l_AnimatedInstanceModel->SetYaw(yaw);
      l_AnimatedInstanceModel->SetPosition(pos);
      l_AnimatedInstanceModel->SetPitch(pitch);
      l_AnimatedInstanceModel->SetRoll(roll);
      l_AnimatedInstanceModel->SetScale(scale);
      l_AnimatedInstanceModel->setName(meshName);
      AddResource(meshName, l_AnimatedInstanceModel);
    } else if (name == "switch_instance") {
      CSwitch *l_Switch  = new CSwitch(Node);
      AddResource(l_Switch->getName(), l_Switch);
    } else if (name == "door") {
      CDoor *l_Door  = new CDoor(Node);
      AddResource(l_Door->getName(), l_Door);
    } else  if (name == "renderable_script") {
      std::string l_name = Node.GetPszISOProperty("name", "");
      if (l_name == "scriptedController") {
        CScriptedController *l_ScriptedController = new CScriptedController();
        AddResource(l_name, l_ScriptedController);
      }
    } else  if (name == "platform") {
      std::string type = Node.GetPszISOProperty("type", "", false);
      if (type == "static") {
        CStaticPlatform *l_StaticPlatform = new CStaticPlatform(Node);
        AddResource(l_StaticPlatform->getName(), l_StaticPlatform);
      } else if (type == "breakable") {
        CBreakablePlatform *l_BreakablePlatform = new CBreakablePlatform(Node);
        AddResource(l_BreakablePlatform->getName(), l_BreakablePlatform);
      } else if (type == "moving") {
        CMovingPlatform *l_MovingPlatform = new CMovingPlatform(Node);
        AddResource(l_MovingPlatform->getName(), l_MovingPlatform);
      } else if (type == "pinchos") {
        CPinchosPlatform *l_PinchosPlatform = new CPinchosPlatform(Node);
        AddResource(l_PinchosPlatform->getName(), l_PinchosPlatform);
      } else if (type == "poly") {
        CPolyPlatform *l_PolyPlatform = new CPolyPlatform(Node);
        AddResource(l_PolyPlatform->getName(), l_PolyPlatform);
      }
    }
  }
}
void CRenderableObjectsManager::Reload() {
  Destroy();
  Load(m_FileName);
  Load(m_FileName2);
  std::stringstream ss;
  int var = 1;
  ss << CCORE->getLuaLoadLevelFunc() << "(" << var << ")";
  std::string toRun = ss.str();
  SCRIPTM->RunCode(toRun.c_str());
// ENEMYM->Reload();
}

std::string CRenderableObjectsManager::SetUserDataName(std::string name) {
  std::stringstream ss;
  ss << name << "_UserData";
  return ss.str();
}

void CRenderableObjectsManager::ActivateElements(std::string roomName) {
  for (int i = 0; i < GetResourcesVector().size(); ++ i ) {
    if (((CMovingPlatform *)GetResourcesVector()[i])->getActivated() == false && ((CMovingPlatform *)GetResourcesVector()[i])->getRoom() == roomName) {
      ((CMovingPlatform *)GetResourcesVector()[i])->setActivated(true);
    }
  }
}

void CRenderableObjectsManager::RestoreBrokenPlatforms() {
  for (int i = 0; i < GetResourcesVector().size(); ++ i ) {
    if (((CBreakablePlatform *)GetResourcesVector()[i])->IsBroken() == true) {
      ((CBreakablePlatform *)GetResourcesVector()[i])->EnablePlatform();
    }
  }
}