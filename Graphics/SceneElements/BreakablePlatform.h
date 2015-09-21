#pragma once
#ifndef BREAKABLE_PLATFORM_H
#define BREAKABLE_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "StaticPlatform.h"
#include "Utils\Defines.h"

class CTrigger;
class CXMLTreeNode;

class CBreakablePlatform : public CStaticPlatform {
 protected:
  std::string		m_TriggerName;
  bool				m_Broken;
  Vect3f			m_ActorOriginalPosition;
  Vect3f			m_ActorOriginalRotation;
  bool				m_EmittersSet;

 public:
  CBreakablePlatform(std::string platformName, std::string coreName, std::string triggerName);
  CBreakablePlatform(const CXMLTreeNode &node);

  virtual ~CBreakablePlatform ();
  void DisablePlatform(float dt/*, Vect3f direction*/);
  void EnablePlatform();
  void Update(float ElapsedTime);

  bool IsBroken() {
    return m_Broken;
  }

  GET_SET(bool, EmittersSet);
};

#endif