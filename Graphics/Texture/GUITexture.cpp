#include "GUITexture.h"
#include "Utils\Logger.h"
#include "Core_Utils/MemLeaks.h"

bool CGUITexture::LoadFile() {
  LOGGER->AddNewLog(ELL_ERROR, "GUITexture failed to load file '%s'", m_FileName);
  return false;
}