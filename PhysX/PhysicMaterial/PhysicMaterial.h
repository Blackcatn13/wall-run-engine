#pragma once

class NxMaterial;
class NxMaterialDesc;

class CPhysicMaterial
{
public:
    CPhysicMaterial(void);
    virtual ~CPhysicMaterial(void);
    void NewMaterial(float, float, float);
    void setMaterial(NxMaterial* newMat)
    {
        m_Mat = newMat;
    }
    int getMaterialID();
    NxMaterialDesc* getMaterialDesc()
    {
        return m_Desc;
    }
    NxMaterial* getMaterial()
    {
        return m_Mat;
    }
private:
    NxMaterialDesc *m_Desc;
    NxMaterial *m_Mat;
};

