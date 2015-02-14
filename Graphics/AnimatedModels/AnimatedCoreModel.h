#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H

#include <vector>
#include <string>
#include "cal3D\coremodel.h"
#include "cal3d\hardwaremodel.h"

class CRenderableVertexs;

class CAnimatedCoreModel
{
private:
    CalCoreModel*				m_CalCoreModel;
    CalHardwareModel*			m_CalHardwareModel;
    CRenderableVertexs*			m_RenderableVertexs;
    int							m_NumFaces;
    int							m_NumVtxs;
    std::string m_Name;
    std::vector<std::string> m_TextureFilenameList;
    std::string m_Path;
    bool LoadMesh(const std::string &Filename);
    bool LoadSkeleton(const std::string &Filename);
    bool LoadAnimation(const std::string &Name, const std::string &Filename);
public:
    CAnimatedCoreModel();
    ~CAnimatedCoreModel();
    CalCoreModel *GetCoreModel( )
    {
        return m_CalCoreModel;
    }
    const std::string & GetTextureName( size_t id );
    size_t GetNumTextures( ) const
    {
        return m_TextureFilenameList.size();
    }
    void Load(const std::string &Path);
    int GetAnimationId(const std::string &AnimationName) const;

    CalHardwareModel * GetCalHardwareModel() const
    {
        return m_CalHardwareModel;
    }
    bool LoadVertexBuffer();
    CRenderableVertexs * GetRenderableVertexs() const
    {
        return m_RenderableVertexs;
    }
};


#endif

//****************************************************************************//
