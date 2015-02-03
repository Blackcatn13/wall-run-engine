//----------------------------------------------------------------------------------
// CPhysicASELoader class
// Author: Enric Vergara
//
// Description:
// Funcion para poder cargar una malla (solo vertices y caras) de un ASE
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_ASE_LOADER_H
#define INC_PHYSIC_ASE_LOADER_H

#include "Math\MathUtils.h"
#include <vector>
class CPhysicASELoader
{
public:
	static bool		ReadMeshFromASE			(	std::string fileName, std::vector<Vect3f>&vertices, std::vector<uint32>& faces );

private:
	//---------PARA PARSEAR UN ASE-------------
	static void		ReadMeshFromASE_aux	(	FILE* f, std::vector<Vect3f>&vertices, std::vector<uint32>& faces );
	static void		ReadVertices				( FILE* f, std::vector<Vect3f>& vertices );
	static void		ReadFaces						( FILE* f, std::vector<uint32>& faces );
	static char*	TrimFront						( char* c );
	//-------------------------------------------
};

#endif //INC_PHYSIC_ASE_LOADER_H
