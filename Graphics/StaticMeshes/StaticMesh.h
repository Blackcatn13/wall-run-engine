#pragma once
#ifndef _STATIC_MESH_H
#define _STATIC_MESH_H

#include "Utils\Defines.h"
#include <string>
#include <vector>

class CTexture;
class CRenderableVertexs;
class CGraphicsManager;

class BoundingBox
{
private:
	Vect3f m_minPos, m_maxPos;
public:
	BoundingBox(){}
	~BoundingBox(){}
	Vect3f GetMinPos(){return m_minPos;}
	Vect3f GetMaxPos(){return m_maxPos;}
	void SetMinPos(Vect3f pos){m_minPos = pos;}
	void SetMaxPos(Vect3f pos){m_maxPos = pos;}
};

class BoundingSphere
{
private:
	Vect3f m_centerPos;
	float m_radius;
public:
	BoundingSphere(){}
	~BoundingSphere(){}
	Vect3f GetCenterPos(){return m_centerPos;}
	float GetRadius(){return m_radius;}
	void SetCenterPos(Vect3f pos){m_centerPos = pos;}
	void SetRadius(float radius){m_radius = radius;}
};

class CStaticMesh
{
protected:
    std::vector<CRenderableVertexs*> m_RVs;
    std::vector<std::vector<CTexture *>> m_Textures;
    std::string m_FileName;
    unsigned int m_NumVertexs, m_NumFaces;
    bool m_deleted;
	BoundingBox m_BBox;
	BoundingSphere m_BSphere;
public:
    CStaticMesh();
    ~CStaticMesh();
    bool Load (const std::string &FileName);
    bool ReLoad ();
    void Render (CGraphicsManager *RM);
    void Destroy();
    bool isDestroyed()
    {
        return m_deleted;
    }
	BoundingBox GetBoundingBox(){return m_BBox;}
	void SetBoundingBox(BoundingBox box){m_BBox = box;}
	BoundingSphere GetBoundingSphere(){return m_BSphere;}
	void SetBoundingSphere(BoundingSphere sphere){m_BSphere = sphere;}
};

#endif