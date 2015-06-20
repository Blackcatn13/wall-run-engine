#include "Object3D.h"
#include "Core_Utils/MemLeaks.h"
#include "XML\XMLTreeNode.h"

CObject3D::CObject3D(const Vect3f &pos, float yaw, float pitch, float roll)
  : m_Position(pos)
  , m_fYaw(yaw)
  , m_fPitch(pitch)
  , m_fRoll(roll)
  , m_transform(m44fIDENTITY) {
}

CObject3D::CObject3D()
  : m_Position( Vect3f(0.f, 0.f, 0.f))
  , m_fYaw(0.f)
  , m_fPitch(0.f)
  , m_fRoll(0.f)
  , m_transform(m44fIDENTITY) {
}

CObject3D::CObject3D(const CXMLTreeNode &node)
  : CVisible(node),
    m_Position(node.GetVect3fProperty("pos", v3fZERO)),
    m_fYaw(node.GetFloatProperty("yaw")),
    m_fPitch(node.GetFloatProperty("pitch")),
    m_fRoll(node.GetFloatProperty("roll")),
    m_scale(node.GetVect3fProperty("scale", v3fONE)),
    m_transform(m44fIDENTITY) {
}

Mat44f CObject3D::getTransform() {
  Mat44f trans =  m44fIDENTITY;
  Mat44f rot = m44fIDENTITY;
  Mat44f scale = m44fIDENTITY;
  trans.Translate(m_Position);
  scale.SetScale(m_scale.x, m_scale.y, m_scale.z);
  rot.RotByAnglesYXZ(m_fYaw, m_fPitch, m_fRoll);
  return trans * rot * scale;
}

void CObject3D::InitMat44() {
  m_transform = m44fIDENTITY;
}