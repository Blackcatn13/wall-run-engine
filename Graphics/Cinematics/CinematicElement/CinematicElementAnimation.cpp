#include "CinematicElementAnimation.h"
#include "XML\XMLTreeNode.h"
#include "Core\Core.h"
#include "Cinematics\Cinematic.h"
#include "Renderable\RenderableObjectsLayersManager.h"
#include "Renderable\RenderableObjectsManager.h"
#include "Renderable\RenderableObject.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "Core_Utils/MemLeaks.h"


CCinematicElementAnimation::CCinematicElementAnimation(const CXMLTreeNode &node)
  : CCinematicElement(node),
    m_room(node.GetIntProperty("room")),
    m_layer(node.GetPszISOProperty("layer", "")),
    m_animatedModel(node.GetPszISOProperty("animated", "")),
    m_animationId(node.GetIntProperty("id")) {
}

bool CCinematicElementAnimation::Execute() {
  ((CAnimatedInstanceModel *)RENDLM->GetRenderableObjectsManagerByStrAndRoom(m_layer, m_room)->GetResource(m_animatedModel))->ExecuteAction(m_animationId, 0.2, 0.2, 1, false);
  return true;
}

bool CCinematicElementAnimation::Update(float dt) {
  return false;
}

CCinematicElementAnimation::~CCinematicElementAnimation() {
}

void CCinematicElementAnimation::Stop() {
  ((CAnimatedInstanceModel *)RENDLM->GetRenderableObjectsManagerByStrAndRoom(m_layer, m_room)->GetResource(m_animatedModel))->RemoveAction(m_animationId);
}