#pragma once
#ifndef INDEXED_VERTEX_H
#define INDEXED_VERTEX_H

#include <d3d9.h>
#include "RenderableVertex\RenderableVertexs.h"

template<class T>
class CIndexedVertexs : public CRenderableVertexs
{
protected:
    inline size_t GetVertexSize()
    {
        return sizeof(T);
    }
    inline size_t GetIndexSize()
    {
        return sizeof(unsigned short);
    }
public:
    CIndexedVertexs(CGraphicsManager *RM, void *VertexAddress, void *IndexAddres, size_t VertexCount, size_t IndexCount)
    {
        HRESULT m_ok = RM->GetDevice()->CreateVertexBuffer(GetVertexSize() * VertexCount, 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0);
        m_ok = RM->GetDevice()->CreateIndexBuffer(GetIndexSize() * IndexCount, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, 0);
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
    ~CIndexedVertexs()
    {
    }
    bool Render(CGraphicsManager *RM)
    {
        HRESULT m_ok = RM->GetDevice()->SetStreamSource(0, m_VB, 0, GetVertexSize());
        m_ok = RM->GetDevice()->SetIndices(m_IB);
        m_ok = RM->GetDevice()->SetFVF(T::GetFVF());
        m_ok = RM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3);
        return true;
    }
    virtual inline unsigned short GetVertexType() const
    {
        return T::GetVertexType();
    }
};

#endif