#include "AnimatedCoreModel.h"
#include "AnimatedModelManager.h"
#include "XML\XMLTreeNode.h"
#include "Texture\TextureManager.h"
#include "Core\Core.h"
#include "Utils\Defines.h"
#include "RenderableVertex\RenderableVertexs.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "cal3d\cal3d.h"
#include "cal3d\coremodel.h"
#include "cal3d\hardwaremodel.h"
#include "Core_Utils/MemLeaks.h"


CAnimatedCoreModel::CAnimatedCoreModel()
    : m_CalCoreModel(NULL)
{
}


CAnimatedCoreModel::~CAnimatedCoreModel()
{
    CHECKED_DELETE(m_CalCoreModel);
    CHECKED_DELETE(m_RenderableVertexs);
    CHECKED_DELETE(m_CalHardwareModel);
}


bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    if (m_CalCoreModel->loadCoreMesh(Filename) == 0)
        return false;
    return true;
}
bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
    if (m_CalCoreModel->loadCoreSkeleton(Filename) == 0)
        return false;
    return true;
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
    if (m_CalCoreModel->loadCoreAnimation(Filename, Name) == 0)
        return false;
    return true;
}


void CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;
    CXMLTreeNode newFile;
    if (!newFile.LoadFile((Path + std::string("\\actor.xml")).c_str())) {
        printf("ERROR loading the file.");
    }
    CXMLTreeNode  m = newFile["animated_model"];
    if (m.Exists()) {
        std::string l_Name = m.GetPszISOProperty("name", "");
        m_CalCoreModel = new CalCoreModel(l_Name);
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i) {
            std::string l_Element(m(i).GetName());
            if ( l_Element == "skeleton") {
                std::string Filename = m(i).GetPszISOProperty("file", "");
                LoadSkeleton(Path + "\\" + Filename);
            } else if (l_Element == "animation") {
                std::string name = m(i).GetPszISOProperty("name", "");
                std::string Filename = m(i).GetPszISOProperty("file", "");
                LoadAnimation(name, Path + "\\" + Filename);
            } else if (l_Element == "mesh") {
                std::string Filename = m(i).GetPszISOProperty("file", "");
                LoadMesh(Path + "\\" + Filename);
            } else if (l_Element == "texture") {
                std::string Filename = Path + std::string(m(i).GetPszISOProperty("file", ""));
                m_TextureFilenameList.push_back(Filename);
                TEXTM->GetResource(Filename);
            }
        }
    }
    LoadVertexBuffer();
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
    assert(id <= m_TextureFilenameList.size());
    return m_TextureFilenameList[id];
}

int CAnimatedCoreModel::GetAnimationId(const std::string &AnimationName) const
{
    return m_CalCoreModel->getCoreAnimationId(AnimationName);
}

/*
 else if(strKey == "skeleton")
    {
      // load core skeleton
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!m_calCoreModel->loadCoreSkeleton(strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreAnimation(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(m_calCoreModel->loadCoreMesh(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
	*/

bool CAnimatedCoreModel::LoadVertexBuffer()
{
    m_NumVtxs = 0;
    m_NumFaces = 0;
    for (int i = 0; i < m_CalCoreModel->getCoreMeshCount(); ++i) {
        CalCoreMesh *l_CalCoreMesh = m_CalCoreModel->getCoreMesh(i);
        for (int j = 0; j < l_CalCoreMesh->getCoreSubmeshCount(); ++j) {
            CalCoreSubmesh *l_CalCoreSubmesh = l_CalCoreMesh->getCoreSubmesh(j);
            m_NumFaces += l_CalCoreSubmesh->getFaceCount();
            m_NumVtxs += l_CalCoreSubmesh->getVertexCount();
        }
    }
    m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);
    CAL3D_HW_VERTEX *l_Vtxs = new CAL3D_HW_VERTEX[m_NumFaces * 3];
    //Cogemos el doble de vértices necesarios porque al crear el model de hardware
    //puede necesitar más vértices que el modelo por software
    unsigned short *l_Idxs = new unsigned short[m_NumFaces * 3];
    m_CalHardwareModel->setVertexBuffer((char*) l_Vtxs,	sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setWeightBuffer(((char*)l_Vtxs) + 12,	sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vtxs) + 28,	sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setNormalBuffer(((char*)l_Vtxs) + 44,	sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setTextureCoordNum(1);
    m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vtxs) + 92,	sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setIndexBuffer((CalIndex *)l_Idxs);
    m_CalHardwareModel->load( 0, 0, MAXBONES);
    m_NumVtxs = m_CalHardwareModel->getTotalVertexCount();
    //En caso de utilizar NormalMap
    //CalcTangentsAndBinormals(l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces*3, sizeof(CAL3D_HW_VERTEX),0, 44, 60, 76, 92);
    m_RenderableVertexs = new CIndexedVertexs<CAL3D_HW_VERTEX>(GRAPHM, l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces * 3);
    delete []l_Vtxs;
    delete []l_Idxs;
    return true;
}