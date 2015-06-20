#ifndef COLLECTIBLE	MANAGER_H
#define COLLECTIBLE	MANAGER_H

#include "Utils\Defines.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Collectible.h"
#include <string>
#include <vector>

class CCollectible;
class CXMLTreeNode;

typedef struct CollectibleTypes {
  std::string		CoreMesh;
  std::string		MeshLuaFunction;
  std::string		TriggerFunction;
  Vect3f			TriggerSize;
  std::string		Name;

} StrCollectibles;


class CCollectibleManager : public CTemplatedVectorMapManager<CCollectible> {

 private:
  std::string									m_FileName;
  std::vector<StrCollectibles>					m_VectorCollectibleTypes;
  std::string									m_Layer;
  std::map<std::string, std::string>			m_Unlockables;
 public:
  CCollectibleManager();
  ~CCollectibleManager();

  bool Load(std::string &FileName);
  void Load();
  void Reload();
  void Update(float dt);
  void DeInit();

  void ResetCollectibles();

  void InitCollectibles(std::string layerName);

  std::vector<StrCollectibles> &GetCollectibleTypes() {
    return m_VectorCollectibleTypes;
  }

  std::string GetCollectiblesLayerName() {
    return m_Layer;
  }


};

#endif