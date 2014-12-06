#include "GUITexture.h"
#include "Utils\Logger.h"

bool CGUITexture::LoadFile()
{
    LOGGER->AddNewLog(ELL_ERROR, "GUITexture failed to load file '%s'", m_FileName);
    return false;
}