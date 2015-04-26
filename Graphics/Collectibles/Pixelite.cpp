#include "Pixelite.h"
#include "Renderable\RenderableObject.h"


//CPixelite::CPixelite(std::string coreName, std::string name, std::string layerName, std::string userDataName, std::string luaCode):
//  CCollectible(coreName, name, layerName, userDataName, luaCode) {
//}

CPixelite::CPixelite(CRenderableObject *RendObj, std::string layerName, std::string MeshLuaCode, std::string TriggerLuaCode):
  CCollectible(RendObj, layerName, MeshLuaCode, TriggerLuaCode)
{}



