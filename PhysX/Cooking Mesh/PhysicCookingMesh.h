//----------------------------------------------------------------------------------
// CPhysicCookingMesh class
// Author: Enric Vergara
//
// Description:
// Clase encargarda de gestionar las mallas triangulares utilizadas para la física
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_PHYSIC_COOKING_MESH_H_
#define INC_PHYSIC_COOKING_MESH_H_

#include <map>
#include <vector>
#include "Math\Vector3.h"

//---Forward Declarations---
class NxPhysicsSDK;
class CPhysicUserAllocator;
class NxCookingInterface;
class NxTriangleMesh;
//Cloth
class NxStream;
class NxClothMeshDesc;
//--------------------------

using namespace std;

class CPhysicCookingMesh
{
public:
	typedef std::map<std::string,NxTriangleMesh*> VecMeshes;

public:
	//--- Init and End protocols	
								CPhysicCookingMesh	( void );
								~CPhysicCookingMesh	( void )			{ Done(); }

	bool						Init				( NxPhysicsSDK* physicSDK, CPhysicUserAllocator* myAllocator );
	void						Done				( void );
	bool						IsOk				( void ) const { return m_bIsOk; }


	NxTriangleMesh*				GetPhysicMesh		(const std::string& name);
	size_t						GetNumMeshes		() { return m_TriangleMeshes.size(); }		// Added by XMA


	bool						CreatePhysicMesh	( const std::string& _BinFilename, const std::string& _NameMesh );
	bool						CreatePhysicMesh	( const std::vector<Vect3f>& _Vertices, const std::vector<uint32>& _Faces, const std::string &_NameMesh );
	bool						SavePhysicMesh		( const std::vector<Vect3f>& _Vertices, const std::vector<uint32>& _Faces, const std::string &_BinFilename );
	bool						CreateMeshFromASE	( std::string _FileName, std::string _Name );
	bool						CookClothMesh		( const NxClothMeshDesc& _Desc, NxStream& _Stream);

	/* Movido desde private por XMA */
	void						Release				( void );
	void						ReleasePhysicMesh   (const std::string& name);

private:
	bool						ReadMeshFromASE		( std::string _FileName, std::vector<Vect3f>& _Vertices, std::vector<unsigned int>& _Faces );
//	void						Release				( void );

private:

	bool						m_bIsOk;
	NxPhysicsSDK*				m_pPhysicSDK;
	CPhysicUserAllocator*		m_pMyAllocator;
	NxCookingInterface*			m_pCooking;
	VecMeshes					m_TriangleMeshes;
};

#endif // INC_PHYSIC_COOKING_MESH_H_