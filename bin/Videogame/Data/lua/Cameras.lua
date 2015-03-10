--Crear instancias de camaras
--[[variables
	m_3DCamera
	m_2DCamera
]]
function on_update_cameras_lua(l_ElapsedTime)
	local coreInstance = CCoreLuaWrapper().m_CoreInstance;
	local act2in = coreInstance:get_action_to_input();
	local camController = coreInstance.m_CameraController;
	if act2in:do_action_from_lua("Set3DCamera") then
		coreInstance.m_CameraController:set_active_camera("3DCam");
	end
	if act2in:do_action_from_lua("Set2DCamera") then
		coreInstance.m_CameraController:set_active_camera("2DCam");
	end
	--local name2=CNamed();
	--name2:set_name("UpdatePass1");
	local cam = camController:get_active_camera();
	--name2:set_name("UpdatePass2");
	--Update Camera 3D
	if(cam.m_eTypeCamera == 6) then
		local pCont = coreInstance:get_player_controller();
		local positionController = pCont:get_position();
		local obj = cam.m_pObject3D;
		obj:set_position(Vect3f(0, 1, positionController.z));
		obj:set_yaw(1.57);
		obj:set_pitch(-0.25);
		obj:set_roll(0);
		cam:set_zoom(25);
		cam.m_fZNear = 0.1;
		cam.m_fZFar = 1000;
		--cam.m_fFOV = 45.0 * 3.1415 / 180;
		--cam.m_fAspectRatio = 1;
	end
	--Update Camera 2D
	if(cam.m_eTypeCamera == 5) then
		local pCont = coreInstance:get_player_controller();
		local positionController = pCont:get_position();
		local obj = cam.m_pObject3D;
		obj:set_position(Vect3f(0, 1, positionController.z));
		obj:set_yaw(3.1415);
		obj:set_pitch(-0.1);
		obj:set_roll(0);
		cam:set_zoom(50);
		cam.m_fZNear = 0.1;
		cam.m_fZFar = 1000;
		--cam.m_fFOV = 45.0 * 3.1415 / 180;
		--cam.m_fAspectRatio = 1;
	end
	--name2:set_name("UpdatePassFinal");
end