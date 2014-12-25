#include "AnimatedCoreModel.h"
#include "XML\XMLTreeNode.h"
#include "Texture\TextureManager.h"
#include "Core\Core.h"


CAnimatedCoreModel::CAnimatedCoreModel()
	: m_CalCoreModel(NULL)
{	
}


CAnimatedCoreModel::~CAnimatedCoreModel()
{
	//CHECKED_DELETE(m_CalCoreModel);
}


bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
	if(m_CalCoreModel->loadCoreMesh(Filename)==0)
      {
        return false;
      }

}
bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
	 if(m_CalCoreModel->loadCoreSkeleton(Filename)==0)
      {
        return false;
      }
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	if(m_CalCoreModel->loadCoreAnimation(Filename,Name)==0)
      {
        return false;
      }
}


void CAnimatedCoreModel::Load(const std::string &Path)
{

		m_Path = Path;
		CXMLTreeNode newFile;
		if (!newFile.LoadFile((Path+ std::string("\\actor.xml")).c_str()))
		{
			printf("ERROR loading the file.");
		}
		CXMLTreeNode  m = newFile["animated_model"];
		if (m.Exists())
		{
			std::string l_Name=m.GetPszISOProperty("name", "");
			m_CalCoreModel=new CalCoreModel(l_Name);
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				std::string l_Element(m(i).GetName());
				if( l_Element== "skeleton") {
					std::string Filename = m(i).GetPszISOProperty("file", "");
					LoadSkeleton(Path+"\\"+Filename);
				}
				else if(l_Element == "animation") {
					std::string name = m(i).GetPszISOProperty("name", "");
					std::string Filename = m(i).GetPszISOProperty("file", "");
					LoadAnimation(name, Path+"\\"+Filename);
				}
				else if(l_Element == "mesh") {
					std::string Filename = m(i).GetPszISOProperty("file", "");
					LoadMesh(Path+"\\"+Filename);
				}
				else if(l_Element == "texture") {
					std::string Filename = Path+std::string(m(i).GetPszISOProperty("file", ""));
					m_TextureFilenameList.push_back(Filename);
					TEXTM->GetResource(Filename);

				}
			} 
		}
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
	if(id <= m_TextureFilenameList.size())
		return m_TextureFilenameList[id];

	return NULL;
	
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