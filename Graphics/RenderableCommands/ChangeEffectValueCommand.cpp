#include "RenderableCommands\ChangeEffectValueCommand.h"
#include "GraphicsManager.h"
#include "Core_Utils/MemLeaks.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "Effects\EffectManager.h"

CChangeEffectValueCommand::CChangeEffectValueCommand(CXMLTreeNode &atts)
  : CSceneRendererCommand(atts)
  , m_value(atts.GetPszISOProperty("value", "", false))
  , m_time(atts.GetFloatProperty("time"))
  , m_technique(atts.GetPszISOProperty("technique", "", false))
  , m_currentTime(0.f)
  , m_lastValue(false) {
  m_effectTechnique = EFFECTM->GetEffectTechnique(m_technique);
}


CChangeEffectValueCommand::~CChangeEffectValueCommand() {
}

void CChangeEffectValueCommand::Execute(CGraphicsManager &RM) {
  m_currentTime += CCORE->GetElapsedTime();
  if (m_currentTime >= m_time) {
    m_effectTechnique->setChangeUV(m_lastValue);
    m_lastValue = !m_lastValue;
    m_currentTime = 0.f;
  }
}
