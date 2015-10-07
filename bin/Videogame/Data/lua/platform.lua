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

	local platform = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", name, player_controller.m_Room)
	
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
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("breakable",player_controller.m_Room):get_resource(platform_name)

	if platform ~= nil and not platform.m_EmittersSet then
		coreInstance:trace("Setting Platform particles")
		local array_emitters = { particle_manager:get_resource(platform.m_ParticleEmitter), particle_manager:get_resource(platform.m_ParticleEmitter2), particle_manager:get_resource(platform.m_ParticleEmitter3), particle_manager:get_resource(platform.m_ParticleEmitter4)}
		set_emitters_visible(array_emitters, true)
		platform.m_EmittersSet = true
	end
	if current_time >= max_time then
		
		--local falling_force = Vect3f(0.0,-1.0,0.0)
		if platform ~= nil then
			platform:disable_platform(dt)--, falling_force)
			--m_current_time = 0
			local trigger = trigger_manager:get_resource(trigger_name)
			trigger.m_Time = 0.0
			local array_emitters = { particle_manager:get_resource(platform.m_ParticleEmitter), particle_manager:get_resource(platform.m_ParticleEmitter2), particle_manager:get_resource(platform.m_ParticleEmitter3), particle_manager:get_resource(platform.m_ParticleEmitter4)}
			set_emitters_visible(array_emitters, false)
			platform.m_EmittersSet = false
		end
	end
	
	if player.is_dead then
		
		local trigger = trigger_manager:get_resource(trigger_name)
		trigger.m_Time = 0.0
		trigger.set_update(false)
		trigger.activate(false)
		--local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("breakable",player_controller.m_Room):get_resource(platform_name)
		--local falling_force = Vect3f(0.0,-1.0,0.0)
		if platform ~= nil then
			platform.enable_platform()
			local array_emitters = { particle_manager:get_resource(platform.m_ParticleEmitter), particle_manager:get_resource(platform.m_ParticleEmitter2), particle_manager:get_resource(platform.m_ParticleEmitter3), particle_manager:get_resource(platform.m_ParticleEmitter4)}
			set_emitters_visible(array_emitters, false)
			platform.m_EmittersSet = false
		end
	end
	--m_current_time = m_current_time + 1
end

function restore_broken_platforms_by_layer(layer_name)
	--renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(layer_name, player_controller.m_Room):restore_broken_platforms()
	renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(layer_name, 4):restore_broken_platforms()
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

function on_enter_pinchos()
--[[
	if not player.hurt_by_spikes and not player.is_dead and not gui_manager:is_transition_effect_active() then
		player.hurt_by_spikes = true
		local renderable_piky_mesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name)
		renderable_piky_mesh:execute_action(anim_Burn,0,0.3,1,false) -- animacion de piky tostada
		local emitter3 = particle_manager:get_resource(renderable_piky_mesh.m_ParticleEmitter3)
		if emitter3:get_visible() == false then
			emitter3:set_visible(true)
			emitter3.m_vPos = renderable_piky_mesh:get_position() + renderable_piky_mesh.m_EmitterOffset3
		end
	end
]]
	player.hurt_by_spikes = true

	player.player_die()
	
end


--

-- Moving Platform
--local mov_platforms_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", player_controller.m_Room)

function activate_moving_platforms(room_name)
	--mov_platforms_manager:activate_elements(room_name)
	renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", room_name):activate_elements(room_name)
end

function mp_enter_stopped(room_name, name)
	--coreInstance:trace("Parando plataforma");
	--next_wp = Vect3f(-15,0,-15) 
	--instance.m_string = "Buscar_next_WP_Plaform"
	local platform =renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", room_name):get_resource(name)
	
	--[[if name == "Plataforma0er3" and tostring(platform.m_FirstTime) == "true" then
		local offsetPose = Vect3f(platform.m_Actor:get_position().x , platform.m_Actor:get_position().y, platform.m_Actor:get_position().z)
			coreInstance:trace("Platform position Antes: "..tostring(platform.m_Actor:get_position().x)..", "..tostring(platform.m_Actor:get_position().y)..", "..tostring(platform.m_Actor:get_position().z))
			platform.m_Actor:set_global_position(offsetPose)
			coreInstance:trace("Platform position Despues: "..tostring(platform.m_Actor:get_position().x)..", "..tostring(platform.m_Actor:get_position().y)..", "..tostring(platform.m_Actor:get_position().z))
			platform.m_FirstTime = false
	end]]
	platform:get_fsm():newState("Buscar_next_WP_Plaform")
	return 0 
end

function mp_exit_stopped()
	--coreInstance:trace("Saliendo Platform stopped");

end

function mp_update_stopped(ElapsedTime,room_name, name)
	--core:trace("Update Platform stopped");
	--instance.m_string = "Buscar_next_WP_Plaform"
	local platform =renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", room_name):get_resource(name)
	--coreInstance:trace("plataforma =: "..platform:get_name())
	if player_controller.m_Room == room then
		platform.m_Activated = true
	end
	--[[coreInstance:trace("Platform name: "..tostring(name)..", ".."Platform first time: "..tostring(platform.m_FirstTime))
	if name == "Plataforma0er3" and tostring(platform.m_FirstTime) == "true" then
		local offsetPose = Vect3f(platform.m_Actor:get_position().x , platform.m_Actor:get_position().y, platform.m_Actor:get_position().z)
			coreInstance:trace("Platform position Antes: "..tostring(platform.m_Actor:get_position().x)..", "..tostring(platform.m_Actor:get_position().y)..", "..tostring(platform.m_Actor:get_position().z))
			platform.m_Actor:set_global_position(offsetPose)
			coreInstance:trace("Platform position Despues: "..tostring(platform.m_Actor:get_position().x)..", "..tostring(platform.m_Actor:get_position().y)..", "..tostring(platform.m_Actor:get_position().z))
			platform.m_FirstTime = false
	end
	]]
	platform.m_Activated = true
	if platform.m_Activated == true then
		--coreInstance:trace("activa")
		--instance.m_string = "Buscar_next_WP_Plaform"
		platform:get_fsm():newState("Buscar_next_WP_Plaform")
	end
end

function mp_enter_moving(room_name, name)
	--coreInstance:trace("Entering Platform moving State");
end

function mp_exit_moving()
	--coreInstance:trace("Exit Moviendo Plataforma");
end

function mp_update_moving(ElapsedTime, room_name, name)
	
	--local player_position = coreInstance:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", room_name):get_resource(name)-- modificar para poder pasarlo por parametro
	if platform ~= nil then
		local next_wp = platform.m_NextWP
		
		--	coreInstance:trace("Platform position: "..tostring(platform.m_Actor:get_position().x)..", "..tostring(platform.m_Actor:get_position().y)..", "..tostring(platform.m_Actor:get_position().z))
		
		--if(act2in:do_action_from_lua("MovePlatforms") == true) then
		platform:move_to_point(ElapsedTime, next_wp, 2)
		
		--end
		--move_platform_to_point(ElapsedTime, next_wp, platform)
		local current_pos = platform:get_position()
		local distance_to_point = get_distance_to_point(current_pos, next_wp)
		
		if distance_to_point <= 4 then
			--coreInstance:trace("Destino alcanzado")
			--instance.m_string = "Parado"
			platform:get_fsm():newState("Parado")
		end
	else
		--instance.m_string = "Parado"
		platform:get_fsm():newState("Parado")
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


function mp_enter_calcwp(room_name, name) -- Pasar el nombre de la plataforma y de ahí que recoja el wp que necesita? 
-- OnEnter Buscar_next_WP 
	--coreInstance:trace("Entering Buscar Platform next WP");
	--coreInstance:trace(tostring(next_wp.x));
	--if next_wp == Vect3f(15,0,15) then
	--	next_wp = Vect3f(-15, 0, 15)	
	--else
	--	next_wp = Vect3f(15,0,15)
	--end
	
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", room_name):get_resource(name)
	--coreInstance:trace("plataforma =: "..platform:get_name())
	if platform ~= nil then
		platform.m_NextWP = platform:get_next_wp()
		--coreInstance:trace(tostring(next_wp.x));
		--instance.m_string = "Andar_WP"
		platform:get_fsm():newState("Andar_WP")
	else
		--instance.m_string = "Parado"
		platform:get_fsm():newState("Parado")
	end
end

function mp_exit_calcwp()
	--core:trace("Saliendo Buscar_next_WP Platform");
end

function mp_update_calcwp(ElapsedTime, room_name, name)
	--core:trace("Update Buscar_next_WP Platform");
	--mp_enter_calcwp()
	local platform = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("mov_platforms", room_name):get_resource(name)
	--coreInstance:trace("plataforma =: "..platform:get_name())
	if platform ~= nil then
		platform.m_NextWP = platform:get_next_wp()
		--coreInstance:trace(tostring(next_wp.x));
		--instance.m_string = "Andar_WP"
		platform:get_fsm():newState("Andar_WP")
	else
		--instance.m_string = "Parado"
		platform:get_fsm():newState("Parado")
	end
end
--End Moving Platform


--Poly Platform

function update_poly_platform(current_poly_time, dt, platform_name)
	-- Enable Poly Emmerald power
	local platform = renderable_objects_layer_manager:get_resource_from_layers_and_room("poly", "enabled_poly", platform_name, player_controller.m_Room)
	local platform_position = platform:get_position()
	local position = coreInstance:get_player_controller():get_position()
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
	if (act2in:do_action_from_lua("PolyPowa") == true and platform.m_Enabled and not platform.m_IsMoving and  platform.m_ActiveTime== 0.0) or platform.m_IsMoving == true then
		--[[local pikyRenderable = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name);
		if pikyRenderable:is_action_animation_active() == false and player.is_activating_poly == false then
			if player_controller.m_isJumping then
				pikyRenderable:remove_action(2)
				player_controller.m_isJumping = false
			end
			pikyRenderable:execute_action(7,0,0.3,1,false);
			player.is_activating_poly = true
		end
		]]
		--pikyRenderable:is_action_animation_active()
		--player_controller.m_isJumping = false
		player.is_activating_poly = true
		platform:activate_poly()
		
		--activate_poly(platform, dt)
		--local new_pos = Vect3f(position + platform.m_RedimScale)
		--coreInstance:get_player_controller().m_PhysicController:set_position(new_pos) 
	
	-- If poly is activated
	elseif current_poly_time > platform.m_TimeOut then
		platform:deactivate_poly()
		player.is_activating_poly = false
		--deactivate_poly(platform, dt)
	end
	
	platform:apply_physics_to_player(platform.m_Direction, 0.0)
	
		
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