#pragma once
#ifndef CRENDERABLE_VERTEX_H
#define CRENDERABLE_VERTEX_H

#include <d3d9.h>
#include "../GraphicsManager.h"

class CEffectTechnique;

class CRenderableVertexs {
 protected:
  LPDIRECT3DVERTEXBUFFER9 m_VB;
  LPDIRECT3DINDEXBUFFER9 m_IB;
  size_t m_IndexCount, m_VertexCount;
 public:
  CRenderableVertexs() : m_IndexCount(0), m_VertexCount(0) {
  }
  virtual ~CRenderableVertexs() {
    CHECKED_RELEASE(m_VB);
    CHECKED_RELEASE(m_IB);
  }
  virtual bool Render(CGraphicsManager *RM) = 0;
  virtual bool Render(CGraphicsManager *RM, CEffectTechnique *EffectTechnique, int = 0, int = 0, int = -1, int = 0, int = -1) const = 0;

  virtual inline size_t GetFacesCount() const {
    return m_IndexCount / 3;
  }
  virtual inline size_t GetVertexsCount() const {
    return m_VertexCount;
  }
  virtual inline unsigned short GetVertexType() const = 0;
  virtual inline size_t GetVertexSize() = 0;
  virtual inline size_t GetIndexSize() = 0;
};

#endif