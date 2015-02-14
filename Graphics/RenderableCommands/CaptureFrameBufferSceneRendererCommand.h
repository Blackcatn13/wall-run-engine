#pragma once
#ifndef CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H
#define CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"

class CCaptureFrameBufferSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
    CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &atts);
    void Execute(CGraphicsManager &RM);
};

#endif