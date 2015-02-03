#define __DONT_INCLUDE_MEM_LEAKS__
#include <string>

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
	, m_pCooking		( NULL ) 
{
}

bool CPhysicCookingMesh::Init (NxPhysicsSDK* _pPhysicSDK, CPhysicUserAllocator* _pMyAllocator)
{

	m_pPhysicSDK	= _pPhysicSDK;
	m_pMyAllocator	= new CPhysicUserAllocator();
	assert( m_pPhysicSDK && m_pMyAllocator );
	m_bIsOk = ( m_pMyAllocator != NULL && m_pPhysicSDK != NULL );	
	if	(m_bIsOk)
	{
		m_pCooking = NxGetCookingLib( NX_PHYSICS_SDK_VERSION );
		m_bIsOk = ( m_pCooking != NULL );
		if ( m_bIsOk )
		{
			m_bIsOk = m_pCooking->NxInitCooking(m_pMyAllocator, NULL);
		}
	}

	if (!m_bIsOk)
	{
		Release();
	}

	return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicCookingMesh::Done ( void )
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicCookingMesh::Release ( void )
{
	//---Delete TriangleMeshes-----------
	VecMeshes::iterator it(m_TriangleMeshes.begin());
	VecMeshes::iterator endIt(m_TriangleMeshes.end());
	while (it!=endIt)
	{
		NxTriangleMesh* mesh = it->second;
		if( mesh != NULL && mesh->getReferenceCount() == 0 )
		{
			m_pPhysicSDK->releaseTriangleMesh(*mesh);
		}
		++it;
	}
	m_TriangleMeshes.clear();
	//------------------------------------

	//Close Cooking
	if  (m_pCooking != NULL)
	{
		m_pCooking->NxCloseCooking();
	}
	CHECKED_DELETE(m_pMyAllocator);
}

void CPhysicCookingMesh::ReleasePhysicMesh (const std::string& name)
{
	VecMeshes::iterator it = m_TriangleMeshes.find(name);
	if(it != m_TriangleMeshes.end() )
	{
		m_TriangleMeshes.erase(it);
	}
}

NxTriangleMesh* CPhysicCookingMesh::GetPhysicMesh( const std::string& _Name )
{
	VecMeshes::iterator it = m_TriangleMeshes.find(_Name);
	if(it != m_TriangleMeshes.end() )
	{
		return it->second;
	}
	return NULL;
}

//----------------------------------------------------------------------------
// PhysicMesh created from a Bin file. This file has been saved previously
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh (	const std::string& _Bin_Filename, const std::string& _NameMesh )
{
	bool isOk = false;
	VecMeshes::iterator it = m_TriangleMeshes.find(_NameMesh);
	if( it == m_TriangleMeshes.end() )
	{
		NxTriangleMesh* triangleMesh = NULL;
		triangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicUserStream(_Bin_Filename.c_str(), true));
		isOk = (triangleMesh!=NULL);
		if( isOk )
		{
			m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh*>(_NameMesh, triangleMesh));
		}
	}

	return isOk;
}


//----------------------------------------------------------------------------
// Creating a PhysicMesh from a buffer
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh (	const std::vector<Vect3f>& _Vertices, const std::vector<uint32>& _Faces, const std::string &_NameMesh )
{
	bool isOk = false;
	std::map<std::string,NxTriangleMesh*>::iterator it = m_TriangleMeshes.find(_NameMesh);
	if( it == m_TriangleMeshes.end() )
	{
		// Build physical model
		NxTriangleMeshDesc triangleMeshDesc;
		triangleMeshDesc.numVertices			= (NxU32) _Vertices.size();
		triangleMeshDesc.numTriangles			= (NxU32) _Faces.size()/3;
		triangleMeshDesc.pointStrideBytes		= sizeof(Vect3f);
		triangleMeshDesc.triangleStrideBytes	= 3*sizeof(uint32);
		triangleMeshDesc.points					= &_Vertices[0].x;
		triangleMeshDesc.triangles				= &_Faces[0];
		triangleMeshDesc.flags					= 0;

		assert(m_pCooking); //by if the flies...

		CPhysicMemoryWriteBuffer buf;
		if (m_pCooking->NxCookTriangleMesh(triangleMeshDesc,buf))
		{
			NxTriangleMesh* l_TriangleMesh = NULL;
			l_TriangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicMemoryReadBuffer(buf.data));
			isOk = (l_TriangleMesh != NULL);
			if (isOk)
			{
				m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh*>(_NameMesh, l_TriangleMesh));
			}
		}	

	}

	return isOk;	
}

//----------------------------------------------------------------------------
// Save a PhysicMesh in a bin file
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::SavePhysicMesh ( const std::vector<Vect3f>& _Vertices, const std::vector<uint32>& _Faces,
																				 const std::string &_BinFilename )
{
	// Build physical model
	NxTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.numVertices			= (NxU32)_Vertices.size();
	triangleMeshDesc.numTriangles			= (NxU32)_Faces.size()/3;
	triangleMeshDesc.pointStrideBytes		= sizeof(Vect3f);
	triangleMeshDesc.triangleStrideBytes	= 3*sizeof(uint32);
	triangleMeshDesc.points					= &_Vertices[0].x;
	triangleMeshDesc.triangles				= &_Faces[0];
	triangleMeshDesc.flags					= 0;

	assert(m_pCooking);
	bool isOk = false;

	isOk = m_pCooking->NxCookTriangleMesh(triangleMeshDesc, CPhysicUserStream( _BinFilename.c_str(), false));

	return isOk;
}

//----------------------------------------------------------------------------
// Create a Cloth mesh
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CookClothMesh(const NxClothMeshDesc& desc, NxStream& stream)
{
	assert(m_pCooking);
	bool isOk = false;
	isOk = m_pCooking->NxCookClothMesh(desc,stream);
	return isOk;
}


//----------------------------------------------------------------------------
// Load the ASE File thought ASE Loader
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::ReadMeshFromASE ( std::string _FileName, std::vector<Vect3f>& _Vertices, std::vector<unsigned int>& _Faces )
{
	bool l_bOK = CPhysicASELoader::ReadMeshFromASE ( _FileName, _Vertices, _Faces );
	return l_bOK;
}

bool CPhysicCookingMesh::CreateMeshFromASE ( std::string _FileName, std::string _Name )
{
	std::vector<Vect3f>			l_Vertices;
	std::vector<unsigned int>	l_Faces;

	if ( ReadMeshFromASE ( _FileName, l_Vertices, l_Faces ) )
		if ( CreatePhysicMesh ( l_Vertices, l_Faces, _Name ) )
			return true;
		else
		{
			LOGGER->AddNewLog ( ELL_ERROR, "Error al leer la mesh del fichero ASE: %s", _FileName );
			return false; 
		}
	else
	{
		LOGGER->AddNewLog ( ELL_ERROR, "Error al leer la mesh del fichero ASE: %s", _FileName );
		return false; 
	}
}