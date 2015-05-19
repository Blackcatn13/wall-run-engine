#include "Process.h"
#include "Core.h"
#include "Font/FontManager.h"
#include "Core_Utils/MemLeaks.h"

CProcess::CProcess(void) {
}


CProcess::~CProcess(void) {
}

void CProcess::Init() {
}

void CProcess::DeInit() {
}

Vect2i CProcess::RenderDebugInfo(bool render/*, float dt*/) {
  Vect2i pos = Vect2i(1030, 30);//10, 30);
  if (!render) {
    //  pos.Add_Max(FONTM->GetDefaultTextSize(pos.x, pos.y, colWHITE, "Info from %s", __FILE__));
    pos.Add_Max(FONTM->GetDefaultTextSize(pos.x, pos.y, colWHITE, "%f FPS", 1 / m_Dt));
  } else {
    // pos.Add_Max(FONTM->DrawDefaultText(pos.x, pos.y, colWHITE, "Info from %s", __FILE__));
    pos.Add_Max(FONTM->DrawDefaultText(pos.x, pos.y, colWHITE, "%f FPS", 1 / m_Dt));
    pos = WriteBoolInfo(m_DebugPhisics, "Debug Phisics: ", pos);
    pos = WriteBoolInfo(m_PaintPhisicActors, "Draw Phisic actors: ", pos);
    pos = WriteBoolInfo(m_PaintPhisicTriggers, "Draw Triggers: ", pos);
    pos = WriteBoolInfo(m_PaintPhisics, "Draw all Phisics: ", pos);
    pos = WriteStrInfo(CAMCONTM->GetCameraName(CAMCONTM->getActiveCamera()), "Active Camera: ", pos);
  }
  return pos;
}

Vect2i CProcess::WriteBoolInfo(bool varInfo, std::string text, Vect2i pos) {
// std::string l_text = "Debug Phisics: ";
  int l_aux_i = pos.y;
  pos.Add_Max(FONTM->DrawDefaultText(pos.x, pos.y, colWHITE, text.c_str()));
  if (!varInfo)
    FONTM->DrawDefaultText(pos.x+m_aux_x, l_aux_i, colRED, "%s", "false");
  else
    FONTM->DrawDefaultText(pos.x+m_aux_x, l_aux_i, colBLUE, "%s", "true");

  return pos;
}

Vect2i CProcess::WriteStrInfo(std::string varInfo, std::string text, Vect2i pos) {
  int l_aux_i = pos.y;
  pos.Add_Max(FONTM->DrawDefaultText(pos.x, pos.y, colWHITE, text.c_str()));
  FONTM->DrawDefaultText(pos.x+m_aux_x, l_aux_i, colYELLOW, "%s", varInfo.c_str());

  return pos;
}