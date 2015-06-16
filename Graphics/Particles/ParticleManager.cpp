#include "Particles\ParticleManager.h"
#include "XML\XMLTreeNode.h"
#include "Particles\ParticleEmitter.h"
#ifdef _PARTICLEVIEWER
#include <iostream>
#include "Core_Utils\Timer.h"
#endif

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
#ifdef _PARTICLEVIEWER
  CTimer timer = CTimer(1);
  timer.Update();
  std::cout << "Start update of emitters : " << std::endl;
#endif
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Update(ElapsedTime);
  }
#ifdef _PARTICLEVIEWER
  timer.Update();
  std::cout << " Time to update all the particles " << timer.GetElapsedTime() << " s" << std::endl;
#endif
}

void CParticleManager::Render(CGraphicsManager *RM) {
#ifdef _PARTICLEVIEWER
  CTimer timer = CTimer(1);
  timer.Update();
  std::cout << "Start render of emitters : " << std::endl;
#endif
  for (size_t i = 0; i < m_ResourcesVector.size(); ++i) {
    m_ResourcesVector[i]->Render(RM);
  }
#ifdef _PARTICLEVIEWER
  timer.Update();
  std::cout << " Time to render all the particles " << timer.GetElapsedTime() << " s" << std::endl;
#endif
}



