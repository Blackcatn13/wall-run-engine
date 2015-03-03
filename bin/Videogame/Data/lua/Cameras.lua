--Crear instancias de camaras
--[[variables
	m_3DCamera
	m_2DCamera
]]
function on_init_cameras_lua()
end
function on_update_cameras_lua(l_ElapsedTime)
	if act2in:do_action_from_lua("Set3DCamera") then
		set_active_camera(m_3DCamera);
	end
	if act2in:do_action_from_lua("Move2DForward") then
		set_active_camera(m_2DCamera);
	end
	--Update Camera 3D
	
	--Update Camera 2D
end