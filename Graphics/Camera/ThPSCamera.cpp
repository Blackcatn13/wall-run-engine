#include "ThPSCamera.h"
#include <assert.h>
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"

CThPSCamera::CThPSCamera(float zn, float zf, float fov, float aspect, CObject3D *object3D, float zoom)
  : CCamera( zn, zf, fov, aspect, object3D, TC_THPS)
  , m_fZoom( zoom ) {
}

CThPSCamera::CThPSCamera()
  : CCamera()
  , m_fZoom(50.f)
{}


Vect3f CThPSCamera::GetDirection () const {
  assert(m_pObject3D);
  return (m_pObject3D->GetPosition() - GetEye());
}

Vect3f CThPSCamera::GetLookAt () const {
  assert(m_pObject3D);
  return m_pObject3D->GetPosition();
}

Vect3f CThPSCamera::GetEye () const {
  assert(m_pObject3D);
  float yaw		= m_pObject3D->GetYaw();
  float pitch	= m_pObject3D->GetPitch();
  Vect3f pos	= m_pObject3D->GetPosition();
  //Pasamos de coordenadas esfericas a coordenadas cartesianas
  Vect3f vEyePt(	m_fZoom * cos(yaw) * cos(pitch),
                  m_fZoom * sin(pitch),
                  m_fZoom * sin(yaw) * cos(pitch) );
  return (pos - vEyePt);
}

Vect3f CThPSCamera::GetVecUp () const {
  assert(m_pObject3D);
  float yaw		= m_pObject3D->GetYaw();
  float pitch	= m_pObject3D->GetPitch();
  Vect3f vUpVec(	-cos(yaw) * sin(pitch),
                  cos(pitch),
                  -sin(yaw) * sin(pitch) );
  return vUpVec;
}

void CThPSCamera::SetZoom (float zoom) {
  m_fZoom = zoom;
  if ( m_fZoom > m_fZFar * 0.8f ) {
    m_fZoom = m_fZFar * 0.8f;
  } else if ( m_fZoom < m_fZNear * 2.f) {
    m_fZoom = m_fZNear * 2.f;
  }
}

void CThPSCamera::AddZoom (float zoom) {
  m_fZoom += zoom;
  if ( m_fZoom > m_fZFar * 0.8f ) {
    m_fZoom = m_fZFar * 0.8f;
  } else if ( m_fZoom < m_fZNear * 2.f) {
    m_fZoom = m_fZNear * 2.f;
  }
}
bool CThPSCamera::LoadPathFromFile(std::string filename) {
  m_pathFile = filename;
  m_path = std::vector<Vect3f>();
  CXMLTreeNode newFile;
  if (!newFile.LoadFile(filename.c_str())) {
    printf("ERROR loading the file.");
  }
  CXMLTreeNode  m = newFile["camera_path"];
  int count = m.GetNumChildren();
  for (int i = 0; i < count; ++i) {
    std::string name = m(i).GetName();
    if (name == "spline") {
      // std::string l_lineName = m(i).GetPszISOProperty("name", "");
      int numPoints = m(i).GetNumChildren();
      for (int j = 0; j < numPoints; ++j) {
        std::string namePoint = m(i)(j).GetName();
        if (namePoint == "point") {
          // std::string pointId = m(i)(j).GetPszISOProperty("id", "");
          Vect3f pos = m(i)(j).GetVect3fProperty("pos", v3fZERO);
          m_path.push_back(pos);
		  bool canRotate = m(i)(j).GetBoolProperty("can_rotate", false);
		  m_pathRotate.push_back(canRotate);
        }

      }
    }
  }
  m_CurrentWaypoint = 0;
  if (m_path.size() > 0) {
    m_NextWaypoint = 1;
  } else {
    m_NextWaypoint = 0;
  }

  return true;
}

std::vector<Vect3f>	CThPSCamera::GetMPath() {
  return m_path;
}

std::vector<bool>	CThPSCamera::GetMPathRotate() {
  return m_pathRotate;
}

Vect3f CThPSCamera::GetPathPoint(int i) {
  Vect3f pos = v3fZERO;
  if (i < m_path.size()) {
    pos = m_path[i];
  }
  return pos;
}

bool CThPSCamera::GetPathPointRotate(int i) {
  bool canRotate = false;
  if (i < m_pathRotate.size()) {
    canRotate = m_pathRotate[i];
  }
  return canRotate;
}

int CThPSCamera::GetPathSize() {
  return m_path.size();
}

Vect3f CThPSCamera::GetLastPlayerPos() {
  return m_LastPlayerPos;
}

void CThPSCamera::SetLastPlayerPos(Vect3f lPos) {
  m_LastPlayerPos = lPos;
}

int CThPSCamera::GetCurrentWaypoint() {
  return m_CurrentWaypoint;
}

void CThPSCamera::SetCurrentWaypoint(int i) {
  m_CurrentWaypoint = i;
}

int CThPSCamera::GetNextWaypoint() {
  return m_NextWaypoint;
}

void CThPSCamera::SetNextWaypoint(int i) {
  m_NextWaypoint = i;
}