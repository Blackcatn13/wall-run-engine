--local coreInstance = CCoreLuaWrapper().m_CoreInstance
--local physx_manager = coreInstance:get_phisics_manager()
local renderable_objects_layer_manager = coreInstance:get_renderable_object_layer_manager()
local falling_force = Vect3f(0.0, -2.0, 0.0)

--[[function init_platform(name, user_data_name, size, position)
	local platform = renderable_objects_layer_manager:get_default_renderable_object_manager():get_resource(name)
	platform:insert_platform(user_data_name, size, position)
	coreInstance:trace(name)
	return platform
end

function init_move_platform(name, user_data_name, size, position)
	local platform = init_platform(name, user_data_name, size, position)
	--platform:add_box_controller(Vect3f(3,2,3),0.87,0.1,0.3,0.0)
	--m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_DYNAMIC_OBJECTS, m_PlatformUserData, m_PlatorformActor->GetPosition(), .0f);
    --PHYSXM->AddPhysicController(platorm.m_PhysicController);
end --]]
--[[function set_platforms_lights()
	

	coreInstance:trace("A poner luces")
	local poly_platforms =  renderable_objects_layer_manager:get_renderable_objects_manager_by_str("poly")
		coreInstance:trace("Tenemos El rob manager")
	local poly_vector = poly_platforms:get_resource_vector()
		coreInstance:trace("Tenemos El vector")
	local light_manager = coreInstance:get_light_manager()
	coreInstance:trace("Tenemos los managers")
	for i = 0,  poly_platforms:get_size() do
		coreInstance:trace(poly_vector[i].m_LightName)
		poly_vector[i].m_Light = light_manager:get_resource(poly_vector[i].m_LightName)
		if poly_vector[i].m_Light ~= nil then
			poly_vector[i].m_LightOriginalPosition = poly_vector[i].m_Light:get_position()
		end
	end
	
end
--]]
function init_poly_platform(name, user_data_name, size, position, time_out, speed)
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str("poly"):get_resource(name)
	if platform == nil then
		platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str("enabled_poly"):get_resource(name)
	end		
	platform:insert_platform(user_data_name, size, position)
	--light = coreInstance:get_light_manager():get_resource(platform.m_LightName)
	platform.m_Light = coreInstance:get_light_manager():get_resource(platform.m_LightName)
	if platform.m_Light ~= nil then
		platform.m_LightOriginalPosition = platform.m_Light:get_position()
	end
	--coreInstance:trace(tostring(platform.m_Collission))
	local collision = true
	if platform.m_Collission == 0 then
		collision = false
	end
	coreInstance:trace(tostring(collision))
	--platform.m_PlatformActor:activate(collision)
	platform.m_OriginalScale = platform:get_scale()
	platform.m_TimeOut= time_out
	platform.m_Speed = speed
	return platform
end
--local m_current_time = 0
function update_break_platform(dt, current_time, max_time, platform_name, trigger_name)
	--coreInstance:trace("m_current_time " .. tostring(m_current_time))
	--local l_time = m_current_time * dt
	if current_time >= max_time then
		local platform = renderable_objects_layer_manager:get_default_renderable_object_manager():get_resource(platform_name)
		--local falling_force = Vect3f(0.0,-1.0,0.0)
		platform:disable_platform(dt)--, falling_force)
		--m_current_time = 0
		local trigger = trigger_manager:get_resource(trigger_name)
		trigger.m_Time = 0.0
	end
	--m_current_time = m_current_time + 1
end

function restore_broken_platforms_by_layer(layer_name)
	renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name):restore_broken_platforms()
end

--[[function enable_breaking_platform(name)
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str("solid"):get_resource(name)
	if platform ~= nil then
		platform.m_Actor:get_user_data().m_bPaintPhysicObject = false
		platform.m_Actor:activate(true)
		platform.m_Printable = true
		local trigger = trigger_manager:get_resource("Break1")
		if trigger ~= nil then
			--trigger:set_update(true)
			trigger:activate(true)
		end
	end
end
--]]
--Pinchos

function on_enter_pinchos(platform_name)
	--TODO registrar todo lo nuevo
	--[[coreInstance:trace("Entrando en Pinchos")
	local platform = renderable_objects_layer_manager:get_default_renderable_object_manager():get_resource(platform_name)
	coreInstance:trace(platform_name)
	--platform:falling_into_platform()
	--calcular direccion del salto
	
	local player = coreInstance:get_player_controller()
	local player_position = player.m_PhysicController:get_position()
	local platform_position = platform:get_position()
	coreInstance:trace(tostring(player_position.x))
	coreInstance:trace(tostring(platform_position.x))
	
	if platform.m_FromX == true then
		direction = platform_position.x - player_position.x
	end
	if platform.m_FromZ == true then
		direction = platform_position.z - player_position.z
	end
	coreInstance:trace(tostring(direction))
	-- <- set position atrás 
	-- -> set position delante
	-- lanzar animacion de daño
	-- restar corazón
	if direction <= 0 then
		player.m_PhysicController:set_position(platform.m_FrontPos)
	else
		player.m_PhysicController:set_position(platform.m_BackPos)
	end
	--]]
	--local player = Player:get_instance()
	coreInstance:trace(tostring(player.num_hearts))
	if player.num_hearts > 0 then
		player.get_player_controller():set_position(player.last_spikes_position)
	end
	player.player_take_damage(Vect3f(0,0,0), 0)
	
	-- Hacer cosas
	
end


--

-- Moving Platform
local mov_platforms_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str("mov_platforms")

function activate_moving_platforms(room_name)
	mov_platforms_manager:activate_elements(room_name)
end

function mp_enter_stopped(name)
	--coreInstance:trace("Parando plataforma");
	--next_wp = Vect3f(-15,0,-15) 
	instance.m_string = "Buscar_next_WP_Plaform"
	return 0 
end

function mp_exit_stopped()
	--coreInstance:trace("Saliendo Platform stopped");

end

function mp_update_stopped(ElapsedTime, name)
	--core:trace("Update Platform stopped");
	--instance.m_string = "Buscar_next_WP_Plaform"
	local platform = mov_platforms_manager:get_resource(name)
	if platform.m_Activated == true then
		instance.m_string = "Buscar_next_WP_Plaform"
	end
end

function mp_enter_moving(name)
	--coreInstance:trace("Entering Platform moving State");
end

function mp_exit_moving()
	--coreInstance:trace("Exit Moviendo Plataforma");
end

function mp_update_moving(ElapsedTime, name)
	
	--local player_position = coreInstance:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
	local platform = mov_platforms_manager:get_resource(name)-- modificar para poder pasarlo por parametro
	if platform ~= nil then
		local next_wp = platform.m_NextWP
		platform:move_to_point(ElapsedTime, next_wp, 2)
		--move_platform_to_point(ElapsedTime, next_wp, platform)
		local current_pos = platform:get_position()
		local distance_to_point = get_distance_to_point(current_pos, next_wp)
		if distance_to_point <= 4 then
			--coreInstance:trace("Destino alcanzado")
			instance.m_string = "Parado"
		end
	else
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


function mp_enter_calcwp(name) -- Pasar el nombre de la plataforma y de ahí que recoja el wp que necesita? 
-- OnEnter Buscar_next_WP 
	--coreInstance:trace("Entering Buscar Platform next WP");
	--coreInstance:trace(tostring(next_wp.x));
	--if next_wp == Vect3f(15,0,15) then
	--	next_wp = Vect3f(-15, 0, 15)	
	--else
	--	next_wp = Vect3f(15,0,15)
	--end
	
	local platform = mov_platforms_manager:get_resource(name)
	if platform ~= nil then
		platform.m_NextWP = platform:get_next_wp()
		--coreInstance:trace(tostring(next_wp.x));
		instance.m_string = "Andar_WP"
	else
		instance.m_string = "Parado"
	end
end

function mp_exit_calcwp()
	--core:trace("Saliendo Buscar_next_WP Platform");
end

function mp_update_calcwp(ElapsedTime, name)
	--core:trace("Update Buscar_next_WP Platform");
	--mp_enter_calcwp()
end
--End Moving Platform


--Poly Platform

function update_poly_platform(current_poly_time, dt, platform_name)
	-- Enable Poly Emmerald power
	local position = coreInstance:get_player_controller():get_position()
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str("poly"):get_resource(platform_name)
	if platform == nil then
		platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str("enabled_poly"):get_resource(platform_name)
	end
	local platform_position = platform:get_position()
	local l_distance = position:distance(platform_position)
	--[[coreInstance:trace("________________________________________________________")
	coreInstance:trace(tostring(platform.m_Enabled))
	coreInstance:trace("________________________________________________________")--]]
	if l_distance < platform.m_ActivationDistance then
		platform.m_Enabled = true
	else
		platform.m_Enabled = false
	end
	
	--local act2in = coreInstance:get_action_to_input();
	if (act2in:do_action_from_lua("PolyPowa") == true and platform.m_Enabled) or platform.m_IsMoving == true then
		platform:activate_poly()
		
		--activate_poly(platform, dt)
		--local new_pos = Vect3f(position + platform.m_RedimScale)
		--coreInstance:get_player_controller().m_PhysicController:set_position(new_pos) 
	
	-- If poly is activated
	elseif current_poly_time > platform.m_TimeOut then
		platform:deactivate_poly()
		--deactivate_poly(platform, dt)
	else
		platform:apply_physics_to_player(platform.m_Direction, 0.0)
	end
		
end

function activate_poly(_platform, dt)
	if _platform.m_Activated == false then
		max_distance = _platform.m_OriginalPosition:distance(_platform.m_FinalPosition)

		if _platform:get_position():distance(_platform.m_FinalPosition) >= 0.1 and _platform:get_position():distance(_platform.m_FinalPosition) <= max_distance then
			local new_position = _platform:get_position() + (_platform.m_Direction * _platform.m_Speed * dt)
			_platform.m_Actor:set_global_position(new_position)
			_platform:set_position(new_position)
			--_platform.m_Light:set_position(new_position)
			_platform.m_IsMoving = true

			player_controller:move(_platform.m_Direction * _platform.m_Speed * dt, dt)
			-- Si colisiona con piky (o si debería bajar) que lo desplace
		else
			--_platform.m_Light:set_position(_platform.m_FinalPosition)
			_platform:set_position(_platform.m_FinalPosition)
			_platform.m_Activated = true
			_platform.m_IsMoving = false
		end
	
	end
end

function deactivate_poly(_platform, dt)
	if _platform.m_Activated == true then
		max_distance = _platform.m_OriginalPosition:distance(_platform.m_FinalPosition)
		if _platform:get_position():distance(_platform.m_OriginalPosition) >= 0.1 and _platform:get_position():distance(_platform.m_OriginalPosition) <= max_distance+0.1 then
			local new_position = _platform:get_position() + (_platform.m_Direction * _platform.m_Speed * dt * -1)
			_platform.m_Actor:set_global_position(new_position)
			_platform:set_position(new_position)
			--_platform.m_Light:set_position(new_position)
			--Si colisiona (o si debería bajar)con Piky => Desplazarle
		else
			--_platform.m_Light:set_position(_platform.m_LightOriginalPosition)
			_platform:set_position(_platform.m_OriginalPosition)
			_platform.m_Activated = false
			_platform.m_ActiveTime = 0.0
		end
	end
--[[
 if (m_Activated) {
        if (m_Position.Distance(m_OriginalPosition) >= 0.5) {
            Vect3f l_NewPosition =  m_Position + ( m_Direction * m_Speed * m_Dt * -1);
            m_PlatorformActor->SetGlobalPosition(l_NewPosition);
            m_Position = l_NewPosition;
            //Si colisiona con Piky => Desplazarle
        } else {
            m_Activated = false;
            m_ActiveTime = 0.0f;
        }
--]]

end
--End Poly Platform


function get_distance_to_point(current_position, final_position)
	-- calcular distancia hacia destino
	local distance = 0
	distance = ((final_position.x - current_position.x) ^2 + (final_position.y - current_position.y) ^2 + (final_position.z - current_position.z) ^2)
	return distance
end