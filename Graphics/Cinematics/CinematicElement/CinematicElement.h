#pragma once

#ifndef CINEMATIC_ELEMENT_H
#define CINEMATIC_ELEMENT_H

class CXMLTreeNode;

class CCinematicElement {
 protected:
  bool m_onStart;
  bool m_block;
  bool m_ended;
  bool m_atEnd;

 public:
  CCinematicElement(const CXMLTreeNode &node);
  virtual ~CCinematicElement();
  virtual bool Execute() = 0;
  virtual void restart() {m_ended = false; }
  virtual bool Update(float dt) = 0;
  virtual void Stop() = 0;
  bool atEnd() {return m_atEnd;}
};

#endif