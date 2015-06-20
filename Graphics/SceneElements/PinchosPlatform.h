#pragma once
#ifndef PINCHOS_PLATFORM_H
#define PINCHOS_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "BreakablePlatform.h"
#include "Utils\Defines.h"

class CXMLTreeNode;
class CTrigger;

class CPinchosPlatform : public CBreakablePlatform {
 public:
  CPinchosPlatform(std::string platformName, std::string coreName, std::string triggerName/*, Vect3f backPos, Vect3f frontPos, bool fromX, bool fromZ*/);
  CPinchosPlatform(const CXMLTreeNode &node);
  virtual ~CPinchosPlatform();
  void FallingIntoPlatform();
};

#endif