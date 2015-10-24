#include "CinematicElementCinematic.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Camera\CameraController.h"
#include "Core_Utils/MemLeaks.h"

CCinematicElementCinematic::CCinematicElementCinematic(const CXMLTreeNode &node)
  : CCinematicElement(node) {
}

bool CCinematicElementCinematic::Execute() {
  return false;
}

bool CCinematicElementCinematic::Update(float dt) {
  return false;
}

CCinematicElementCinematic::~CCinematicElementCinematic() {
}