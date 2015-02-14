#ifndef _ENEMY_MANAGER_H
#define _ENEMY_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include <map>

class CEnemy;
class CXMLTreeNode;

class CEnemyManager
{
private:
    std::vector<CEnemy *>						m_Enemies;
    std::string									m_File;
    std::map<std::string, CXMLTreeNode>			m_Cores;

    CEnemyManager();
protected:
    static CEnemyManager*		m_Singleton;

public:
    ~CEnemyManager();
    void Init(const std::string &FileName);
    void Update(float elapsedTime);
    void Render();
    void Destroy();
    void Reload();
    static CEnemyManager* GetInstance();
    const CXMLTreeNode & GetCoreEnemy(const std::string &type);
};

#endif