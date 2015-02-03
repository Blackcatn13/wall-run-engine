#define __DONT_INCLUDE_MEM_LEAKS__
#include "Utils\PhysicASELoader.h"
#include "Math\Vector3.h"
#include "Utils\BaseUtils.h"
#include <stdio.h>

#include "Core_Utils/MemLeaks.h"

char* CPhysicASELoader::TrimFront(char* c)
{
	while(*c == ' ' || *c == '\t')
	{
		c++;
	}
	return c;
}


bool CPhysicASELoader::ReadMeshFromASE(	std::string fileName, std::vector<Vect3f>&vertices,
																			 std::vector<uint32>& faces ) 
{
	FILE* f = NULL;
	fopen_s(&f, fileName.c_str(), "rb");
	if (f == NULL)
	{
		//TODO....printf("File not found: %s\n", filename.c_str());
		return false;
	}

	char line[512];
	int linenbr = 0;
	while (!feof(f))
	{
		fgets(line, 512, f);
		char* l = TrimFront(line);
		if (!strncmp(l, "*MESH {", 7)) 
		{
			ReadMeshFromASE_aux(f, vertices, faces);
		} 
		else 
		{
			//printf("Line %4d: %s\n", linenbr++, l);
		}
	}
	fclose(f);
	return true;
};

void CPhysicASELoader::ReadMeshFromASE_aux(	FILE* f, std::vector<Vect3f>&vertices, std::vector<uint32>& faces ) 
{
	char line[512];
	int nbVertices = 0;
	int nbFaces = 0;
	while(true)
	{
		fgets(line, 512, f);
		char* l = TrimFront(line);
		if (*l == '}')
		{
			return;
		}
		else if (strstr(l, "*MESH_VERTEX_LIST {")) 
		{
			ReadVertices(f, vertices);
		}
		else if (strstr(l, "*MESH_FACE_LIST {")) 
		{
			ReadFaces(f, faces);
		}
	}
}



void CPhysicASELoader::ReadVertices( FILE* f, std::vector<Vect3f>& vertices ) 
{
	char line[512];

	while(true) 
	{
		fgets(line, 512, f);
		char* l = TrimFront(line);
		if (*l == '}') 
		{
			return;
		} 
		else if (strstr(l, "*MESH_VERTEX")) 
		{
			float a,b,c;
			int i;
			sscanf_s(l, "*MESH_VERTEX %d %f %f %f", &i, &a, &b, &c);
			//Vect3f a2(3,3,3);
			vertices.push_back( Vect3f(a,c,b) );
		}
	}
}

void CPhysicASELoader::ReadFaces( FILE* f, std::vector<uint32>& faces ) 
{
	char line[512];

	while(true)
	{
		fgets(line, 512, f);
		char* l = TrimFront(line);
		if (*l == '}') 
		{
			return;
		} 
		else if (strstr(l, "*MESH_FACE")) 
		{
			uint32 a,b,c;
			int i;
			sscanf_s(l, "*MESH_FACE %d: A: %d B: %d C: %d ", &i, &a, &b, &c);
			faces.push_back(a);
			faces.push_back(c);
			faces.push_back(b);
		}
	}
}

