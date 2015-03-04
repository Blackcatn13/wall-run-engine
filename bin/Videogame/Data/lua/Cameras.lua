--Crear instancias de camaras
--[[variables
	m_3DCamera
	m_2DCamera
]]
m_3DCamera = 0;
m_2DCamera = 0;
function on_init_cameras_lua()
	local nameInit=CNamed();
	nameInit:set_name("Empieza");
	local znear = 0.1;
	local zfar = 100;
	local fov = 45*3.14/180;
	local aspect = 1;
	local nameBef=CNamed();
	nameBef:set_name("InitCamaraBefore");
	local object = coreInstance:get_player_controller();
	local zoom = 50;
	local name=CNamed();
	name:set_name("InitCamara");
	m_3DCamera = CThPSCamera(znear, zfar, fov, aspect, object, zoom);
	m_3DCmaera.m_eTypeCamera = ETypeCamera.TC_3DCAM;
	m_2DCamera = CThPSCamera(znear, zfar, fov, aspect, object, zoom);
	m_3DCmaera.m_eTypeCamera = ETypeCamera.TC_2DCAM;
end
function on_update_cameras_lua(l_ElapsedTime)
	if act2in:do_action_from_lua("Set3DCamera") then
		local name = CNamed();
		name:set_name("TestCamara");
		coreInstance.m_CameraController:set_active_camera(m_3DCamera);
	end
	if act2in:do_action_from_lua("Move2DForward") then
		coreInstance.m_CameraController:set_active_camera(m_2DCamera);
	end
	--Update Camera 3D
	
	--Update Camera 2D
end