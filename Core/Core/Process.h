#ifndef CPROCESS_H
#define CPROCESS_H

#include "Math\Vector2.h"
#include "Utils\Defines.h"
#include <string>

class CCamera;


class CProcess {
 protected:
  CCamera	*m_Camera;
  bool        m_printInfo;
  float		m_Dt;
  Vect2i              m_textPosition;
  bool	m_PaintPhisicActors;
  bool	m_PaintPhisicTriggers;
  bool	m_PaintPhisics;
  bool	m_DebugPhisics;
 public:
  CProcess(void);
  virtual ~CProcess(void);
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;
  virtual void Init();
  Vect2i WriteBoolInfo(bool varInfo, std::string text, Vect2i pos);

  void DeInit();
  CCamera *GetCamera() {
    return m_Camera;
  }
  virtual Vect2i RenderDebugInfo(bool render/*, float dt*/);
  void SetPrintInfo(bool print) {
    m_printInfo = print;
  }
  bool getPrintInfo() {
    return m_printInfo;
  }
  float GetDt() {
    return m_Dt;
  }
  GET_SET(bool, PaintPhisicActors )
  GET_SET(bool, PaintPhisicTriggers )
  GET_SET(bool, PaintPhisics )
  GET_SET(bool, DebugPhisics )
};


#endif
