#include "ParticleProcess.h"

#include "Camera\ThPSCamera.h"
#include "Camera\CameraController.h"
#include "Object\Object3D.h"
#include "Core\Core.h"
#include "RenderableCommands\SceneRendererCommandManager.h"
#include "Language\LanguageManager.h"
#include "GUI\GUIManager.h"


CParticleProcess::CParticleProcess() {

}

CParticleProcess::~CParticleProcess() {
}

void CParticleProcess::Init() {
  m_ObjectThPS = new CObject3D(Vect3f(0, 0, 0), 0, 0, 0);
  m_ThPSCamera = new CThPSCamera(0.1f, 100.f, 45.0f * D3DX_PI / 180.0f, 1.f, m_ObjectThPS, 15);
  m_ThPSCamera->SetTypeCamera(CCamera::TC_ESF);
  CAMCONTM->AddNewCamera("ThPSEDF", m_ThPSCamera);
  CAMCONTM->setActiveCamera(m_ThPSCamera);
}

void CParticleProcess::DeInit() {
}

Vect2i CParticleProcess::RenderDebugInfo(bool render/*, float dt*/) {
  return Vect2i(1, 1);
}

void CParticleProcess::Update(float dt) {
  CAMCONTM->Update(dt);

  if (ACT2IN->DoAction("ChangeCatalan")) {
    LANGM->SetCurrentLanguage("catalan");
  }
  if (ACT2IN->DoAction("ChangeCastellano")) {
    LANGM->SetCurrentLanguage("castellano");
  }
  if (ACT2IN->DoAction("ChangeIngles")) {
    LANGM->SetCurrentLanguage("ingles");
  }
  if (ACT2IN->DoAction("Console")) {
    GUIM->SetConsole();
  }
}

void CParticleProcess::Render() {
  SCENRENDCOMM->Execute(*GRAPHM);
}
