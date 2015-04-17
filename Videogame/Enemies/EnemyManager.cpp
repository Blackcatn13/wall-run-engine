#include "EnemyManager.h"
#include "XML\XMLTreeNode.h"
#include "Enemy.h"
#include <string>
#include "BossEnemy.h"
#include "EasyEnemy.h"
#include "PatrolEnemy.h"
#include <map>
#include "Utils\PhysicUserData.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"

CEnemyManager *CEnemyManager::m_Singleton = 0;

CEnemyManager::CEnemyManager() {
}


CEnemyManager::~CEnemyManager() {
}

void CEnemyManager::InitEnemies(std::string layerName) {
  m_LayerName = layerName;
  CRenderableObjectsManager *l_Rom = RENDLM->GetRenderableObjectsManagerByStr(layerName);
  for (int i = 0; i < l_Rom->GetResourcesVector().size(); ++i) {
    for (int j = 0; j < m_EnemyInstances.size(); ++j) {
      if (l_Rom->GetResourcesVector()[i]->getName().find(m_EnemyInstances[j])) {
        CEasyEnemy *Enemy = new CEasyEnemy(l_Rom->GetResourcesVector()[i]);
        m_Enemies.push_back(Enemy);
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
        std::string l_Name = m(i).GetName();
        if (l_Name == "enemy") {
          std::string name = m(i).GetPszProperty("type", "", false);
          if (name == "easy") {
            CEasyEnemy *l_Enemy = new CEasyEnemy(m(i));
            m_Enemies.push_back(l_Enemy);
          } else if (name == "patrol") {
            CPatrolEnemy *l_Enemy = new CPatrolEnemy(m(i));
            m_Enemies.push_back(l_Enemy);
          } else if (name == "boss") {
            CBossEnemy *l_Enemy = new CBossEnemy(m(i));
            m_Enemies.push_back(l_Enemy);
          }
        } else if (l_Name == "core_enemy") {
          std::string type = m(i).GetPszProperty("type", "", false);
          m_Cores[type] = m(i);
        } else if (l_Name == "instance_enemy") {
          std::string instanceEnemy = m(i).GetPszISOProperty("name", "", false);
          m_EnemyInstances.push_back(instanceEnemy);
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
}

void CEnemyManager::Reload() {
  Destroy();
  Init(m_File);
  InitEnemies(m_LayerName);
}

CEnemyManager *CEnemyManager::GetInstance() {
  if (m_Singleton != NULL) {
    return m_Singleton;
  } else {
    m_Singleton = new CEnemyManager();
  }
  return m_Singleton;
}

const CXMLTreeNode &CEnemyManager::GetCoreEnemy(const std::string &type) {
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