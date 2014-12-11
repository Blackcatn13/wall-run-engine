#include "AnimatedCoreModel.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Texture\TextureManager.h"

CAnimatedCoreModel::CAnimatedCoreModel()
{

}


CAnimatedCoreModel::~CAnimatedCoreModel()
{
	delete m_CalCoreModel;
}


bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
	if(!m_CalCoreModel->loadCoreMesh(Filename))
      {
        return false;
      }

}
bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
	 if(!m_CalCoreModel->loadCoreSkeleton(Filename))
      {
        return false;
      }
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	if(!m_CalCoreModel->loadCoreAnimation( Filename,Name))
      {
        return false;
      }
}


void CAnimatedCoreModel::Load(const std::string &Path)
{

		m_Path = Path;
		CXMLTreeNode newFile;
		if (!newFile.LoadFile(Path.c_str()))
		{
			printf("ERROR loading the file.");
		}
		CXMLTreeNode  m = newFile["actor"];
		if (m.Exists())
		{
			int count = m.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				if(m(i).GetName() == "skeleton") {
					std::string Filename = m(i).GetPszISOProperty("file");
					LoadSkeleton(Path+"\\"+Filename);
				}
				else if(m(i).GetName() == "animation") {
					std::string name = m(i).GetPszISOProperty("name");
					std::string Filename = m(i).GetPszISOProperty("file");
					LoadAnimation(name, Path+"\\"+Filename);
				}
				else if(m(i).GetName() == "mesh") {
					std::string Filename = m(i).GetPszISOProperty("file");
					LoadMesh(Path+"\\"+Filename);
				}
				else if(m(i).GetName() == "texture") {
					std::string Filename = m(i).GetPszISOProperty("file");
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