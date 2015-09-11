//----------------------------------------------------------------------------------
// CTimer class
// Author: Enric Vergara
#pragma once
#ifndef TICK_COUNTER_H_
#define TICK_COUNTER_H_

#include "Utils\Types.h"
#include <string>
#include <map>

class AuxInfo {
 public:
  float addValue;
  float MaxValue;
  float currentValue;
};

class CTickCounter {
 public:
  static CTickCounter *getInstance();
  void addTick(const std::string &name, float addValue, float MaxValue);
  ~CTickCounter();
  void Update(float elapsedTime);
  CTickCounter();
  float getValue(const std::string &name);
 private:
  static CTickCounter *m_Instance;
  std::map<std::string, AuxInfo> m_ticks;
};

#endif //TICK_COUNTER_H_