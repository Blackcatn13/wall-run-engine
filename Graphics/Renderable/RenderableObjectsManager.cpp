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
		bool visible = m(i).GetBoolProperty("visible", true, false);
        //TODO Static mesh por mesh instance
        //
        CMeshInstance *l_meshInstance = new CMeshInstance(meshName, core);
        l_meshInstance->SetYaw(yaw);
        l_meshInstance->SetPosition(pos);
        l_meshInstance->SetPitch(pitch);
        l_meshInstance->SetRoll(roll);
        l_meshInstance->SetScale(scale);
		l_meshInstance->setVisible(visible);
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
        l_AnimatedInstanceModel->setName(meshName);
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(meshName, l_AnimatedInstanceModel);
      } else if (name == "switch_instance") {
        std::string meshName = m(i).GetPszISOProperty("name", "box1");
        std::string core = m(i).GetPszISOProperty("core", "box");
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
        float yaw = m(i).GetFloatProperty("yaw");
        float pitch = m(i).GetFloatProperty("pitch");
        float roll = m(i).GetFloatProperty("roll");
        float scale = m(i).GetFloatProperty("scale");
        CSwitch *l_Switch  = new CSwitch(meshName, core);
        l_Switch->SetYaw(yaw);
        l_Switch->SetPosition(pos);
        l_Switch->SetPitch(pitch);
        l_Switch->SetRoll(roll);
        l_Switch->SetScale(scale);
        std::stringstream ss;
        ss << meshName << "_UserData";
        std::string userDataName = ss.str();
        Vect3f l_ShapeSize = m.GetVect3fProperty("phisic_size", v3fZERO, false);
        l_Switch->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
        AddResource(meshName, l_Switch);
      } else if (name == "door") {
        std::string meshName = m(i).GetPszISOProperty("name", "box1");
        std::string core = m(i).GetPszISOProperty("core", "box");
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
        float yaw = m(i).GetFloatProperty("yaw");
        float pitch = m(i).GetFloatProperty("pitch");
        float roll = m(i).GetFloatProperty("roll");
        float scale = m(i).GetFloatProperty("scale");
        std::string l_LuaFunc = m(i).GetPszISOProperty("lua_function", "", false);
        Vect3f l_FinalPos = m(i).GetVect3fProperty("final_position", v3fZERO, false);
        CDoor *l_Door  = new CDoor(meshName, core, l_LuaFunc, l_FinalPos);
        l_Door->SetYaw(yaw);
        l_Door->SetPosition(pos);
        l_Door->SetPitch(pitch);
        l_Door->SetRoll(roll);
        l_Door->SetScale(scale);
        std::stringstream ss;
        ss << meshName << "_UserData";
        std::string userDataName = ss.str();
        Vect3f l_ShapeSize = m.GetVect3fProperty("phisic_size", v3fZERO, false);
        l_Door->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
        AddResource(meshName, l_Door);

      }  else  if (name == "platform") {
        //TODO
        std::string platformName = m(i).GetPszISOProperty("name", "box1");
        std::string core = m(i).GetPszISOProperty("core", "box");
        Vect3f pos = m(i).GetVect3fProperty("pos", v3fZERO);
        float yaw = m(i).GetFloatProperty("yaw");
        float pitch = m(i).GetFloatProperty("pitch");
        float roll = m(i).GetFloatProperty("roll");
        Vect3f scale = m(i).GetVect3fProperty("scale", v3fONE);
        std::string type = m(i).GetPszISOProperty("type", "", false);
        std::string userDataName = SetUserDataName(platformName);
        Vect3f l_ShapeSize = m.GetVect3fProperty("phisic_size", v3fZERO, false);
        if (type == "static") {
          CStaticPlatform *l_StaticPlatform = new CStaticPlatform(platformName, core);
          l_StaticPlatform->SetYaw(yaw);
          l_StaticPlatform->SetPosition(pos);
          l_StaticPlatform->SetPitch(pitch);
          l_StaticPlatform->SetRoll(roll);
          l_StaticPlatform->SetScale(scale);
          //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
          l_StaticPlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
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
          l_BreakablePlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
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
          l_MovingPlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
          //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
          AddResource(platformName, l_MovingPlatform);
        } else if (type == "pinchos") {
          std::string l_TriggerName =  m(i).GetPszISOProperty("trigger_name", "", false);
          /* Vect3f l_BackPos = m(i).GetVect3fProperty("back_position", v3fZERO, false);
           Vect3f l_FrontPos = m(i).GetVect3fProperty("forth_position", v3fZERO, false);
           bool	l_FromX = m(i).GetBoolProperty("from_x", false, false);
           bool	l_FromZ = m(i).GetBoolProperty("from_z", false, false);*/
          CPinchosPlatform *l_PinchosPlatform = new CPinchosPlatform(platformName, core, l_TriggerName/*, l_BackPos, l_FrontPos,  l_FromX, l_FromZ*/);
          l_PinchosPlatform->SetYaw(yaw);
          l_PinchosPlatform->SetPosition(pos);
          l_PinchosPlatform->SetPitch(pitch);
          l_PinchosPlatform->SetRoll(roll);
          l_PinchosPlatform->SetScale(scale);
          l_PinchosPlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
          AddResource(platformName, l_PinchosPlatform);
        } else if (type == "poly") {
          Vect3f	l_FinalPosition = m(i).GetVect3fProperty("final_position",  v3fZERO, false);
          float	l_ActivationDistance = m(i).GetFloatProperty("activation_distance", .0f, false);
          // bool	l_InitialCollision = m(i).GetBoolProperty("initial_collision", false, false);
          // std::string	l_RedimAxis = m(i).GetPszISOProperty("redim_axis", "", false);
          Vect3f	l_Direction = m.GetVect3fProperty("direction",  v3fZERO, false);
          float	l_timeOut = m.GetFloatProperty("time_out", .0f, false);
          float	l_speed = m.GetFloatProperty("speed", .0f, false);
          CPolyPlatform *l_PolyPlatform = new CPolyPlatform(platformName, core, l_FinalPosition, l_Direction, l_ActivationDistance, l_timeOut, l_speed);
          l_PolyPlatform->setLightName(m(i).GetPszISOProperty("light", "", false));
          l_PolyPlatform->SetYaw(yaw);
          l_PolyPlatform->SetPosition(pos);
          l_PolyPlatform->SetPitch(pitch);
          l_PolyPlatform->SetRoll(roll);
          l_PolyPlatform->SetScale(scale);
          l_PolyPlatform->InitPolyPlatform(l_ShapeSize);
          AddResource(platformName, l_PolyPlatform);
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
    //cargamos aqui el animated models para no hacerlo varias veces dentro del bucle
    //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\level1\\animated_models.xml"); //se carga animatedmodels.xml
    //CCore::GetInstance()->GetAnimatedModelManager()->Load(".\\Data\\animated_models.xml"); //se carga animatedmodels.xml
    //int count = m.GetNumChildren();
    //for (int i = 0; i < count; ++i) {
    std::string name = m.GetName();
    if (name == "mesh_instance") {
      //TODO poder pasar XMLTreeNode
      std::string meshName = m.GetPszISOProperty("name", "box1");
      std::string core = m.GetPszISOProperty("core", "box");
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
      float yaw = m.GetFloatProperty("yaw");
      float pitch = m.GetFloatProperty("pitch");
      float roll = m.GetFloatProperty("roll");
      Vect3f scale = m.GetVect3fProperty("scale", v3fONE);
	  bool visible = m.GetBoolProperty("visible", true, false);
      //TODO Static mesh por mesh instance hecho?
      //
      CMeshInstance *l_meshInstance = new CMeshInstance(meshName, core);
      l_meshInstance->SetYaw(yaw);
      l_meshInstance->SetPosition(pos);
      l_meshInstance->SetPitch(pitch);
      l_meshInstance->SetRoll(roll);
      l_meshInstance->SetScale(scale);
	  l_meshInstance->setVisible(visible);
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
      l_AnimatedInstanceModel->setName(meshName);
      //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
      AddResource(meshName, l_AnimatedInstanceModel);
    } else if (name == "switch_instance") {
      std::string meshName = m.GetPszISOProperty("name", "box1");
      std::string core = m.GetPszISOProperty("core", "box");
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
      float yaw = m.GetFloatProperty("yaw");
      float pitch = m.GetFloatProperty("pitch");
      float roll = m.GetFloatProperty("roll");
      float scale = m.GetFloatProperty("scale");

      CSwitch *l_Switch  = new CSwitch(meshName, core);
      l_Switch->SetYaw(yaw);
      l_Switch->SetPosition(pos);
      l_Switch->SetPitch(pitch);
      l_Switch->SetRoll(roll);
      l_Switch->SetScale(scale);

      std::string userDataName = SetUserDataName(meshName);
      Vect3f l_ShapeSize = m.GetVect3fProperty("phisic_size", v3fZERO, false);
      l_Switch->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
      AddResource(meshName, l_Switch);

    } else if (name == "door") {
      std::string meshName = m.GetPszISOProperty("name", "box1");
      std::string core = m.GetPszISOProperty("core", "box");
      Vect3f pos = m.GetVect3fProperty("pos", v3fZERO);
      float yaw = m.GetFloatProperty("yaw");
      float pitch = m.GetFloatProperty("pitch");
      float roll = m.GetFloatProperty("roll");
      float scale = m.GetFloatProperty("scale");
      std::string l_LuaFunc = m.GetPszISOProperty("lua_function", "", false);
      Vect3f l_FinalPos = m.GetVect3fProperty("final_position", v3fZERO, false);
      CDoor *l_Door  = new CDoor(meshName, core, l_LuaFunc, l_FinalPos);
      l_Door->SetYaw(yaw);
      l_Door->SetPosition(pos);
      l_Door->SetPitch(pitch);
      l_Door->SetRoll(roll);
      l_Door->SetScale(scale);
      std::string userDataName = SetUserDataName(meshName);
      Vect3f l_ShapeSize = m.GetVect3fProperty("phisic_size", v3fZERO, false);
      l_Door->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));

      AddResource(meshName, l_Door);

    } else  if (name == "renderable_script") {
      std::string l_name = m.GetPszISOProperty("name", "");
      //std::string l_file = m.GetPszISOProperty("file", "");
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
      std::string userDataName = SetUserDataName(platformName);
      Vect3f l_ShapeSize = m.GetVect3fProperty("phisic_size", v3fZERO, false);
      if (type == "static") {
        CStaticPlatform *l_StaticPlatform = new CStaticPlatform(platformName, core);
        l_StaticPlatform->SetYaw(yaw);
        l_StaticPlatform->SetPosition(pos);
        l_StaticPlatform->SetPitch(pitch);
        l_StaticPlatform->SetRoll(roll);
        l_StaticPlatform->SetScale(scale);
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(platformName, l_StaticPlatform);
        l_StaticPlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
        //std::string boxName, std::string coreName, std::string userDataName, Vect3f size, Vect3f localPosition)
      } else if (type == "breakable") {
        std::string l_TriggerName =  m.GetPszISOProperty("trigger_name", "", false);
        CBreakablePlatform *l_BreakablePlatform = new CBreakablePlatform(platformName, core, l_TriggerName);
        l_BreakablePlatform->SetYaw(yaw);
        l_BreakablePlatform->SetPosition(pos);
        l_BreakablePlatform->SetPitch(pitch);
        l_BreakablePlatform->SetRoll(roll);
        l_BreakablePlatform->SetScale(scale);
        l_BreakablePlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
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
        l_MovingPlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
        l_MovingPlatform->setRoom( m.GetPszISOProperty("room", "0", false));
        //CMeshInstance* l_meshInstance = new CMeshInstance(m(i));
        AddResource(platformName, l_MovingPlatform);
        //std::string boxName, std::string coreName, std::string userDataName, Vect3f size, Vect3f localPosition)
      } else if (type == "pinchos") {
        std::string l_TriggerName =  m.GetPszISOProperty("trigger_name", "", false);
        // float	l_Offset = m.GetFloatProperty("offset", .0f, false);
        //back_position="3.64 0.5 170.0" forth_position ="3.64 0.5 260.0"
        Vect3f l_BackPos = m.GetVect3fProperty("back_position", v3fZERO, false);
        Vect3f l_FrontPos = m.GetVect3fProperty("forth_position", v3fZERO, false);
        //bool	l_FromX = m.GetBoolProperty("from_x", false, false);
        //bool	l_FromZ = m.GetBoolProperty("from_z", false, false);
        CPinchosPlatform *l_PinchosPlatform = new CPinchosPlatform(platformName, core, l_TriggerName/*, l_BackPos, l_FrontPos,  l_FromX, l_FromZ*/);
        l_PinchosPlatform->SetYaw(yaw);
        l_PinchosPlatform->SetPosition(pos);
        l_PinchosPlatform->SetPitch(pitch);
        l_PinchosPlatform->SetRoll(roll);
        l_PinchosPlatform->SetScale(scale);
        l_PinchosPlatform->InsertPhisic(userDataName, l_ShapeSize, Vect3f(0.0f, l_ShapeSize.y, 0.0f));
        AddResource(platformName, l_PinchosPlatform);
      } else if (type == "poly") {
        Vect3f	l_FinalPosition = m.GetVect3fProperty("final_position",  v3fZERO, false);
        Vect3f	l_Direction = m.GetVect3fProperty("direction",  v3fZERO, false);
        float	l_ActivationDistance = m.GetFloatProperty("activation_distance", .0f, false);
        float	l_timeOut = m.GetFloatProperty("time_out", .0f, false);
        float	l_speed = m.GetFloatProperty("speed", .0f, false);
        // bool	l_InitialCollision = m.GetBoolProperty("initial_collision", false, false);
        //std::string	l_RedimAxis = m.GetPszISOProperty("redim_axis", "", false);
        CPolyPlatform *l_PolyPlatform = new CPolyPlatform(platformName, core, l_FinalPosition, l_Direction,  l_ActivationDistance, l_timeOut, l_speed);
        l_PolyPlatform->setLightName(m.GetPszISOProperty("light", "", false));
        l_PolyPlatform->SetYaw(yaw);
        l_PolyPlatform->SetPosition(pos);
        l_PolyPlatform->setOriginalPosition(pos);
        l_PolyPlatform->SetPitch(pitch);
        l_PolyPlatform->SetRoll(roll);
        l_PolyPlatform->SetScale(scale);
        l_PolyPlatform->InitPolyPlatform(l_ShapeSize);
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