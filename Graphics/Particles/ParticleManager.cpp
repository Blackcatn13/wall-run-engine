#include "Particles\ParticleManager.h"
#include "XML\XMLTreeNode.h"
#include "Particles\ParticleEmitter.h"

CParticleManager::CParticleManager()
  : m_fileName ("") {
}

CParticleManager::~CParticleManager() {
}

void CParticleManager::Load(std::string file) {
  m_fileName = file;
  Load();
}

void CParticleManager::Reload() {
  Destroy();
  Load();
}

void CParticleManager::Load() {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(m_fileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["particles"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        std::string l_Name = m(i).GetName();
        if (l_Name == "emitter") {
          CParticleEmitter *newEmitter = new CParticleEmitter(m(i));
          std::string EmitterName = m(i).GetPszISOProperty("name", "");
          AddResource(EmitterName, newEmitter);
        }
      }
    }
  }
}


void CParticleManager::Update(float ElapsedTime) {
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(ElapsedTime);
  }
}

void CParticleManager::Render(CGraphicsManager *RM) {
// int test = m_ResourcesVector.size();
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Render(RM);
  }
}



