local coreInstance = CCoreLuaWrapper().m_CoreInstance
local physx_manager = coreInstance:get_phisics_manager()
local rol_manager = coreInstance:get_renderable_object_layer_manager()

function init_platform(name, user_data_name, size, position)
	local platform = rol_manager:get_default_renderable_object_manager():get_resource(name)
	platform:insert_platform(user_data_name, size, position)
	coreInstance:trace(name)
	return platform
end

function init_poly_platform(name, user_data_name, size, position, activeActor, time_out)
	local platform = init_platform(name, user_data_name, size, position)
	platform.m_PlatformActor:activate(activeActor)
	platform.m_OriginalScale = platform:get_scale()
	platform.m_TimeOut= time_out
	return platform
end

function update_break_platform(dt, current_time, max_time, platform_name)

	if current_time >= max_time then
		local platform = rol_manager:get_default_renderable_object_manager():get_resource(platform_name)
		platform:disable_platform(dt)
	end
end

--Pinchos

function on_enter_pinchos(platform_name)
	local platform = rol_manager:get_default_renderable_object_manager():get_resource(platform_name)
	platform:falling_into_platform()

end


--

-- Moving Platform
local next_wp = Vect3f(0,0,0)
instance = CLuaGlobalsWrapper().m_CoreInstance;

function mp_enter_stopped()
	coreInstance:trace("Parando plataforma");
	--next_wp = Vect3f(-15,0,-15) 
	instance.m_string = "Buscar_next_WP_Plaform"
	return 0 
end

function mp_exit_stopped()
	--coreInstance:trace("Saliendo Platform stopped");

end

function mp_update_stopped(ElapsedTime)
	--core:trace("Update Platform stopped");
	--instance.m_string = "Buscar_next_WP_Plaform"
end

function mp_enter_moving()
	--coreInstance:trace("Entering Platform moving State");
end

function mp_exit_moving()
	--coreInstance:trace("Exit Moviendo Plataforma");
end

function mp_update_moving(ElapsedTime)
	
	--local player_position = coreInstance:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
	local platform = rol_manager:get_default_renderable_object_manager():get_resource("MovingPlatform001")-- modificar para poder pasarlo por parametro
	platform:move_to_point(ElapsedTime, next_wp, 2)
	--move_platform_to_point(ElapsedTime, next_wp, platform)
	local current_pos = platform:get_position()
	local distance_to_point = get_distance_to_point(current_pos, next_wp)
	if distance_to_point <= 4 then
		coreInstance:trace("Destino alcanzado")
		instance.m_string = "Parado"
	end
		
end

--[[function move_platform_to_point(ElapsedTime, wp, _platform)

local platform_position = _platform:get_position()
  if wp:distance(platform_position)> 2 then
        local direction = (wp - _platform:get_position())
		--local normalized = direction:normalize()
        _platform:set_position( _platform:get_position() + direction * _platform.m_Speed * ElapsedTime)  --Hay que mirar de normalizar direccion desde aqui
        _platform.m_PlatorformActor:set_kinematic(true);
        _platform. m_PlatorformActor:set_global_position(_platform.m_Position);
        --Vect3f l_VelPos = _platform.m_Position - (Vect3f(GetScale().x / 2, .0f, .0f));
        --m_PlatorformActor->AddVelocityAtPos(direction.Normalize(), v3fZERO, 10);
        --m_PlatorformActor->SetLinearVelocity(direction.Normalize() * m_Speed * ElapsedTime);
    end

end--]]


function mp_enter_calcwp() -- Pasar el nombre de la plataforma y de ahí que recoja el wp que necesita? 
-- OnEnter Buscar_next_WP 
	coreInstance:trace("Entering Buscar Platform next WP");
	--coreInstance:trace(tostring(next_wp.x));
	--if next_wp == Vect3f(15,0,15) then
	--	next_wp = Vect3f(-15, 0, 15)	
	--else
	--	next_wp = Vect3f(15,0,15)
	--end
	
	local platform = rol_manager:get_default_renderable_object_manager():get_resource("MovingPlatform001")
	next_wp = platform:get_next_wp()
	--coreInstance:trace(tostring(next_wp.x));
	instance.m_string = "Andar_WP"
end

function mp_exit_calcwp()
	--core:trace("Saliendo Buscar_next_WP Platform");
end

function mp_update_calcwp(ElapsedTime)
	core:trace("Update Buscar_next_WP Platform");
	--mp_enter_calcwp()
end
--End Moving Platform


--Poly Platform

function update_poly_platform(current_poly_time, platform_name)
	-- Enable Poly Emmerald power
	local position = coreInstance:get_player_controller():get_position()
	local platform = rol_manager:get_default_renderable_object_manager():get_resource(platform_name)
	local platform_position = platform:get_position()
	local l_distance = position:distance(platform_position)
	if l_distance < platform.m_ActivationDistance then
		platform.m_Enabled = true
	else
		platform.m_Enabled = false
	end
	
	local act2in = coreInstance:get_action_to_input();
	
	if act2in:do_action_from_lua("PolyPowa") == true and platform.m_Enabled then
		platform:activate_poly()
		--local new_pos = Vect3f(position + platform.m_RedimScale)
		--coreInstance:get_player_controller().m_PhysicController:set_position(new_pos) 
	end
	-- If poly is activated
	
	if current_poly_time > platform.m_TimeOut then
		platform:deactivate_poly()
	end
		
end

--End Poly Platform


function get_distance_to_point(current_position, final_position)
	-- calcular distancia hacia destino
	local distance = 0
	distance = ((final_position.x - current_position.x) ^2 + (final_position.y - current_position.y) ^2 + (final_position.z - current_position.z) ^2)
	return distance
end