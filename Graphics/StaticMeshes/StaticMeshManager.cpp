#include "StaticMeshManager.h"
#include "XML/XMLTreeNode.h"

#include "Core_Utils/MemLeaks.h"

CStaticMeshManager::CStaticMeshManager() {}

CStaticMeshManager::~CStaticMeshManager () {
  Destroy();
}

bool CStaticMeshManager::Load(const std::string &FileName) {
  m_FileName = FileName;
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(FileName.c_str())) {
    printf("ERROR loading the file.");
  } else {
    CXMLTreeNode  m = newFile["static_meshes"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode nodeChild = m(i);
        std::string name = nodeChild.GetName();
        if (name == "static_mesh") {
          CStaticMesh *l_sMesh = new CStaticMesh();
          std::string meshName = nodeChild.GetPszISOProperty("name", "box");
          std::string filename = nodeChild.GetPszISOProperty("filename", "meshes/a.mesh");
          l_sMesh->Load(filename);
          AddResource(meshName, l_sMesh);
        }
      }
      return true;
    }
  }
  return false;
}
bool CStaticMeshManager::Reload() {
  Destroy();
  return Load(m_FileName);
}
