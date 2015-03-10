local next_wp = Vect3f(0,0,0)
local current_time = 0
ai_controller = CAIController()
ai_controller.m_Speed = 0.1
instance = CLuaGlobalsWrapper().m_CoreInstance;

core = CCoreLuaWrapper().m_CoreInstance;


function on_enter_patrulla_parado()
--	core:trace("Entering Parado");
-- OnEnter Parado 
--play animation "parado"
  --nameEnter1 = CNamed()
--core:trace("onEnter1");

  next_wp = Vect3f(10,0,10) 
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

	if next_wp == Vect3f(10,0,10) then
	--name:set_name("10")
		next_wp = Vect3f(-10, 0, -10)	
	else
	--name:set_name("-10")
		next_wp = Vect3f(10,0,10)
	end

		--core:trace(tostring(next_wp.x));
		--	name:set_name("cambio")
	instance.m_string = "Andar_WP"
end

function on_enter_patrulla_perseguir_player()
	core:trace("Entering Perseguir_PLayer");
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
	core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	current_time = 0
end

function on_exit_patrulla_perseguir_player()
--	core:trace("Saliendo Perseguir_PLayer");
-- OnExit Perseguir_PLayer 	
	current_time = 0
end

function on_update_patrulla_parado(ElapsedTime)
-- Update Parado	
--core:trace("Update Parado");
	--player_position = Vect3f(19,0,5)
	player_position = core:get_player_controller():get_position()
--	core:trace(tostring(player_position.x));
	if current_time >= 0.1 then
		instance.m_string = "Andar_WP"
	end
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance <= 25 then
		instance.m_string = "Perseguir_Player"
	end
	current_time = current_time +1 * ElapsedTime
	return 0
end

function on_update_patrulla_andar_wp(ElapsedTime)
	-- agafar player , player_position
-- Update Andar_WP	
	--andar a wp
--	core:trace("Update Andar_WP");
	--player_position = Vect3f(10,0,10)
	player_position = core:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));

	ai_controller:move_to(ElapsedTime, next_wp)
	local wp_distance = get_distance_to_player(ai_controller:get_position(), next_wp)
	if wp_distance < 25 then
		instance.m_string = "Buscar_next_WP"
	end
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance < 25 then	
		instance.m_string = "Perseguir_Player"
	end
	
end

function on_update_patrulla_buscar_next_wp(ElapsedTime)
-- Update Buscar_next_WP	
--core:trace("Update Buscar_next_WP");
	if next_wp == Vect3f(10,0,10) then

		next_wp = Vect3f(-10, 0, -10)	
	else

		next_wp = Vect3f(10,0,10)
	end
	instance.m_string = "Andar_WP"
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance < 25 then	
		instance.m_string = "Perseguir_Player"
	end
end

function on_update_patrulla_perseguir_player(ElapsedTime)
--	core:trace("Update Perseguir_Player");
-- Update Perseguir_Player 	, player_position
	-- obtener posicion actual del player
	--move to posicion del player
	--player_position = Vect3f(10,0,10)
	--player_position = core:get_player_controller():get_position()
	ai_controller:move_to(ElapsedTime, core:get_player_controller():get_position())
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance > 225 then
		instance.m_string = "Parado"
	end
	if player_distance < 1 then
		instance.m_string = "Parado"
	end
end

function get_distance_to_player(current_position, player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((player_position.x - current_position.x) ^2 + (player_position.y - current_position.y) ^2 + (player_position.z - current_position.z) ^2)
	return distance
end