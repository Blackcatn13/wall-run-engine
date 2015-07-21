#include "Renderable\RenderableObjectTechniqueManager.h"
#include "Core\Core.h"
#include "Effects\EffectManager.h"
#include "Renderable\PoolRenderableObjectTechnique.h"
#include "Core_Utils/MemLeaks.h"

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager() {
  //Constructor, construye la clase CRenderableObjectTechniqueManager.
  //XML relacion RenderableObjectManagers con Techniques
}

CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager() {
  //Destructor, destruye la clase CRenderableObjectTechniqueManager llamando al método Destroy.
  Destroy();
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName) {
  CEffectTechnique *l_EffectTechnique = EFFECTM->GetEffectTechnique(TechniqueName);
  CRenderableObjectTechnique *l_RenderableObjectTechnique = new CRenderableObjectTechnique(ROTName, l_EffectTechnique);
  AddResource(ROTName, l_RenderableObjectTechnique);
  //InsertRenderableObjectTechnique, introducirá un
  //CRenderableObjectTechnique en el mapa según un nombre de renderable
  //object technique y el nombre de la technique que utilizará.
}

void CRenderableObjectTechniqueManager::Destroy() {
  //TODO cambiar TMapResource por CRenderableObjectTechnique?
  for (TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it) {
    CHECKED_DELETE(it->second);
  }
  m_Resources.clear();
  m_PoolRenderableObjectTechniques.Destroy();
  //Destroy, destruye todos los CPoolRenderableObjectTechnique y los CRenderableObjectTechnique.
}


void CRenderableObjectTechniqueManager::Load(const std::string &FileName) {
  m_FileName = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  }
  CXMLTreeNode  m = newFile["renderable_object_techniques"];
  int count = m.GetNumChildren();
  for (int i = 0; i < count; ++i) {
    CXMLTreeNode nodeChild = m.getNextChild();
    std::string name = nodeChild.GetName();
    if (name == "pool_renderable_object_technique") {
      std::string l_poolName = nodeChild.GetPszISOProperty("name", "");
      bool l_isDefault = nodeChild.GetBoolProperty("default", false, false);
      if (l_isDefault) {
        int count2 = nodeChild.GetNumChildren();
        for (int j = 0; j < count2; ++j) {
          CXMLTreeNode nodeChild1 = nodeChild.getNextChild();
          std::string name2 = nodeChild1.GetName();
          if (name2 == "default_technique") {
            unsigned int l_VertexNumber = nodeChild1.GetIntProperty("vertex_type", 0, false);
            std::string l_TechniqueName = nodeChild1.GetPszISOProperty("technique", "", false);
            std::string l_RenderableObjectTechniqueName = GetRenderableObjectTechniqueNameByVertexType(l_VertexNumber);
            InsertRenderableObjectTechnique(l_RenderableObjectTechniqueName, l_TechniqueName);
          }
        }
      }
      CPoolRenderableObjectTechnique *l_Pool = new CPoolRenderableObjectTechnique(m(i));
      m_PoolRenderableObjectTechniques.AddResource(l_poolName, l_Pool);
    }
  }
}

CMapManager<CPoolRenderableObjectTechnique> &CRenderableObjectTechniqueManager::GetPoolRenderableObjectTechniques() {
  return m_PoolRenderableObjectTechniques;
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType) {
  //TODO creo que esto esta mal
  std::string l_VertexName = "DefaultROTTechnique_";
  char buff[500];
  sprintf(buff, "%u", VertexType);
  std::string l_VertexType = buff;
  l_VertexName.append(l_VertexType);
  return l_VertexName;
}
void CRenderableObjectTechniqueManager::Reload() {
  Destroy();
  Load(m_FileName);
}