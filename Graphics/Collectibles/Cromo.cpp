#include "Cromo.h"


CCromo::CCromo(CRenderableObject *RendObj, std::string layerName, std::string MeshLuaCode, std::string TriggerLuaCode):
  CCollectible(RendObj, layerName, MeshLuaCode, TriggerLuaCode) {
}
