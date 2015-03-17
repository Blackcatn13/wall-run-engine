local coreInstance = CCoreLuaWrapper().m_CoreInstance
local physx_manager = coreInstance:get_phisics_manager()
local rol_manager = coreInstance:get_renderable_object_layer_manager()
local falling_force = Vect3f(0.0,-2.0,0.0)

function init_platform(name, user_data_name, size, position)
	local platform = rol_manager:get_default_renderable_object_manager():get_resource(name)
	platform:insert_platform(user_data_name, size, position)
	coreInstance:trace(name)
	return platform
end

function init_move_platform(name, user_data_name, size, position)
	local platform = init_platform(name, user_data_name, size, position)
	--platform:add_box_controller(Vect3f(3,2,3),0.87,0.1,0.3,0.0)
	--m_PhysicController = new CPhysicController(1, 2, 0.87, 0.1, 0.3, ECG_DYNAMIC_OBJECTS, m_PlatformUserData, m_PlatorformActor->GetPosition(), .0f);
    --PHYSXM->AddPhysicController(platorm.m_PhysicController);
end

function init_poly_platform(name, user_data_name, size, position, time_out)
	local platform = init_platform(name, user_data_name, size, position)
	--coreInstance:trace(tostring(platform.m_Collission))
	local collision = true
	if platform.m_Collission == 0 then
		collision = false
	end
	coreInstance:trace(tostring(collision))
	platform.m_PlatformActor:activate(collision)
	platform.m_OriginalScale = platform:get_scale()
	platform.m_TimeOut= time_out
	return platform
end

function update_break_platform(dt, current_time, max_time, platform_name)

	if current_time >= max_time then
		local platform = rol_manager:get_default_renderable_object_manager():get_resource(platform_name)
		platform:disable_platform(dt, Vect3f(falling_force))
		--break_platform(platform, dt)
	end
end

function break_platform(_platform, dt)
	coreInstance:trace("breaking platform")
	_platform.m_PlatformUserData:set_paint(false)
	coreInstance:trace("actor plataforma borrado")
	_platform.m_PlatformActor:activate(false)
	coreInstance:trace("plataforma desactivada")
	_platform.m_Printable = false
	coreInstance:trace("plataforma borrada")
	local trigger_manager = coreInstance:get_trigger_manager()
	local trigger = trigger_manager:get_resource(_platform.m_TriggerName)
	coreInstance:trace("Tenemos trigger")
	trigger:set_update(false)
	coreInstance:trace("Trigger update")
	trigger:activate(false)
	coreInstance:trace("Trigger activado")
	local player_controller = coreInstance:get_player_controller()
	coreInstance:trace("tenemos controller")
	local position = player_controller:get_position()
	coreInstance:trace("get position")
	coreInstance:get_player_controller().m_PhysicController():move(falling_force, dt)
	coreInstance:trace("apply force")
--[[
	m_PlatformUserData->SetPaint(false);
    m_PlatorformActor->Activate(false);
    m_Printable = false;
    TRIGGM->GetResource(m_TriggerName)->setUpdate(false);
    TRIGGM->GetResource(m_TriggerName)->Activate(false);
    Vect3f l_CurrentPosition = Vect3f (CCORE->GetPlayerController()->GetPosition());
    CCORE->GetPlayerController()->getPhysicController()->Move(Vect3f(.0f, -1.f, .0f), dt);
]]--
end

--Pinchos

function on_enter_pinchos(platform_name)
	--TODO registrar todo lo nuevo
	coreInstance:trace("Entrando en Pinchos")
	local platform = rol_manager:get_default_renderable_object_manager():get_resource(platform_name)
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

end


--

-- Moving Platform
local next_wp = Vect3f(0,0,0)
instance = CLuaGlobalsWrapper().m_CoreInstance;

function mp_enter_stopped()
	--coreInstance:trace("Parando plataforma");
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
		--coreInstance:trace("Destino alcanzado")
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
	--coreInstance:trace("Entering Buscar Platform next WP");
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
	--core:trace("Update Buscar_next_WP Platform");
	--mp_enter_calcwp()
end
--End Moving Platform


--Poly Platform

function update_poly_platform(current_poly_time, dt, platform_name)
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
		platform:activate_poly(falling_force)
		
		--activate_poly(platform, dt)
		--local new_pos = Vect3f(position + platform.m_RedimScale)
		--coreInstance:get_player_controller().m_PhysicController:set_position(new_pos) 
	end
	-- If poly is activated
	
	if current_poly_time > platform.m_TimeOut then
		platform:deactivate_poly(falling_force)
	end
		
end

--[[function activate_poly(_platform, dt)
	if _platform.m_Activated == false then
		core:trace("Activating poly")
		local new_scale = 0.0
		_platform.m_Activated = true
		core:trace(tostring(_platform.m_Collission))
		if _platform.m_Collission == 1 then
			core:trace("Platform with collision")
			new_scale = 0
			_platform.m_PlatformActor:activate(false)
			_platform.m_Collission = false
			_platform:get_redim_axis() 
			core:trace("Platform with collision")
			_platform:set_redim_axis("lalala")  
			local redim_axis = _platform:get_redim_axis()
			core:trace("lolo")
			if _platform:get_redim_axis()=="y" then
				core:trace("Redim y")
				coreInstance:get_player_controller().m_PhysicController:move(Vect3f(0.0,falling_force,0.0), dt)
			end
		else
			core:trace("Platform without collision")
			new_scale = _platform.m_RedimScale
			_platform.m_Activated:activate(true)
			_platform.m_Collission = true
		
		end
		if _platform.m_RedimAxis =="x" then
			_platform:set_scale(Vect3f(new_scale, _platform:get_scale().y, _platform:get_scale().z))
		elseif _platform.m_RedimAxis =="y" then 
			_platform:set_scale(Vect3f(_platform:get_scale().x, new_scale, _platform:get_scale().z))
		else
			_platform:set_scale(Vect3f(_platform:get_scale().x, _platform:get_scale().y, new_scale))
		end
		
	end

end
]]--
--End Poly Platform


function get_distance_to_point(current_position, final_position)
	-- calcular distancia hacia destino
	local distance = 0
	distance = ((final_position.x - current_position.x) ^2 + (final_position.y - current_position.y) ^2 + (final_position.z - current_position.z) ^2)
	return distance
end