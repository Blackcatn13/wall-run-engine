#include "CinematicObjectKeyFrame.h"
#include "Core_Utils/MemLeaks.h"


CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(CXMLTreeNode &atts)
    : CObject3D(atts.GetVect3fProperty("pos", v3fONE), atts.GetFloatProperty("yaw"),
                atts.GetFloatProperty("pitch"), atts.GetFloatProperty("roll"))
    , m_KeyFrameTime(atts.GetFloatProperty("time"))
{
    m_scale = atts.GetVect3fProperty("scale", v3fONE);
}