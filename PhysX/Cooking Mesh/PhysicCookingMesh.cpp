#define __DONT_INCLUDE_MEM_LEAKS__
#include <string>
#include <cstdio>

#include "Utils\Logger.h"
#include "Utils\BaseUtils.h"
#include "Utils\Defines.h"

//---PhysX includes----
#undef min
#undef max
#include "NxPhysics.h"
#include "NxCooking.h"
#include "Utils\PhysicUserAllocator.h"
#include "PhysXLoader.h"
#include "Utils\PhysicStream.h"
#include "Utils\PhysicASELoader.h"
#include "Cooking Mesh\PhysicCookingMesh.h"
#include "PhysicsManager.h"
#include "XML\XMLTreeNode.h"

//--------------------------

#if defined( _DEBUG )
#include "Core_Utils/MemLeaks.h"
#endif //defined(_DEBUG)

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
CPhysicCookingMesh::CPhysicCookingMesh( void )
  : m_bIsOk			( false )
  , m_pPhysicSDK		( NULL )
  , m_pMyAllocator	( NULL )
  , m_pCooking		( NULL ) {
}

bool CPhysicCookingMesh::Init (NxPhysicsSDK *_pPhysicSDK, CPhysicUserAllocator *_pMyAllocator) {
  m_pPhysicSDK	= _pPhysicSDK;
  m_pMyAllocator	= new CPhysicUserAllocator();
  assert( m_pPhysicSDK && m_pMyAllocator );
  m_bIsOk = ( m_pMyAllocator != NULL && m_pPhysicSDK != NULL );
  if	(m_bIsOk) {
    m_pCooking = NxGetCookingLib( NX_PHYSICS_SDK_VERSION );
    m_bIsOk = ( m_pCooking != NULL );
    if ( m_bIsOk ) {
      m_bIsOk = m_pCooking->NxInitCooking(m_pMyAllocator, NULL);
    }
  }
  if (!m_bIsOk) {
    Release();
  }
  return m_bIsOk;
}
//To init from Lua
void CPhysicCookingMesh::Init ( CPhysicsManager *physicsManager, CPhysicUserAllocator *myAllocator ) {
  Init(physicsManager->GetPhysicsSDK(), myAllocator );
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicCookingMesh::Done ( void ) {
  if (IsOk()) {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicCookingMesh::Release ( void ) {
  //---Delete TriangleMeshes-----------
  VecMeshes::iterator it(m_TriangleMeshes.begin());
  VecMeshes::iterator endIt(m_TriangleMeshes.end());
  while (it != endIt) {
    NxTriangleMesh *mesh = it->second;
    if ( mesh != NULL && mesh->getReferenceCount() == 0 ) {
      m_pPhysicSDK->releaseTriangleMesh(*mesh);
    }
    ++it;
  }
  m_TriangleMeshes.clear();
  //------------------------------------
  //Close Cooking
  if  (m_pCooking != NULL) {
    m_pCooking->NxCloseCooking();
  }
  CHECKED_DELETE(m_pMyAllocator);
}

void CPhysicCookingMesh::ReleasePhysicMesh (const std::string &name) {
  VecMeshes::iterator it = m_TriangleMeshes.find(name);
  if (it != m_TriangleMeshes.end() ) {
    m_TriangleMeshes.erase(it);
  }
}

NxTriangleMesh *CPhysicCookingMesh::GetPhysicMesh( const std::string &_Name ) {
  VecMeshes::iterator it = m_TriangleMeshes.find(_Name);
  if (it != m_TriangleMeshes.end() ) {
    return it->second;
  }
  return NULL;
}

//----------------------------------------------------------------------------
// PhysicMesh created from a Bin file. This file has been saved previously
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh (	const std::string &_Bin_Filename, const std::string &_NameMesh ) {
  bool isOk = false;
  VecMeshes::iterator it = m_TriangleMeshes.find(_NameMesh);
  if ( it == m_TriangleMeshes.end() ) {
    NxTriangleMesh *triangleMesh = NULL;
    triangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicUserStream(_Bin_Filename.c_str(), true));
    isOk = (triangleMesh != NULL);
    if ( isOk ) {
      m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh *>(_NameMesh, triangleMesh));
    }
  }
  return isOk;
}


//----------------------------------------------------------------------------
// Creating a PhysicMesh from a buffer
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh (	const std::vector<Vect3f> &_Vertices, const std::vector<uint32> &_Faces, const std::string &_NameMesh ) {
  bool isOk = false;
  std::map<std::string, NxTriangleMesh *>::iterator it = m_TriangleMeshes.find(_NameMesh);
  if ( it == m_TriangleMeshes.end() ) {
    // Build physical model
    NxTriangleMeshDesc triangleMeshDesc;
    triangleMeshDesc.numVertices			= (NxU32) _Vertices.size();
    triangleMeshDesc.numTriangles			= (NxU32) _Faces.size() / 3;
    triangleMeshDesc.pointStrideBytes		= sizeof(Vect3f);
    triangleMeshDesc.triangleStrideBytes	= 3 * sizeof(uint32);
    triangleMeshDesc.points					= &_Vertices[0].x;
    triangleMeshDesc.triangles				= &_Faces[0];
    triangleMeshDesc.flags					= 0;
    assert(m_pCooking); //by if the flies...
    CPhysicMemoryWriteBuffer buf;
    if (m_pCooking->NxCookTriangleMesh(triangleMeshDesc, buf)) {
      NxTriangleMesh *l_TriangleMesh = NULL;
      l_TriangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicMemoryReadBuffer(buf.data));
      isOk = (l_TriangleMesh != NULL);
      if (isOk) {
        m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh *>(_NameMesh, l_TriangleMesh));
      }
    }
  }
  return isOk;
}

//----------------------------------------------------------------------------
// Creating a PhysicMesh from void buffers
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh (void *&_Vertices, void *&_Faces, const unsigned short n_vtx, const unsigned short n_indx, const std::string &_NameMesh ) {
  bool isOk = false;
  std::map<std::string, NxTriangleMesh *>::iterator it = m_TriangleMeshes.find(_NameMesh);
  if ( it == m_TriangleMeshes.end() ) {
    // Build physical model
    NxTriangleMeshDesc triangleMeshDesc;
    triangleMeshDesc.numVertices			= (NxU32) n_vtx;
    triangleMeshDesc.numTriangles			= (NxU32) n_indx / 3;
    triangleMeshDesc.pointStrideBytes		= sizeof(float) * 3;
    triangleMeshDesc.triangleStrideBytes	= 3 * sizeof(unsigned short);
    triangleMeshDesc.points					= _Vertices;
    triangleMeshDesc.triangles				= _Faces;
    triangleMeshDesc.flags					= NX_MF_16_BIT_INDICES;
    assert(m_pCooking); //by if the flies...
    CPhysicMemoryWriteBuffer buf;
    if (m_pCooking->NxCookTriangleMesh(triangleMeshDesc, buf)) {
      NxTriangleMesh *l_TriangleMesh = NULL;
      l_TriangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicMemoryReadBuffer(buf.data));
      isOk = (l_TriangleMesh != NULL);
      if (isOk) {
        m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh *>(_NameMesh, l_TriangleMesh));
      }
    }
  }
  return isOk;
}
//----------------------------------------------------------------------------
// Save a PhysicMesh in a bin file
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::SavePhysicMesh ( const std::vector<Vect3f> &_Vertices, const std::vector<uint32> &_Faces,
    const std::string &_BinFilename ) {
  // Build physical model
  NxTriangleMeshDesc triangleMeshDesc;
  triangleMeshDesc.numVertices			= (NxU32)_Vertices.size();
  triangleMeshDesc.numTriangles			= (NxU32)_Faces.size() / 3;
  triangleMeshDesc.pointStrideBytes		= sizeof(Vect3f);
  triangleMeshDesc.triangleStrideBytes	= 3 * sizeof(uint32);
  triangleMeshDesc.points					= &_Vertices[0].x;
  triangleMeshDesc.triangles				= &_Faces[0];
  triangleMeshDesc.flags					= 0;
  assert(m_pCooking);
  bool isOk = m_pCooking->NxCookTriangleMesh(triangleMeshDesc, CPhysicUserStream( _BinFilename.c_str(), false));
  return isOk;
}

//----------------------------------------------------------------------------
// Create a Cloth mesh
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CookClothMesh(const NxClothMeshDesc &desc, NxStream &stream) {
  assert(m_pCooking);
  bool isOk = m_pCooking->NxCookClothMesh(desc, stream);
  return isOk;
}


//----------------------------------------------------------------------------
// Load the ASE File thought ASE Loader
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::ReadMeshFromASE ( FILE *f, std::vector<Vect3f> &_Vertices, std::vector<unsigned int> &_Faces, std::string &name) {
  bool l_bOK = CPhysicASELoader::ReadMeshFromASE (f, _Vertices, _Faces, name);
  return l_bOK;
}

bool CPhysicCookingMesh::CreateMeshFromASE ( std::string _FileName, std::string _Name ) {
  std::vector<Vect3f>			l_Vertices;
  std::vector<unsigned int>	l_Faces;
  FILE *f = CPhysicASELoader::open(_FileName);
  std::string addName;
  while (ReadMeshFromASE ( f, l_Vertices, l_Faces, addName)) {
    CreatePhysicMesh ( l_Vertices, l_Faces, (_Name + addName) );
    l_Vertices.clear();
    l_Faces.clear();
  }
  CPhysicASELoader::close(f);
  return true;
}

bool ReadMesh(const std::string file, void *&pos, void *&index, unsigned short *n_vtxs, unsigned short *n_idxs) {
  FILE *f = fopen(file.c_str(), "rb");
  if (f != NULL) {
    unsigned short header;
    fread(&header, sizeof header, 1, f);
    if (header == 0x55ff) {
      unsigned short n_mat;
      fread(&n_mat, sizeof n_mat, 1, f);
      for (int i = 0; i < n_mat; ++i) {
        //unsigned short n_vtxs;
        fread(n_vtxs, sizeof(unsigned short), 1, f);
        void *pos_aux = (void *)malloc((*n_vtxs) * 3 * sizeof(float));
        fread(pos_aux, (*n_vtxs) * 3 * sizeof(float), 1, f);
        //unsigned short n_idxs;
        fread(n_idxs, sizeof(unsigned short), 1, f);
        void *index_aux = (void *)malloc((*n_idxs) * sizeof(unsigned short));
        fread(index_aux, (*n_idxs) * sizeof(unsigned short), 1, f);
        pos = pos_aux;
        index = index_aux;
      }
      unsigned short footer;
      fread(&footer, sizeof footer, 1, f);
      if (footer != 0xff55) {
        fclose(f);
        return false;
      } else {
        fclose(f);
        return true;
      }
    }
  }
  return false;
}

bool CPhysicCookingMesh::LoadFromXML(const std::string &_FileName) {
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(_FileName.c_str())) {
    printf("ERROR loading the file.");
    return false;
  } else {
    CXMLTreeNode  m = newFile["Physx_meshes"];
    if (m.Exists()) {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i) {
        CXMLTreeNode child = m.getNextChild();
        std::string name = child.GetName();
        if (name == "Physx_mesh") {
          void *vertex = 0;
          void *index = 0;
          unsigned short n_vtx = 0;
          unsigned short n_indx = 0;
          ReadMesh(child.GetPszISOProperty("filename", "", false), vertex, index, &n_vtx, &n_indx);
          CreatePhysicMesh(vertex, index, n_vtx, n_indx, child.GetPszISOProperty("name", "", false));
          delete(vertex);
          delete(index);
        }
      }
      return true;
    }
  }
}