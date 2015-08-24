#pragma once

#ifndef CINEMATIC_ELEMENT_H
#define CINEMATIC_ELEMENT_H

class CXMLTreeNode;

class CCinematicElement {
 protected:
  bool m_onStart;
  bool m_block;
  bool m_ended;
 public:
  CCinematicElement(const CXMLTreeNode &node);
  virtual ~CCinematicElement();
  virtual bool Execute() = 0;
  virtual void restart() {m_ended = false; }
};

#endif