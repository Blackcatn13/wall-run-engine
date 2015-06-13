#include "EnemyManager.h"
#include "XML\XMLTreeNode.h"
#include "Enemy.h"
#include <string>
#include "BossEnemy.h"
#include "EasyEnemy.h"
#include "PatrolEnemy.h"
#include "RunnerEnemy.h"
#include <map>
#include "Utils\PhysicUserData.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "Core\ScriptManager.h"

CEnemyManager *CEnemyManager::m_Singleton = 0;

CEnemyManager::CEnemyManager() {
}


CEnemyManager::~CEnemyManager() {
  Destroy();
}

void CEnemyManager::InitEnemies(std::string layerName) {
  m_LayerName = layerName;
  CRenderableObjectsManager *l_Rom = RENDLM->GetRenderableObjectsManagerByStr(layerName);
  for (int i = 0; i < l_Rom->GetResourcesVector().size(); ++i) {
    for (int j = 0; j < m_EnemyInstances.size(); ++j) {
      if (l_Rom->GetResourcesVector()[i]->getName() == m_EnemyInstances[j].instanceName) {
        std::string nam = m_Cores.find(m_EnemyInstances[j].instanceType)->first;
        float l_Speed = m_Cores.find(m_EnemyInstances[j].instanceType)->second.Speed;
        std::string l_FsmName = m_Cores.find(m_EnemyInstances[j].instanceType)->second.FsmName;
        Vect2f l_ControllerSize = m_Cores.find(m_EnemyInstances[j].instanceType)->second.ControllerSize;
		float l_Zone = m_EnemyInstances[j].Zone;
        float l_AttackDistance = m_EnemyInstances[j].AttackDistance;
        if ((nam.find("_easy") != std::string::npos)) {
          float l_AttackSpeed = m_Cores.find(m_EnemyInstances[j].instanceType)->second.SpeedAttack;
          int l_Life = m_Cores.find(m_EnemyInstances[j].instanceType)->second.life;
          float l_TurnSpeed = m_Cores.find(m_EnemyInstances[j].instanceType)->second.TurnSpeed;       
		  std::vector<Vect3f> l_WayPoints = m_EnemyInstances[j].waypoints;
		  bool isStatic =  m_EnemyInstances[j].Static;
		  CEasyEnemy *Enemy = new CEasyEnemy(l_Rom->GetResourcesVector()[i], l_WayPoints, l_Speed, l_TurnSpeed, l_AttackSpeed, l_Life, l_FsmName, l_ControllerSize, l_AttackDistance, l_Zone, isStatic);
          m_Enemies.push_back(Enemy);
        } else if (nam.find("_runner") != std::string::npos) {
          CRunnerEnemy *Enemy = new CRunnerEnemy(l_Rom->GetResourcesVector()[i], l_Speed, l_FsmName, l_ControllerSize, l_AttackDistance, l_Zone);
          m_Enemies.push_back(Enemy);
        }
      }
    }
  }

}

void CEnemyManager::Init(const std::string &FileName) {
  m_File = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["enemies"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m(i);
        std::string l_Name = nodeChild.GetName();
        if (l_Name == "enemy") {
          std::string name = nodeChild.GetPszProperty("type", "", false);
          if (name == "easy") {
            CEasyEnemy *l_Enemy = new CEasyEnemy(nodeChild);
            m_Enemies.push_back(l_Enemy);
          } else if (name == "patrol") {
            CPatrolEnemy *l_Enemy = new CPatrolEnemy(nodeChild);
            m_Enemies.push_back(l_Enemy);
          } else if (name == "boss") {
            CBossEnemy *l_Enemy = new CBossEnemy(nodeChild);
            m_Enemies.push_back(l_Enemy);
          }
        } else if (l_Name == "core_enemy") {
          std::string type = nodeChild.GetPszProperty("type", "", false);
          StatsStr l_Stats;
          l_Stats.life = nodeChild.GetIntProperty("life", 1 , false);
          l_Stats.ShootSpeed = nodeChild.GetFloatProperty("shoot_speed", 0.0f , false);
          l_Stats.Speed = nodeChild.GetFloatProperty("speed", 0.0f , false);
          l_Stats.SpeedAttack = nodeChild.GetFloatProperty("speed_attack", 0.0f , false);
          l_Stats.FsmName = nodeChild.GetPszISOProperty("fsm", "NoFSM", false);
          l_Stats.ControllerSize = nodeChild.GetVect2fProperty("controller_size", v2fZERO, false);
          l_Stats.TurnSpeed = nodeChild.GetFloatProperty("turn_speed", 0.2f , false);
          m_Cores.insert(std::pair<std::string, StatsStr>(type, l_Stats));
        } else if (l_Name == "instance_enemy") {
          EnemiesStr l_Enemy;
          l_Enemy.instanceName = nodeChild.GetPszISOProperty("name", "", false);
          l_Enemy.instanceType = nodeChild.GetPszISOProperty("type", "", false);
          int instanceNumChilds = nodeChild.GetNumChildren();
          for (int j = 0; j < instanceNumChilds; ++j) {
            l_Enemy.waypoints.push_back(nodeChild(j).GetVect3fProperty("position", v3fZERO, false));
          }
          l_Enemy.AttackDistance = nodeChild.GetFloatProperty("attack_distance", 15.0f, false);
		  l_Enemy.Zone = nodeChild.GetFloatProperty("zone", 1.0f, false);
		  l_Enemy.Static = nodeChild.GetFloatProperty("static", false, false);
          m_EnemyInstances.push_back(l_Enemy);
        }
      }
    }
  }

}

void CEnemyManager::Update(float elapsedTime) {
  for (std::vector<CEnemy *>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it) {
    (*it)->Update(elapsedTime);
  }
}

void CEnemyManager::Render() {
  for (std::vector<CEnemy *>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it) {
    (*it)->Render();
  }
}

void CEnemyManager::Destroy() {
  for (std::vector<CEnemy *>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it) {
    CHECKED_DELETE(*it);
  }
  m_Enemies.clear();
  m_EnemyInstances.clear();
}

void CEnemyManager::Reload() {
  Destroy();
  Init(m_File);
  InitEnemies(m_LayerName);
}

void CEnemyManager::ReloadEnemies(){
	for (int num = 0; num < m_Enemies.size(); num++){
		if (!m_Enemies[num]->getisAlive()){
			m_Enemies[num]->setisAlive(true);
			m_Enemies[num]->getRenderableObject()->setPrintable(true);
			m_Enemies[num]->getRenderableObject()->setVisible(true);
			std::stringstream ss;
			ss << "mikmik_set_alive(\"" <<m_Enemies[num]->getName()<<"\")";
			std::string str = ss.str();
			CCORE->GetScriptManager()->RunCode(str);
		}
	}
}

CEnemyManager *CEnemyManager::GetInstance() {
  if (m_Singleton != NULL) {
    return m_Singleton;
  } else {
    m_Singleton = new CEnemyManager();
  }
  return m_Singleton;
}

const EnemyStats &CEnemyManager::GetCoreEnemy(const std::string &type) {
  return m_Cores[type];
}


void CEnemyManager::SetPaintEnemies(bool paint) {
  for (int i = 0; i < m_Enemies.size(); ++i) {
    m_Enemies[i]->getPhysicUserData()->SetPaint(paint);
  }
}

void CEnemyManager::InsertEnemy(CEnemy *enemy) {

  m_Enemies.push_back(enemy);
}

CEnemy *CEnemyManager::GetEnemy(std::string enemyName) {
  for (int i = 0; i < m_Enemies.size(); ++ i) {
    if (m_Enemies[i]->getName() == enemyName)
      return m_Enemies[i];
  }
  return NULL;
}