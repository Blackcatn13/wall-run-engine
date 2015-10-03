#include "BossEnemy.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Renderable\RenderableObjectsManager.h"


CBossEnemy::CBossEnemy(CXMLTreeNode &info1) : CEnemy(info1)
  , m_Phases(info1.GetIntProperty("phases", 0, false))
  , m_BossRunning(false) {
  if (m_ProjectileName != "")
    SetShootingStats(info1.GetIntProperty("speed_projectile", 0, false), info1.GetIntProperty("projectile_life", 0, false), info1.GetIntProperty("cooldown_timer", 0, false) );
  m_ProjectileHitbox = info1.GetFloatProperty("projectile_size", .0f, false);
  Init(info1.GetPszISOProperty("fsm", "NoFSM", false));
}


CBossEnemy::~CBossEnemy() {
}


void CBossEnemy::Update(float elapsedTime) {
  if (m_BossRunning)
    UpdateFSM(elapsedTime);
}

void CBossEnemy::Render() {
}