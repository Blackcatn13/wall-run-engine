#include "Particles\BillboardManager.h"
#include "XML\XMLTreeNode.h"
#include "Particles\Billboard.h"

CBillboardManager::CBillboardManager()
  : m_fileName ("") {
}

CBillboardManager::~CBillboardManager() {
  Destroy();
}

void CBillboardManager::Load(std::string file) {
  m_fileName = file;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(m_fileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["billboards"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        std::string l_Name = m(i).GetName();
        if (l_Name == "billboard") {
          CBillboard *l_Billboard = new CBillboard(m(i));
          std::string EmitterName = m(i).GetPszISOProperty("name", "");
          AddResource(EmitterName, l_Billboard);
        }
      }
    }
  }


}

void CBillboardManager::Reload() {
  Destroy();
  Load();
}

void CBillboardManager::Load() {

  Load(m_fileName);
}


void CBillboardManager::Update(float ElapsedTime) {
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(ElapsedTime);
  }
}

void CBillboardManager::Render(CGraphicsManager *RM) {
  //int test = m_ResourcesVector.size();
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Render(RM);
  }
}



