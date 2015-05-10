#pragma once
#ifndef INDEXED_VERTEX_H
#define INDEXED_VERTEX_H

#include <d3d9.h>
#include "RenderableVertex\RenderableVertexs.h"
#include "Effects/Effect.h"
#include "Effects/EffectTechnique.h"

template<class T>
class CIndexedVertexs : public CRenderableVertexs {
 protected:
  inline size_t GetVertexSize() {
    return sizeof(T);
  }
  inline size_t GetIndexSize() {
    return sizeof(unsigned short);
  }
 public:
  CIndexedVertexs(CGraphicsManager *RM, void *VertexAddress, void *IndexAddres, size_t VertexCount, size_t IndexCount) {
    RM->GetDevice()->CreateVertexBuffer(GetVertexSize() * VertexCount, 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0);
    RM->GetDevice()->CreateIndexBuffer(GetIndexSize() * IndexCount, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, 0);
    m_IndexCount = IndexCount;
    m_VertexCount = VertexCount;
    void *vb_data;
    void *ib_data;
    m_VB->Lock(0, GetVertexSize()* VertexCount, &vb_data, 0);
    memcpy(vb_data, VertexAddress, GetVertexSize() * VertexCount);
    m_VB->Unlock();
    m_IB->Lock(0, GetIndexSize() * IndexCount, &ib_data, 0);
    memcpy(ib_data, IndexAddres, GetIndexSize() * IndexCount);
    m_IB->Unlock();
  }
  ~CIndexedVertexs() {
  }
  bool Render(CGraphicsManager *RM) {
    HRESULT m_ok = RM->GetDevice()->SetStreamSource(0, m_VB, 0, GetVertexSize());
    m_ok = RM->GetDevice()->SetIndices(m_IB);
    m_ok = RM->GetDevice()->SetFVF(T::GetFVF());
    m_ok = RM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3);
    return m_ok;
  }
  bool Render(CGraphicsManager *GM, CEffectTechnique *EffectTechnique, int baseVertex, int MinVertex, int NumVertex, int StartIndex, int IndexCount) const {
    LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
    UINT l_NumPasses;
    LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
    l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
    EffectTechnique->BeginRender();
    if (SUCCEEDED(l_Effect->Begin(&l_NumPasses, 0))) {
      l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
      l_Device->SetStreamSource(0, m_VB, 0, sizeof(T));
      l_Device->SetIndices(m_IB);
      for (UINT b = 0; b < l_NumPasses; ++b) {
        l_Effect->BeginPass(b);
        l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
                                        baseVertex,
                                        MinVertex,
                                        NumVertex == -1 ? (UINT)m_VertexCount : (UINT)NumVertex,
                                        StartIndex,
                                        IndexCount == -1 ? (UINT) m_IndexCount / 3 : (UINT)IndexCount / 3);
        l_Effect->EndPass();
      }
      l_Effect->End();
    }
    return true;
  }
  virtual inline unsigned short GetVertexType() const {
    return T::GetVertexType();
  }
};

#endif