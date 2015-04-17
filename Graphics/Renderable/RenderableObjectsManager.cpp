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
  for (int i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(ElapsedTime);
  }
}

void CRenderableObjectsManager::Render(CGraphicsManager *RM) {
  for (int i = 0; i < m_ResourcesVector.size(); ++i) {
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
  //this->Load(m);
  //TODO comentar todo y usar la llamada a Load(XMLTreeNode)
  if (m.Exists()) {
    m_FileName = FileName;
    //cargamos aqui el animated models para no hacerlo varias veces dentro del bucle
    //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\level1\\animated_models.xml"); //se carga animatedmodels.xml
    //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\animated_models.xml"); //se carga animatedmodels.xml
    int count = m.GetNumChildren();
    for (int i = 0; i < count; ++i) {
      std::string name = m(i).GetName();
      if (name == "mesh_instance") {
        //TODO
        std::string meshName = m(i).GetPszISOProperty("name", "box1");
        std::string core = m(i).GetPszISOProperty("core", "box");
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
        float yaw = m(i).GetFloatProperty("yaw");
        float pitch = m(i).GetFloatProperty("pitch");
        float roll = m(i).GetFloatProperty("roll");
        Vect3f scale = m(i).GetVect3fProperty("scale", v3fONE);
        //TODO Static mesh por mesh instance
        //
        CMeshInstance *l_meshInstance = new CMeshInstance(meshName, core);
        l_meshInstance->SetYaw(yaw);
        l_meshInstance->SetPosition(pos);
        l_meshInstance->SetPitch(pitch);
        l_meshInstance->SetRoll(roll);
        l_meshInstance->SetScale(scale);
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(meshName, l_meshInstance);
      } else if (name == "animated_model") {
        //FALTA CARGAR EL ANIMATED_MODELS.XML
        std::string meshName = m(i).GetPszISOProperty("name", "box1");
        std::string core = m(i).GetPszISOProperty("core", "box");
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
        float yaw = m(i).GetFloatProperty("yaw");
        float pitch = m(i).GetFloatProperty("pitch");
        float roll = m(i).GetFloatProperty("roll");
        float scale = m(i).GetFloatProperty("scale");
        CAnimatedInstanceModel *l_AnimatedInstanceModel  = ANIMAN->GetInstance(core);
        l_AnimatedInstanceModel->SetYaw(yaw);
        l_AnimatedInstanceModel->SetPosition(pos);
        l_AnimatedInstanceModel->SetPitch(pitch);
        l_AnimatedInstanceModel->SetRoll(roll);
        l_AnimatedInstanceModel->SetScale(scale);
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(meshName, l_AnimatedInstanceModel);
      } else  if (name == "platform") {
        //TODO
        std::string platformName = m(i).GetPszISOProperty("name", "box1");
        std::string core = m(i).GetPszISOProperty("core", "box");
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
        float yaw = m(i).GetFloatProperty("yaw");
        float pitch = m(i).GetFloatProperty("pitch");
        float roll = m(i).GetFloatProperty("roll");
        Vect3f scale = m(i).GetVect3fProperty("scale", v3fONE);
        std::string type = m(i).GetPszISOProperty("type", "", false);
        //TODO Static mesh por mesh instance
        //
        if (type == "static") {
          CStaticPlatform *l_StaticPlatform = new CStaticPlatform(platformName, core);
          l_StaticPlatform->SetYaw(yaw);
          l_StaticPlatform->SetPosition(pos);
          l_StaticPlatform->SetPitch(pitch);
          l_StaticPlatform->SetRoll(roll);
          l_StaticPlatform->SetScale(scale);
          //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
          AddResource(platformName, l_StaticPlatform);
          //std::string boxName, std::string coreName, std::string userDataName, Vect3f size, Vect3f localPosition)
        } else if (type == "breakable") {
          std::string l_TriggerName =  m(i).GetPszISOProperty("trigger_name", "", false);
          CBreakablePlatform *l_BreakablePlatform = new CBreakablePlatform(platformName, core, l_TriggerName);
          l_BreakablePlatform->SetYaw(yaw);
          l_BreakablePlatform->SetPosition(pos);
          l_BreakablePlatform->SetPitch(pitch);
          l_BreakablePlatform->SetRoll(roll);
          l_BreakablePlatform->SetScale(scale);
          //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
          AddResource(platformName, l_BreakablePlatform);
          //std::string boxName, std::string coreName, std::string userDataName, Vect3f size, Vect3f localPosition)
        } else if (type == "moving") {
          float l_speed =  m(i).GetFloatProperty("speed", 0.0f, false);
          int l_WpCount = m(i).GetNumChildren();
          CMovingPlatform *l_MovingPlatform = new CMovingPlatform(platformName, core, l_speed);
          for (int j = 0; j < l_WpCount; ++j) {
            std::string name = m(i)(j).GetName();
            if (name == "wp") {
              Vect3f l_Wp = m(i)(j).GetVect3fProperty("pos", Vect3f(.0f, .0f, .0f), false);
              l_MovingPlatform->GetWayPointsVector().push_back(l_Wp);
            }
          }
          l_MovingPlatform->SetNextWPVector(l_MovingPlatform->GetWayPointsVector()[1]);
          l_MovingPlatform->SetYaw(yaw);
          l_MovingPlatform->SetPosition(pos);
          l_MovingPlatform->SetPitch(pitch);
          l_MovingPlatform->SetRoll(roll);
          l_MovingPlatform->SetScale(scale);
          //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
          AddResource(platformName, l_MovingPlatform);
        } else if (type == "pinchos") {
          std::string l_TriggerName =  m(i).GetPszISOProperty("trigger_name", "", false);
          Vect3f l_BackPos = m(i).GetVect3fProperty("back_position", v3fZERO, false);
          Vect3f l_FrontPos = m(i).GetVect3fProperty("forth_position", v3fZERO, false);
          bool	l_FromX = m(i).GetBoolProperty("from_x", false, false);
          bool	l_FromZ = m(i).GetBoolProperty("from_z", false, false);
          CPinchosPlatform *l_PinchosPlatform = new CPinchosPlatform(platformName, core, l_TriggerName, l_BackPos, l_FrontPos,  l_FromX, l_FromZ);
          l_PinchosPlatform->SetYaw(yaw);
          l_PinchosPlatform->SetPosition(pos);
          l_PinchosPlatform->SetPitch(pitch);
          l_PinchosPlatform->SetRoll(roll);
          l_PinchosPlatform->SetScale(scale);
          AddResource(platformName, l_PinchosPlatform);
        } else if (type == "poly") {
          Vect3f	l_FinalPosition = m(i).GetVect3fProperty("final_position",  v3fZERO, false);
          float	l_ActivationDistance = m(i).GetFloatProperty("activation_distance", .0f, false);
          // bool	l_InitialCollision = m(i).GetBoolProperty("initial_collision", false, false);
          // std::string	l_RedimAxis = m(i).GetPszISOProperty("redim_axis", "", false);
          Vect3f	l_Direction = m.GetVect3fProperty("direction",  v3fZERO, false);
          CPolyPlatform *l_PolyPlatform = new CPolyPlatform(platformName, core, l_FinalPosition, l_Direction, l_ActivationDistance);
          l_PolyPlatform->setLightName(m(i).GetPszISOProperty("light", "", false));
          l_PolyPlatform->SetYaw(yaw);
          l_PolyPlatform->SetPosition(pos);
          l_PolyPlatform->SetPitch(pitch);
          l_PolyPlatform->SetRoll(roll);
          l_PolyPlatform->SetScale(scale);
          AddResource(platformName, l_PolyPlatform);
        }
      } else if (name == "collectible") {
        std::string meshName = m(i).GetPszISOProperty("name", "", false);
        std::string core = m(i).GetPszISOProperty("core", "", false);
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO, false);
        float yaw = m(i).GetFloatProperty("yaw", false);
        float pitch = m(i).GetFloatProperty("pitch", false);
        float roll = m(i).GetFloatProperty("roll", false);
        Vect3f scale = m(i).GetVect3fProperty("scale", v3fONE, false);
        std::string type = m(i).GetPszISOProperty("type", "", false);
        std::string layer = m(i).GetPszISOProperty("layer", "", false);
        std::string luaCode = m(i).GetPszISOProperty("lua_code", "", false);
        std::string userDataName = m(i).GetPszISOProperty("user_data_name", "", false);
        if (type == "pixelite") {
          CPixelite *l_Pixelite = new CPixelite(core, meshName, layer, userDataName, luaCode);
          l_Pixelite->SetYaw(yaw);
          l_Pixelite->SetPosition(pos);
          l_Pixelite->SetPitch(pitch);
          l_Pixelite->SetRoll(roll);
          l_Pixelite->SetScale(scale);
          AddResource(meshName, l_Pixelite);

        } else if (type == "cromo") {
          CCromo *l_Cromo = new CCromo(core, meshName, layer, userDataName, luaCode);
          l_Cromo->SetYaw(yaw);
          l_Cromo->SetPosition(pos);
          l_Cromo->SetPitch(pitch);
          l_Cromo->SetRoll(roll);
          l_Cromo->SetScale(scale);
          AddResource(meshName, l_Cromo);
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
        std::string l_file = m2(i).GetPszISOProperty("file", "");
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
    //cargamos aqui el animated models para no hacerlo varias veces dentro del bucle
    //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\level1\\animated_models.xml"); //se carga animatedmodels.xml
    //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\animated_models.xml"); //se carga animatedmodels.xml
    //int count = m.GetNumChildren();
    //for (int i = 0; i < count; ++i) {
    std::string name = m.GetName();
    if (name == "collectible") {
      std::string meshName = m.GetPszISOProperty("name", "", false);
      std::string core = m.GetPszISOProperty("core", "", false);
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO, false);
      float yaw = m.GetFloatProperty("yaw", false);
      float pitch = m.GetFloatProperty("pitch", false);
      float roll = m.GetFloatProperty("roll", false);
      Vect3f scale = m.GetVect3fProperty("scale", v3fONE, false);
      std::string type = m.GetPszISOProperty("type", "", false);
      std::string layer = m.GetPszISOProperty("layer", "", false);
      std::string luaCode = m.GetPszISOProperty("lua_code", "", false);
      std::string userDataName = m.GetPszISOProperty("user_data_name", "", false);
      if (type == "pixelite") {
        CPixelite *l_Pixelite = new CPixelite(core, meshName, layer, userDataName, luaCode);
        l_Pixelite->SetYaw(yaw);
        l_Pixelite->SetPosition(pos);
        l_Pixelite->SetPitch(pitch);
        l_Pixelite->SetRoll(roll);
        l_Pixelite->SetScale(scale);
        l_Pixelite->SetActor();
        AddResource(meshName, l_Pixelite);

      } else if (type == "cromo") {
        CCromo *l_Cromo = new CCromo(core, meshName, layer, userDataName, luaCode);
        l_Cromo->SetYaw(yaw);
        l_Cromo->SetPosition(pos);
        l_Cromo->SetPitch(pitch);
        l_Cromo->SetRoll(roll);
        l_Cromo->SetScale(scale);
        l_Cromo->SetActor();
        AddResource(meshName, l_Cromo);
      }

    } else if (name == "mesh_instance") {
      //TODO poder pasar XMLTreeNode
      std::string meshName = m.GetPszISOProperty("name", "box1");
      std::string core = m.GetPszISOProperty("core", "box");
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
      float yaw = m.GetFloatProperty("yaw");
      float pitch = m.GetFloatProperty("pitch");
      float roll = m.GetFloatProperty("roll");
      Vect3f scale = m.GetVect3fProperty("scale", v3fONE);
      //TODO Static mesh por mesh instance hecho?
      //
      CMeshInstance *l_meshInstance = new CMeshInstance(meshName, core);
      l_meshInstance->SetYaw(yaw);
      l_meshInstance->SetPosition(pos);
      l_meshInstance->SetPitch(pitch);
      l_meshInstance->SetRoll(roll);
      l_meshInstance->SetScale(scale);
      //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
      AddResource(meshName, l_meshInstance);
    } else if (name == "animated_model") {
      //FALTA CARGAR EL ANIMATED_MODELS.XML hecho?
      std::string meshName = m.GetPszISOProperty("name", "box1");
      std::string core = m.GetPszISOProperty("core", "box");
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
      float yaw = m.GetFloatProperty("yaw");
      float pitch = m.GetFloatProperty("pitch");
      float roll = m.GetFloatProperty("roll");
      float scale = m.GetFloatProperty("scale");
      CAnimatedInstanceModel *l_AnimatedInstanceModel  = ANIMAN->GetInstance(core);
      l_AnimatedInstanceModel->SetYaw(yaw);
      l_AnimatedInstanceModel->SetPosition(pos);
      l_AnimatedInstanceModel->SetPitch(pitch);
      l_AnimatedInstanceModel->SetRoll(roll);
      l_AnimatedInstanceModel->SetScale(scale);
      //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
      AddResource(meshName, l_AnimatedInstanceModel);
    } else  if (name == "renderable_script") {
      std::string l_name = m.GetPszISOProperty("name", "");
      std::string l_file = m.GetPszISOProperty("file", "");
      if (l_name == "scriptedController") {
        CScriptedController *l_ScriptedController = new CScriptedController();
        AddResource(l_name, l_ScriptedController);
      }
    } else  if (name == "platform") {
      //TODO
      std::string platformName = m.GetPszISOProperty("name", "box1");
      std::string core = m.GetPszISOProperty("core", "box");
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
      float yaw = m.GetFloatProperty("yaw");
      float pitch = m.GetFloatProperty("pitch");
      float roll = m.GetFloatProperty("roll");
      Vect3f scale = m.GetVect3fProperty("scale", v3fONE);
      std::string type = m.GetPszISOProperty("type", "", false);
      //TODO Static mesh por mesh instance
      //
      if (type == "static") {
        CStaticPlatform *l_StaticPlatform = new CStaticPlatform(platformName, core);
        l_StaticPlatform->SetYaw(yaw);
        l_StaticPlatform->SetPosition(pos);
        l_StaticPlatform->SetPitch(pitch);
        l_StaticPlatform->SetRoll(roll);
        l_StaticPlatform->SetScale(scale);
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(platformName, l_StaticPlatform);
        //std::string boxName, std::string coreName, std::string userDataName, Vect3f size, Vect3f localPosition)
      } else if (type == "breakable") {
        std::string l_TriggerName =  m.GetPszISOProperty("trigger_name", "", false);
        CBreakablePlatform *l_BreakablePlatform = new CBreakablePlatform(platformName, core, l_TriggerName);
        l_BreakablePlatform->SetYaw(yaw);
        l_BreakablePlatform->SetPosition(pos);
        l_BreakablePlatform->SetPitch(pitch);
        l_BreakablePlatform->SetRoll(roll);
        l_BreakablePlatform->SetScale(scale);
        AddResource(platformName, l_BreakablePlatform);
      } else if (type == "moving") {
        float l_speed =  m.GetFloatProperty("speed", 0.0f, false);
        int l_WpCount = m.GetNumChildren();
        CMovingPlatform *l_MovingPlatform = new CMovingPlatform(platformName, core, l_speed);
        for (int i = 0; i < l_WpCount; ++i) {
          std::string name = m(i).GetName();
          if (name == "wp") {
            Vect3f l_Wp = m(i).GetVect3fProperty("pos", Vect3f(.0f, .0f, .0f), false);
            l_MovingPlatform->GetWayPointsVector().push_back(l_Wp);
          }
        }
        l_MovingPlatform->SetNextWPVector(l_MovingPlatform->GetWayPointsVector()[1]);
        l_MovingPlatform->SetYaw(yaw);
        l_MovingPlatform->SetPosition(pos);
        l_MovingPlatform->SetPitch(pitch);
        l_MovingPlatform->SetRoll(roll);
        l_MovingPlatform->SetScale(scale);
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(platformName, l_MovingPlatform);
        //std::string boxName, std::string coreName, std::string userDataName, Vect3f size, Vect3f localPosition)
      } else if (type == "pinchos") {
        std::string l_TriggerName =  m.GetPszISOProperty("trigger_name", "", false);
        // float	l_Offset = m.GetFloatProperty("offset", .0f, false);
        //back_position="3.64 0.5 170.0" forth_position ="3.64 0.5 260.0"
        Vect3f l_BackPos = m.GetVect3fProperty("back_position", v3fZERO, false);
        Vect3f l_FrontPos = m.GetVect3fProperty("forth_position", v3fZERO, false);
        bool	l_FromX = m.GetBoolProperty("from_x", false, false);
        bool	l_FromZ = m.GetBoolProperty("from_z", false, false);
        CPinchosPlatform *l_PinchosPlatform = new CPinchosPlatform(platformName, core, l_TriggerName, l_BackPos, l_FrontPos,  l_FromX, l_FromZ);
        l_PinchosPlatform->SetYaw(yaw);
        l_PinchosPlatform->SetPosition(pos);
        l_PinchosPlatform->SetPitch(pitch);
        l_PinchosPlatform->SetRoll(roll);
        l_PinchosPlatform->SetScale(scale);
        AddResource(platformName, l_PinchosPlatform);
      } else if (type == "poly") {
        Vect3f	l_FinalPosition = m.GetVect3fProperty("final_position",  v3fZERO, false);
        Vect3f	l_Direction = m.GetVect3fProperty("direction",  v3fZERO, false);
        float	l_ActivationDistance = m.GetFloatProperty("activation_distance", .0f, false);
        // bool	l_InitialCollision = m.GetBoolProperty("initial_collision", false, false);
        //std::string	l_RedimAxis = m.GetPszISOProperty("redim_axis", "", false);
        CPolyPlatform *l_PolyPlatform = new CPolyPlatform(platformName, core, l_FinalPosition, l_Direction,  l_ActivationDistance);
        l_PolyPlatform->setLightName(m.GetPszISOProperty("light", "", false));
        l_PolyPlatform->SetYaw(yaw);
        l_PolyPlatform->SetPosition(pos);
        l_PolyPlatform->setOriginalPosition(pos);
        l_PolyPlatform->SetPitch(pitch);
        l_PolyPlatform->SetRoll(roll);
        l_PolyPlatform->SetScale(scale);
        AddResource(platformName, l_PolyPlatform);
      }
    }
    //}
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