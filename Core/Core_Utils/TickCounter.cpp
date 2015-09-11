#include "Core_Utils\TickCounter.h"
#include "Core_Utils\MemLeaks.h"

CTickCounter *CTickCounter::m_Instance = 0;

CTickCounter *CTickCounter::getInstance() {
  if (m_Instance == 0)
    m_Instance = new CTickCounter();
  return m_Instance;
}

CTickCounter::CTickCounter() {
}

CTickCounter::~CTickCounter() {
  m_ticks.clear();
}

void CTickCounter::Update(float dt) {
  for (auto it = m_ticks.begin(); it != m_ticks.end(); ++it) {
    it->second.currentValue += it->second.addValue * dt;
    if (it->second.currentValue > it->second.MaxValue)
      it->second.currentValue = 0;
  }
}

void CTickCounter::addTick(const std::string &name, float increment, float max) {
  AuxInfo aux;
  aux.addValue = increment;
  aux.MaxValue = max;
  aux.currentValue = 0;
  m_ticks[name] = aux;
}

float CTickCounter::getValue(const std::string &name) {
  return m_ticks[name].currentValue;
}