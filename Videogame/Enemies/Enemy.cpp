#include "Enemy.h"
#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"

CEnemy::CEnemy(CXMLTreeNode &info1)
    : m_Name(info1.GetPszISOProperty("name"))
    , m_Position(info1.GetVect3fProperty("pos", Vect3f()))
    , m_Yaw(info1.GetFloatProperty("yaw"))
{
    // ask for it to enemyManager
    //m_Life(info2.GetFloatProperty("life"))
    //m_ShootAccuracy(info2.GetFloatProperty("shoot_accuracy"))
    //, m_TimeToShoot(info2.GetFloatProperty("time_to_shoot"))
    //, m_TimeToSpawn(info2.GetFloatProperty("time_to_spawn"))
}


CEnemy::~CEnemy()
{
}
