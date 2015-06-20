#pragma once
#ifndef VISIBLE_H
#define VISIBLE_H

#include "XML\XMLTreeNode.h"

class CVisible {
 protected:
  bool m_visible;
 public:
  CVisible() : m_visible(false) {}
  CVisible(const CXMLTreeNode &node) : m_visible(node.GetBoolProperty("visible", true)) {}
  void setVisible(bool visible) {
    m_visible = visible;
  }
  bool getVisible() {
    return m_visible;
  }
};

#endif