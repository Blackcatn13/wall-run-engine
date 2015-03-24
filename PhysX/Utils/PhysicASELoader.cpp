#define __DONT_INCLUDE_MEM_LEAKS__
#include "Utils\PhysicASELoader.h"
#include "Math\Vector3.h"
#include "Utils\BaseUtils.h"


#include "Core_Utils/MemLeaks.h"

char *CPhysicASELoader::TrimFront(char *c) {
  while (*c == ' ' || *c == '\t') {
    c++;
  }
  return c;
}

std::string CPhysicASELoader::GetName(char *c) {
  char *firstpos = c;
  int size = 0;
  while (*c != '\"') {
    c++;
    size++;
  }
  return std::string(firstpos, size);
}

FILE *CPhysicASELoader::open(std::string fileName) {
  FILE *m_f = NULL;
  fopen_s(&m_f, fileName.c_str(), "rb");
  if (m_f == NULL) {
    //TODO....printf("File not found: %s\n", filename.c_str());
  }
  return m_f;
}

bool CPhysicASELoader::close(FILE *m_f) {
  fclose(m_f);
  return true;
}

bool CPhysicASELoader::ReadMeshFromASE(FILE *m_f,	std::vector<Vect3f> &vertices,
                                       std::vector<uint32> &faces, std::string &name) {
  char line[512];
  int linenbr = 0;
  while (!feof(m_f)) {
    fgets(line, 512, m_f);
    char *l = TrimFront(line);
    if (!strncmp(l, "*NODE_NAME ", 11)) {
      char *aux = l + 12;
      name = GetName(aux);
    }
    if (!strncmp(l, "*MESH {", 7)) {
      ReadMeshFromASE_aux(m_f, vertices, faces);
      return true;
    } else {
      //printf("Line %4d: %s\n", linenbr++, l);
    }
  }
  if (feof(m_f))
    return false;
  return true;
}

void CPhysicASELoader::ReadMeshFromASE_aux(	FILE *f, std::vector<Vect3f> &vertices, std::vector<uint32> &faces ) {
  char line[512];
  int nbVertices = 0;
  int nbFaces = 0;
  while (true) {
    fgets(line, 512, f);
    char *l = TrimFront(line);
    if (*l == '}') {
      return;
    } else if (strstr(l, "*MESH_VERTEX_LIST {")) {
      ReadVertices(f, vertices);
    } else if (strstr(l, "*MESH_FACE_LIST {")) {
      ReadFaces(f, faces);
    }
  }
}

void CPhysicASELoader::ReadVertices( FILE *f, std::vector<Vect3f> &vertices ) {
  char line[512];
  while (true) {
    fgets(line, 512, f);
    char *l = TrimFront(line);
    if (*l == '}') {
      return;
    } else if (strstr(l, "*MESH_VERTEX")) {
      float a, b, c;
      int i;
      sscanf_s(l, "*MESH_VERTEX %d %f %f %f", &i, &a, &b, &c);
      //Vect3f a2(3,3,3);
      vertices.push_back( Vect3f(a, c, b) );
    }
  }
}

void CPhysicASELoader::ReadFaces( FILE *f, std::vector<uint32> &faces ) {
  char line[512];
  while (true) {
    fgets(line, 512, f);
    char *l = TrimFront(line);
    if (*l == '}') {
      return;
    } else if (strstr(l, "*MESH_FACE")) {
      uint32 a, b, c;
      int i;
      sscanf_s(l, "*MESH_FACE %d: A: %d B: %d C: %d ", &i, &a, &b, &c);
      faces.push_back(a);
      faces.push_back(c);
      faces.push_back(b);
    }
  }
}

