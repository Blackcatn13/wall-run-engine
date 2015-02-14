#include "EnemyManager.h"
#include "XML\XMLTreeNode.h"
#include "Enemy.h"
#include <string>
#include "BossEnemy.h"
#include "EasyEnemy.h"
#include "PatrolEnemy.h"
#include <map>

CEnemyManager* CEnemyManager::m_Singleton = 0;

CEnemyManager::CEnemyManager()
{
}


CEnemyManager::~CEnemyManager()
{
}



void CEnemyManager::Init(const std::string &FileName)
{
    m_File = FileName;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(FileName.c_str())) {
        printf("ERROR loading the file.");
    }
    CXMLTreeNode  m = newFile["enemies"];
    if (m.Exists()) {
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i) {
            if (m(i).GetName() == "enemy") {
                std::string name = m(i).GetPszISOProperty("type");
                if (name == "easy")
                    m_Enemies.push_back(new CEasyEnemy(m(i)));
                else if (name == "patrol")
                    m_Enemies.push_back(new CPatrolEnemy(m(i)));
                else if (name == "boss")
                    m_Enemies.push_back(new CBossEnemy(m(i)));
            } else if (m(i).GetName() == "core_enemy") {
                std::string type = m(i).GetPszISOProperty("type");
                m_Cores[type] = m(i);
            }
        }
    }
}

void CEnemyManager::Update(float elapsedTime)
{
    for (std::vector<CEnemy *>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it) {
        (*it)->Update(elapsedTime);
    }
}

void CEnemyManager::Render()
{
    for (std::vector<CEnemy *>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it) {
        (*it)->Render();
    }
}

void CEnemyManager::Destroy()
{
    for (std::vector<CEnemy *>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it) {
        delete(&it);
    }
    m_Enemies.clear();
}

void CEnemyManager::Reload()
{
    Destroy();
    Init(m_File);
}

CEnemyManager* CEnemyManager::GetInstance()
{
    if (m_Singleton != NULL) {
        return m_Singleton;
    } else {
        m_Singleton = new CEnemyManager();
    }
    return m_Singleton;
}

const CXMLTreeNode & CEnemyManager::GetCoreEnemy(const std::string &type)
{
    return m_Cores[type];
}
