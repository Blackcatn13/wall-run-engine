#pragma once
#ifndef VISIBLE_H
#define VISIBLE_H

class CVisible {
 private:
  bool m_visible;
 public:
  CVisible() : m_visible(false) {}
  void setVisible(bool visible) {
    m_visible = visible;
  }
  bool getVisible() {
    return m_visible;
  }
};

#endif