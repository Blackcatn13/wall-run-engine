//#ifndef BINDMANAGERS_H
//#define BINDMANAGERS_H
#pragma once

#include "ScriptManager.h"
#include "Utils\Defines.h"
#include "Core\Core.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "SoundManager.h"
#include "Font\FontManager.h"
#include "GraphicsManager.h"
#include "Language\LanguageManager.h"

#include "StaticMeshes\StaticMeshManager.h"

#include <d3dx9.h>
#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Math\Vector2.h"
#include "Math\MathTypes.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/scope.hpp>


using namespace luabind;


void RegisterManagers()
{
	luabind::module(LUA_STATE) [
	class_<CSoundManager>("CSoundManager")
	.def(constructor<>())
	.def("update", &CSoundManager::Update)
	.def("init", &CSoundManager::Init)
	.def("de_init", &CSoundManager::DeInit)
	];

	luabind::module(LUA_STATE) [
	class_<CGraphicsManager>("CGraphicsManager")
	.def(constructor<>())
	.def("update", &CGraphicsManager::Update)
	.def("init", &CGraphicsManager::Init)
	.def("de_init", &CGraphicsManager::DeInit)
	.def("render", &CGraphicsManager::Render)
	.def("get_window_rect", &CGraphicsManager::GetWindowRect)
	.def("setup_matrices", &CGraphicsManager::SetupMatrices)
	.def("get_window_rect", &CGraphicsManager::GetWindowRect)
	/*.def("begin_rendering", &CGraphicsManager::BeginRendering)
	.def("end_rendering", &CGraphicsManager::EndRendering)*/
	.def("begin_rendering", &CGraphicsManager::BeginRenderCommand)
	.def("end_rendering", &CGraphicsManager::EndRenderCommand)
	 //----Transform Functions-----------------------------------------------
	.def("get_window_rect", (void (CGraphicsManager::*)(D3DXMATRIX &)) &CGraphicsManager::SetTransform)
	.def("begin_rendering", (void (CGraphicsManager::*)(Mat44f &)) &CGraphicsManager::SetTransform)
	.def("draw_line", &CGraphicsManager::DrawLine)
	.def("draw_axis", &CGraphicsManager::DrawAxis)
	.def("draw_cube", &CGraphicsManager::DrawCube)
	.def("draw_box", &CGraphicsManager::DrawBox)
	.def("draw_grid", &CGraphicsManager::DrawGrid)
	.def("draw_plane", &CGraphicsManager::DrawPlane)
	.def("get_sphere", &CGraphicsManager::DrawSphere)
	.def("get_camera", &CGraphicsManager::DrawCamera)
	.def("get_quad2D", &CGraphicsManager::DrawQuad2D)

	.def("calculate_alignment", &CGraphicsManager::CalculateAlignment)
	.def("enable_alpha_blend", &CGraphicsManager::EnableAlphaBlend)
	.def("disable_alpha_blend", &CGraphicsManager::DisbaleAlphaBlend)
	.def("enable_z_buffering", &CGraphicsManager::EnableZBuffering)
	.def("disable_z_buffering", &CGraphicsManager::DisbaleZBuffering)
	.def("is_sphere_visible", &CGraphicsManager::isSphereVisible)
	.def("draw_rectangle_2d", &CGraphicsManager::DrawRectangle2D)
	.def("get_ray", &CGraphicsManager::GetRay)
	.def("get_width_and_height", &CGraphicsManager::GetWidthAndHeight)
	.def("draw_capsule", &CGraphicsManager::DrawCapsule)
	.def("draw_cylinder", &CGraphicsManager::DrawCylinder)
	.def("enable_z_buffering", (void (CGraphicsManager::*)(const Vect3f &, const Vect3f &,const Vect3f &, float, float, CColor)) &CGraphicsManager::DrawQuad3D)
	.def("disable_z_buffering", (void (CGraphicsManager::*)(const Vect3f &, const Vect3f &,const Vect3f &, const Vect3f &, CColor)) &CGraphicsManager::DrawQuad3D)
	
	];
		luabind::module(LUA_STATE) [
	class_<CFontManager>("CFontManager")
	.def(constructor<>())
	.def("done", &CFontManager::Done)
	.def("init", (bool (CFontManager::*)(CGraphicsManager *)) &CFontManager::Init)
	//.def("init", (void (CFontManager::*)()) &CFontManager::Init)
	.def("is_ok", &CFontManager::IsOk)
	.def("reload_ttfss", &CFontManager::ReloadTTFs)
	.def("load_ttfs", &CFontManager::LoadTTFs)
	.def("get_ttf_id", &CFontManager::GetTTF_Id)
	.def("create_font", &CFontManager::CreateFont)
	//.def("draw_default_text", &CFontManager::DrawDefaultText)
	//.def("draw_text", &CFontManager::DrawText)
/*	.def("draw_literal", &CFontManager::DrawLiteral)
	.def("size_x", &CFontManager::SizeX)
	.def("size_y", &CFontManager::SizeY)
	.def("get_default_text_size", &CFontManager::GetDefaultTextSize)
	.def("get_text_size", &CFontManager::GetTextSize)
	.def("get_litearl_size", &CFontManager::GetLiteralSize)*/
	];

	luabind::module(LUA_STATE) [
	class_<CLanguageManager>("CLanguageManager")
	.def(constructor<>())
	.def("load_xmls", &CLanguageManager::LoadXMLs)
	.def("set_xml_file", &CLanguageManager::SetXmlFile)
	.def("get_literal", &CLanguageManager::GetLiteral)
	.def("set_current_language", &CLanguageManager::SetCurrentLanguage)
	];
	
	luabind::module(LUA_STATE) [
	class_<CStaticMeshManager>("CStaticMeshManager")
	.def(constructor<>())
	.def("load", &CStaticMeshManager::Load)
	.def("reload", &CStaticMeshManager::Reload)
	];

	luabind::module(LUA_STATE) [
	class_<CInputManager>("CInputManager")
	.def(constructor<>())
	.def("init", &CInputManager::Init)
	.def("done", &CInputManager::Done)
	.def("is_ok", &CInputManager::IsOk)
	.def("update", &CInputManager::Update)
	.def("get_position", &CInputManager::GetPosition)
	.def("get_mouse_delta", &CInputManager::GetMouseDelta)
	.def("is_down", &CInputManager::IsDown)
	.def("is_down_up", &CInputManager::IsDownUp)
	.def("is_up_down", &CInputManager::IsUpDown)
	.def("has_game_pad", &CInputManager::HasGamePad)
	.def("scan_2_ascii", &CInputManager::Scan2ascii)
	.def("get_game_pad_left_thumb_deflection", &CInputManager::GetGamePadLeftThumbDeflection)
	.def("get_game_pad_right_thumb_deflection", &CInputManager::GetGamePadRightThumbDeflection)
	.def("get_game_pad_delta_triggers", &CInputManager::GetGamePadDeltaTriggers)
	.def("set_game_pad_left_motor_speed", &CInputManager::SetGamePadLeftMotorSpeed)
	.def("set_game_pad_right_motor_speed", &CInputManager::SetGamePadRightMotorSpeed)
	];
}

//#endif