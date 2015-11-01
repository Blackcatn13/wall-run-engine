local next_wp = Vect3f(0,0,0)
local current_time = 0
ai_controller = CAIController()
ai_controller.m_Speed = 0.1
instance = CLuaGlobalsWrapper().m_CoreInstance;

core = CCoreLuaWrapper().m_CoreInstance;
local wp_manager = core:get_wp_manager()
local id_next_wp
local next_wp
local id_destino_wp
local id_path_next_wp

function on_enter_patrulla_parado()
--	core:trace("Entering Parado");
-- OnEnter Parado 
--play animation "parado"
  --nameEnter1 = CNamed()
--core:trace("onEnter1");
  id_next_wp = wp_manager:find_closest_waypoint("sala_principal", ai_controller:get_position())
  next_wp = wp_manager:get_waypoint_position(id_next_wp, "sala_principal")
  return 0 
end

function on_enter_patrulla_andar_wp()
--	core:trace("Entering Andar_WP");
-- OnEnter Andar_WP 
	-- play animacion andar
end

function on_enter_patrulla_buscar_next_wp()
-- OnEnter Buscar_next_WP 
--core:trace("Entering Buscar_next_WP");
--core:trace(tostring(next_wp.x));
--local name = CNamed()
	--core:trace("Creando wpManager ");
	id_destino_wp = wp_manager:find_closest_waypoint("sala_principal", core:get_player_controller():get_position())
	--destino_wp = wp_manager:get_waypoint_position(id_destino_wp, "sala_principal")
	 
	id_path_next_wp = wp_manager:calcular_siguiente_waypoint(id_next_wp,id_destino_wp,"sala_principal")
	id_next_wp = id_path_next_wp
	next_wp = wp_manager:get_waypoint_position(id_next_wp, "sala_principal")
	
	--core:trace(tostring(resultado))
	
	--if next_wp == Vect3f(10,0,10) then
	--name:set_name("10")
		--next_wp = Vect3f(-10, 0, -10)	
	--else
	--name:set_name("-10")
		--next_wp = Vect3f(10,0,10)
	--end

		--core:trace(tostring(next_wp.x));
		--	name:set_name("cambio")
	instance.m_string = "Andar_WP"
end

function on_enter_patrulla_perseguir_player()
-- OnEnter Perseguir_PLayer  
end

function on_exit_patrulla_parado()
-- OnExit Parado	
--core:trace("Saliendo Parado");
	current_time = 0
end

function on_exit_patrulla_andar_wp()
--	core:trace("Saliendo Andar_WP");
-- OnExit Andar_WP	
	instance.m_string = "Buscar_next_WP"
	current_time = 0
end

function on_exit_patrulla_buscar_next_wp()
-- OnExit Buscar_next_WP
	current_time = 0
end

function on_exit_patrulla_perseguir_player()
--	core:trace("Saliendo Perseguir_PLayer");
-- OnExit Perseguir_PLayer 	
	current_time = 0
end

function on_update_patrulla_parado(ElapsedTime, doComprobation)
-- Update Parado	
--core:trace("Update Parado");
	--player_position = Vect3f(19,0,5)
	if doComprobation == 1 then
		local player_position = core:get_player_controller():get_position()
	--	core:trace(tostring(player_position.x));
		local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
		if player_distance <= 49 then
			instance.m_string = "Perseguir_Player"
		elseif current_time >= 0.1 then
			instance.m_string = "Andar_WP"
		end
		
	end
	current_time = current_time +1 * ElapsedTime
	return 0
end

function on_update_patrulla_andar_wp(ElapsedTime, doComprobation)
	-- agafar player , player_position
-- Update Andar_WP	
	--andar a wp
--	core:trace("Update Andar_WP");
	--player_position = Vect3f(10,0,10)
	ai_controller:move_to(ElapsedTime, next_wp)
	
	local player_position = core:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
	if doComprobation == 1 then
		local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
		if player_distance < 49 then	
			instance.m_string = "Perseguir_Player"
		else
			local wp_distance = get_distance_to_player(ai_controller:get_position(), next_wp)
			if wp_distance < 16 then
				instance.m_string = "Buscar_next_WP"
			end
		end
		
	end
end

function on_update_patrulla_buscar_next_wp(ElapsedTime, doComprobation)
-- Update Buscar_next_WP	
--core:trace("Update Buscar_next_WP");
	local player_position = core:get_player_controller():get_position()
	if doComprobation == 1 then
		local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
		if player_distance < 49 then	
			instance.m_string = "Perseguir_Player"
		else
			id_destino_wp = wp_manager:find_closest_waypoint("sala_principal", core:get_player_controller():get_position())
			id_origen_wp = wp_manager:find_closest_waypoint("sala_principal", ai_controller:get_position())
			local origen_wp = wp_manager:get_waypoint_position(id_origen_wp, "sala_principal")
			local wp_distance = get_distance_to_player(ai_controller:get_position(), origen_wp)
			
			if wp_distance > 25 then
				origen_wp = wp_manager:get_waypoint_position(id_origen_wp, "sala_principal")
				next_wp = origen_wp
			else
				id_next_wp = wp_manager:calcular_siguiente_waypoint(id_origen_wp,id_destino_wp,"sala_principal")
				next_wp = wp_manager:get_waypoint_position(id_next_wp, "sala_principal")
			end
			
			instance.m_string = "Andar_WP"
		end
		
	end
end

function on_update_patrulla_perseguir_player(ElapsedTime, doComprobation)
--	core:trace("Update Perseguir_Player");
-- Update Perseguir_Player 	, player_position
	-- obtener posicion actual del player
	--move to posicion del player
	--player_position = Vect3f(10,0,10)
	local player_position = core:get_player_controller():get_position()
	ai_controller:move_to(ElapsedTime, core:get_player_controller():get_position())
	if doComprobation == 1 then
		local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
		if player_distance > 225 then
			instance.m_string = "Parado"
		end
		if player_distance < 1 then
			instance.m_string = "Parado"
		end
	end
end

function get_distance_to_player(current_position, _player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((_player_position.x - current_position.x) ^2 + (_player_position.y - current_position.y) ^2 + (_player_position.z - current_position.z) ^2)
	return distance
end