#pragma once
#ifndef PINCHOS_PLATFORM_H
#define PINCHOS_PLATFORM_H

#include <string>
#include "Math\Vector3.h"
#include "BreakablePlatform.h"
#include "Utils\Defines.h"


class CTrigger;
class CPinchosPlatform : public CBreakablePlatform {
 protected:

  // std::string		m_TriggerName;
  //float	m_Offset;
  /* Vect3f	m_BackPos;
   Vect3f	m_FrontPos;
   bool	m_FromZ;
   bool	m_FromX;*/
 public:

  CPinchosPlatform(std::string platformName, std::string coreName, std::string triggerName/*, Vect3f backPos, Vect3f frontPos, bool fromX, bool fromZ*/);
  ~ CPinchosPlatform ();
  void FallingIntoPlatform();
  /* GET_SET(Vect3f, BackPos)
   GET_SET(Vect3f, FrontPos)
   GET_SET(bool, FromZ)
   GET_SET(bool, FromX)*/

};

#endif