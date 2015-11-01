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
    CXMLTreeNode nodeChild = m.getNextChild();
    std::string name = nodeChild.GetName();
    if (name == "spline") {
      // std::string l_lineName = nodeChild.GetPszISOProperty("name", "");
      int numPoints = nodeChild.GetNumChildren();
      for (int j = 0; j < numPoints; ++j) {
        CXMLTreeNode nodeChild1 = nodeChild.getNextChild();
        std::string namePoint = nodeChild1.GetName();
        if (namePoint == "point") {
          // std::string pointId = nodeChild1.GetPszISOProperty("id", "");
          Vect3f pos = nodeChild1.GetVect3fProperty("pos", v3fZERO);
          m_path.push_back(pos);
          PathPointSpec pointSpecs;
          pointSpecs.canRotate = nodeChild1.GetBoolProperty("can_rotate", false);
		  pointSpecs.distToRotate = nodeChild1.GetFloatProperty("dist_to_rotate", -1);
          pointSpecs.canGoBack = nodeChild1.GetBoolProperty("can_go_back", false);
		  pointSpecs.followPlayerX = nodeChild1.GetBoolProperty("follow_player_x", false);
          pointSpecs.followPlayerY = nodeChild1.GetBoolProperty("follow_player_y", false);
		  pointSpecs.variableZoom = nodeChild1.GetBoolProperty("variable_zoom", false);
		  pointSpecs.variableZoomValue = nodeChild1.GetFloatProperty("variable_zoom_value", 6);
		  pointSpecs.variablePitch = nodeChild1.GetBoolProperty("variable_pitch", false);
		  pointSpecs.variablePitchValue = nodeChild1.GetFloatProperty("variable_pitch_value", -0.2);
		  pointSpecs.yawOffset = nodeChild1.GetBoolProperty("yaw_offset", false);
		  pointSpecs.yawOffsetValue = nodeChild1.GetFloatProperty("yaw_offset_value", 0);
		  pointSpecs.yawSpeed = nodeChild1.GetFloatProperty("yaw_speed", 0);
          m_pathSpecs.push_back(pointSpecs);
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

std::vector<PathPointSpec>	CThPSCamera::GetMPathSpecs() {
  return m_pathSpecs;
}

Vect3f CThPSCamera::GetPathPoint(int i) {
  Vect3f pos = v3fZERO;
  if (i < m_path.size()) {
    pos = m_path[i];
  }
  return pos;
}

PathPointSpec CThPSCamera::GetPathPointSpecs(int i) {
  PathPointSpec spec;
  if (i < m_pathSpecs.size()) {
    spec = m_pathSpecs[i];
  }
  return spec;
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