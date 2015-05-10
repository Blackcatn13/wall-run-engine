#include "Cinematic.h"
#include "CinematicObject.h"
#include "Renderable\RenderableObject.h"
#include "CinematicPlayer.h"
#include "Utils\Logger.h"
#include <vector>
#include "Core_Utils/MemLeaks.h"

CCinematic::CCinematic()
  : CRenderableObject()
  , CCinematicPlayer() {
}

CCinematic::~CCinematic() {
  for (size_t i = 0; i < m_CinematicObjects.size(); ++i) {
    CHECKED_DELETE(m_CinematicObjects[i]);
  }
  m_CinematicObjects.clear();
}

void CCinematic::Stop() {
  m_Playing = false;
  m_CurrentTime = 0.0;
  for (size_t i = 0; i < m_CinematicObjects.size(); ++i) {
    m_CinematicObjects[i]->Stop();
  }
}

void CCinematic::Play(bool Cycle) {
  m_Playing = true;
  m_Cycle = Cycle;
  for (size_t i = 0; i < m_CinematicObjects.size(); ++i) {
    m_CinematicObjects[i]->Play(Cycle);
  }
}

void CCinematic::Pause() {
  m_Playing = false;
  for (size_t i = 0; i < m_CinematicObjects.size(); ++i) {
    m_CinematicObjects[i]->Pause();
  }
}

std::string CCinematic::LoadXML(const std::string &Filename) {
  std::string m_FileName = Filename;
  CXMLTreeNode l_XMLParser;
  if (!l_XMLParser.LoadFile(Filename.c_str())) {
    LOGGER->AddNewLog(ELL_ERROR, "No se ha encontrado el xml de la cinematica %s", Filename.c_str());
  } else {
    CXMLTreeNode  m = l_XMLParser["cinematic"];
    if (m.Exists()) {
      std::string m_Name = m.GetPszProperty("name");
      //SetName(m_Name);
      m_Duration = m.GetFloatProperty("duration");
      Init(m_Duration);
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        if (!strcmp(m(i).GetName(), "cinematic_object")) {
          CCinematicObject *l_cinematicObject = new CCinematicObject(m(i));
          AddCinematicObject(l_cinematicObject);
        }
      }
      return m_Name;
    }
  }
}

void CCinematic::AddCinematicObject(CCinematicObject *CinematicObject) {
  m_CinematicObjects.push_back(CinematicObject);
}

void CCinematic::Update(float ElapsedTime) {
  if (m_Playing == true) {
    for (size_t i = 0; i < m_CinematicObjects.size(); ++i) {
      if (m_CinematicObjects[i]->IsOk() && (m_Playing == true)) {
        m_CinematicObjects[i]->Update(ElapsedTime);
      }
    }
    m_CurrentTime += ElapsedTime;
    if ( m_CurrentTime > m_Duration ) {
      if (m_Cycle == true) {
        m_CurrentTime = 0.0;
      } else {
        m_Playing = false;
        m_CurrentTime = 0.0;
      }
    }
  }
}