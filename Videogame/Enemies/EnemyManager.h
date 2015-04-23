#ifndef _ENEMY_MANAGER_H
#define _ENEMY_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include <map>

class CEnemy;
class CXMLTreeNode;

typedef struct EnemyStruct {
  std::string		instanceName;
  std::string		instanceType;

} EnemiesStr;

typedef struct EnemyStats {
  int			NumWp;
  float			DistPlayer;
  float			Speed;
  float			SpeedAttack;
  float			ShootSpeed;
  int			life;
  std::string 	FsmName;

} StatsStr;

class CEnemyManager {
 private:
  std::vector<CEnemy *>						m_Enemies;
  std::string									m_File;
  std::map<std::string, StatsStr>			m_Cores;
  std::vector<EnemiesStr>					m_EnemyInstances;
  std::string									m_LayerName;


 protected:
  static CEnemyManager		*m_Singleton;

 public:
  CEnemyManager();
  ~CEnemyManager();
  void Init(const std::string &FileName);
  void InitEnemies(std::string layerName);
  void Update(float elapsedTime);
  void Render();
  void Destroy();
  void Reload();
  void InsertEnemy(CEnemy *enemy);
  CEnemy *GetEnemy(std::string enemyName);

  static CEnemyManager *GetInstance();
  const EnemyStats &GetCoreEnemy(const std::string &type);
  void SetPaintEnemies(bool paint);
  std::vector<CEnemy *> GetEnemies() {
    return m_Enemies;
  }

};

#endif