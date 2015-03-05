next_wp = Vect3f(0,0,0)
current_time = 0
ai_controller = CAIController()
instance = CLuaGlobalsWrapper().m_CoreInstance;

core = CCoreLuaWrapper().m_CoreInstance;

function onEnter1()
	core:trace("Entering Parado");
-- OnEnter Parado 
--play animation "parado"
  nameEnter1 = CNamed()
  nameEnter1:set_name("onEnter1")
  next_wp = Vect3f(15,0,15)
  return 0 
end

function onEnter2()
	core:trace("Entering Andar_WP");
-- OnEnter Andar_WP 
	-- play animacion andar
end

function onEnter3()
-- OnEnter Buscar_next_WP 
core:trace("Entering Buscar_next_WP");
	if next_wp == Vect3f(15,0,15) then
		next_wp = Vect3f(-15, 0, -15)
	else
		next_wp = Vect3f(15,0,15)
	end
	instance.m_string = "Andar_WP"
end

function onEnter4()
	core:trace("Entering Perseguir_PLayer");
-- OnEnter Perseguir_PLayer  
end

function OnExit1()
-- OnExit Parado	
core:trace("Saliendo Parado");
	current_time = 0
end

function OnExit2()
	core:trace("Saliendo Andar_WP");
-- OnExit Andar_WP	
	current_time = 0
end

function OnExit3()
	core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	current_time = 0
end

function OnExit4()
	core:trace("Saliendo Perseguir_PLayer");
-- OnExit Perseguir_PLayer 	
	current_time = 0
end

function Update1(ElapsedTime)
-- Update Parado	
core:trace("Update Parado");
	player_position = Vect3f(50,50,50)
	
	core:trace("hi");
	if current_time >= 0.1 then
		instance.m_string = "Andar_WP"
	end
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance <= 25 then
		instance.m_string = "Perseguir_PLayer"
	end
	current_time = current_time +1 * ElapsedTime
	return 0
end

function Update2(ElapsedTime)
	-- agafar player , player_position
-- Update Andar_WP	
	--andar a wp
	core:trace("Update Andar_WP");
	player_position = Vect3f(50,50,50)
	--core:trace(tostring(player_position));
	core:trace("h0")
	ai_controller:move_to(ElapsedTime, next_wp)
	if ai_controller:get_position() == next_wp then
		instance.m_string = "Buscar_next_WP"
	end
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance < 25 then	
		instance.m_string = "Perseguir_PLayer"
	end
	
end

function Update3(ElapsedTime)
-- Update Buscar_next_WP	
core:trace("Update Buscar_next_WP");
	instance.m_string = "Andar_WP"
end

function Update4(ElapsedTime)
	core:trace("Update Perseguir_PLayer");
-- Update Perseguir_PLayer 	, player_position
	-- obtener posicion actual del player
	--move to posicion del player
	player_position = Vect3f(50,50,50)
	local player_distance = get_distance_to_player(ai_controller:get_position(), player_position)
	if player_distance > 49 then
		instance.m_string = "Parado"
	end
end

function get_distance_to_player(current_position, player_position)
	-- calcular distancia hacia player
	local distance = 0
	distance = ((player_position.x - current_position.x) ^2 + (player_position.y - current_position.y) ^2 + (player_position.z - current_position.z) ^2)
	return distance
end