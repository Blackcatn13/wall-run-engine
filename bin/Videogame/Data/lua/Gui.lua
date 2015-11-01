local GameoverCount = 0;
local gui_visible = true
local black_screen_timer = 0
local max_count_timer = 3.0
local original_num_position = gui_manager:get_position_x_element("VidesNumber")

function update_fading_black_screen()
	 if gui_manager.m_IsBlackScreenActive then
		check_death_fade()
	end
end

function check_input_controller_map()
	if inputm:has_game_pad(1) then
		gui_manager:set_visible_gui_element("KeyboardMap",false)
		gui_manager:set_visible_gui_element("ControllerMap",true)
	else
		gui_manager:set_visible_gui_element("KeyboardMap",true)
		gui_manager:set_visible_gui_element("ControllerMap",false)
	end
end

function onUpdateWindowDisplayGUI()
	if gui_manager:get_is_displayed_unlock_message() == true then
			ManagerGUIUnlockMessage();
		end
		
	if not gui_manager:get_is_paused() then
		if act2in:do_action_from_lua("ShowLifeGUI") then
			if gui_manager:get_is_displayed_heart() == false then
				gui_manager:set_is_displayed_heart(true);
			end
			if gui_manager:get_is_displayed_pixelite() == false then
				gui_manager:set_is_displayed_pixelite(true);
			end
		end
		
		--local playerRenderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name)
		
		--Si el valor de isDisplayed es TRUE quiere decir que se muestra y, al cabo de 3 segundos, se ocultará.
		if gui_manager:get_is_displayed_heart() == true then
			ManagerGUIHearts();
		end
		if gui_manager:get_is_displayed_pixelite() == true then
			ManagerGUIPixelites();
		end
		--[[if gui_manager:get_is_displayed_poly_message() == true and gui_manager:get_first_poly_message_displayed() == false then
			ManagerGUIPolyMessage();
		end]]--
		
		if gui_manager:get_is_gameover() == true then
			GameOver();
			set_visible_gui_elements(false)
		--else
			--set_visible_gui_elements(true) 
		end
		
		local use_vignetting = effect_manager:get_effect_technique("DrawVignettingTechnique").m_UseVigneting
		if cinematic_controller.m_executing  then
			if gui_visible then
				set_visible_gui_elements(false)
				gui_visible = false
			end
			if not use_vignetting then
				effect_manager:get_effect_technique("DrawVignettingTechnique").m_UseVigneting = true
			end
		else
			if not gui_visible then
				set_visible_gui_elements(true)
				gui_visible = true
			end
			
			if use_vignetting then
				effect_manager:get_effect_technique("DrawVignettingTechnique").m_UseVigneting = false
			end
		end
		
		check_death_fade()
	end	
end

function check_death_fade()
	if player.is_dead and gui_manager:is_transition_effect_active() and fade_step == 1 then
	--coreInstance:trace(tostring(gui_manager.m_sTransitionEffect.m_currentState))
		if gui_manager.m_sTransitionEffect.m_currentState > (gui_manager.m_sTransitionEffect.m_fTransitionTime *0.5) then
			
			--coreInstance:trace("Fading step 1")
			gui_manager:set_visible_gui_element("PixeliteGUI",false)
			gui_manager:set_visible_gui_element("PixeliteNumber",false)
			gui_manager:set_visible_gui_element("PixeliteNumberShadow",false)
			gui_manager:set_visible_gui_element("BlackScreen", true)
			gui_manager:set_visible_gui_element("VidesGUIDeath",true)
			--gui_manager:set_visible_gui_element("VidesNumberShadowDeath",true)
			gui_manager:set_visible_gui_element("VidesNumberDeath",true)
			fade_step = 2
			player.check_death_actions()
			if player.num_lifes == 0 then
				coreInstance:getWWSoundManager():PlayEvent("_Fail", "Music");
			end
		end
	elseif player.is_dead and not gui_manager:is_transition_effect_active() and fade_step == 2 then
		if gui_manager:get_is_displayed_heart() == false then
			--gui_manager:set_is_displayed_heart(true);
			gui_manager.m_IsBlackScreenActive = true
			show_lifes_death()
		end
		if black_screen_timer >= max_count_timer then
			black_screen_timer = 0.0
			fade_step = 3
			fade(2)
		else
			if not gui_manager:get_is_paused() then
				black_screen_timer = black_screen_timer +1 *coreInstance.m_ElapsedTime
			end
		end
	elseif player.is_dead and gui_manager:is_transition_effect_active() and fade_step == 3 then
		if gui_manager.m_sTransitionEffect.m_currentState > (gui_manager.m_sTransitionEffect.m_fTransitionTime *0.5) then	
			--coreInstance:trace("Ya toca quitar la pantalla negra")
			if gui_manager:get_visible_gui_element("VidesGUIDeath") then
				gui_manager:set_visible_gui_element("VidesGUIDeath",false)
				gui_manager:set_visible_gui_element("VidesNumberDeath",false)
			end
			
			gui_manager:set_visible_gui_element("BlackScreen", false)
			gui_manager.m_IsBlackScreenActive = false
			fade_step = 0
			gui_manager:set_visible_gui_element("PixeliteGUI",true)
			gui_manager:set_visible_gui_element("PixeliteNumber",true)
			gui_manager:set_visible_gui_element("PixeliteNumberShadow",true)
			if player.num_lifes == 0 then
				--coreInstance:getWWSoundManager():PlayEvent("_Fail", "Music");
				local game_over_pos = Vect3f(925, 0, -1.5)
				set_screen_menu(game_over_pos, 9)
				--game over
				gui_manager:set_is_gameover(true);
				player.game_over = true
			end
			player.is_dead = false
			player_reseted = false
			gui_manager:set_transition_effect_active(false)
		end
	end
end

function set_visible_gui_elements(visible)
	gui_manager:set_visible_gui_element("VidesGUI",visible)
	gui_manager:set_visible_gui_element("VidesNumber",visible)
	gui_manager:set_visible_gui_element("VidesNumberShadow",visible)
	gui_manager:set_visible_gui_element("PixeliteGUI",visible)
	gui_manager:set_visible_gui_element("PixeliteNumber",visible)
	gui_manager:set_visible_gui_element("PixeliteNumberShadow",visible)
	gui_manager:set_visible_gui_element("LifeGUI",visible)
	--coreInstance:trace("Showing GUI Elements: ".. tostring(visible))
end

function show_lifes_death()

	local count = gui_manager:get_count_heart()
	if count <= max_count_timer then
		
		if count >= max_count_timer/2 and gui_manager:get_num_heart() ~= player.num_lifes then
			gui_manager:set_num_heart( player.num_lifes )
		end
		if not gui_manager:get_is_paused() then
			gui_manager:set_count_heart(count + gui_manager:get_dt());
			--coreInstance:trace(tostring(count))
		end
	else
		
		--gui_manager:set_is_displayed_heart(false)
		gui_manager:set_count_heart(0.0)
	end

end

function ManagerGUIHearts()
	local pos = gui_manager:get_position_x_element("VidesGUI");
	local posPixelitesText = gui_manager:get_position_x_element("VidesNumber");
	
	
	local count = gui_manager:get_count_heart();
	local positionMin = 2;
	local positionMax = -20;
	local positionYGUI = 44;
	local positionYNum = 53;
	local textShadowDifference = 0.25;
	
	--Vides GUI
	if pos < positionMin and count == 0.0 then
		gui_manager:set_position_element("VidesGUI", pos + (65 * gui_manager:get_dt()), positionYGUI);
		gui_manager:set_position_element("VidesNumber", posPixelitesText + (65 * gui_manager:get_dt()), positionYNum);
		gui_manager:set_position_element("VidesNumberShadow", posPixelitesText + (65 * gui_manager:get_dt()) + textShadowDifference, positionYNum+textShadowDifference);
	else
		if not player.is_dead then
			if count >= 3.0 then
				if pos > positionMax then
					gui_manager:set_position_element("VidesGUI", pos - (25 * gui_manager:get_dt()), positionYGUI);
					gui_manager:set_position_element("VidesNumber", posPixelitesText - (25 * gui_manager:get_dt()), positionYNum);
					gui_manager:set_position_element("VidesNumberShadow", posPixelitesText - (25 * gui_manager:get_dt()) + textShadowDifference, positionYNum+textShadowDifference);
				else
					gui_manager:set_is_displayed_heart(false);
					gui_manager:set_count_heart(0.0);
				end
			else
				if count>=2.0 and pos >= positionMax and gui_manager:get_num_heart() ~= player.num_lifes then
					gui_manager:set_num_heart( player.num_lifes )
				end
				gui_manager:set_position_element("VidesGUI", positionMin, positionYGUI);
				gui_manager:set_position_element("VidesNumber", positionMin+9, positionYNum);
				gui_manager:set_position_element("VidesNumberShadow", positionMin+9+textShadowDifference, positionYNum+textShadowDifference);
				if not gui_manager:get_is_paused() then
					gui_manager:set_count_heart(count + gui_manager:get_dt());
				end
			end
		else
			gui_manager:set_position_element("VidesGUI",positionMax , positionYGUI);
			gui_manager:set_position_element("VidesNumber", original_num_position, positionYNum);
			gui_manager:set_position_element("VidesNumberShadow",original_num_position + textShadowDifference, positionYNum+textShadowDifference);
			gui_manager:set_is_displayed_heart(false);
			gui_manager:set_count_heart(0.0);
		end
	end
end

function ManagerGUIPixelites()
	local posPixelites = gui_manager:get_position_x_element("PixeliteGUI");
	local posPixelitesText = gui_manager:get_position_x_element("PixeliteNumber");
	
	local count = gui_manager:get_count_pixelite();
	
	local positionMin = 2;
	local positionMax = -20;
	local positionYGUI = 61;
	local positionYNum = 70;
	local textShadowDifference = 0.25;
		
	--Pixelite GUI
	if posPixelites < positionMin and count == 0.0 then
		gui_manager:set_position_element("PixeliteGUI", posPixelites + (65 * gui_manager:get_dt()), positionYGUI);
		gui_manager:set_position_element("PixeliteNumber", posPixelitesText + (65 * gui_manager:get_dt()), positionYNum);
		gui_manager:set_position_element("PixeliteNumberShadow", posPixelitesText + (65 * gui_manager:get_dt()) + textShadowDifference, positionYNum + textShadowDifference);
	else
		if count >= 3.0 then
			if posPixelites > positionMax then
				gui_manager:set_position_element("PixeliteGUI", posPixelites - (25 * gui_manager:get_dt()), positionYGUI);
				gui_manager:set_position_element("PixeliteNumber", posPixelitesText - (25 * gui_manager:get_dt()), positionYNum);
				gui_manager:set_position_element("PixeliteNumberShadow", posPixelitesText - (25 * gui_manager:get_dt()) + textShadowDifference, positionYNum + textShadowDifference);
			else
				gui_manager:set_is_displayed_pixelite(false);
				gui_manager:set_count_pixelite(0.0);
			end
		else
			gui_manager:set_position_element("PixeliteGUI", positionMin, positionYGUI);
			gui_manager:set_position_element("PixeliteNumber", positionMin+9, positionYNum);
			gui_manager:set_position_element("PixeliteNumberShadow", positionMin+9+textShadowDifference, positionYNum + textShadowDifference);
			gui_manager:set_count_pixelite(count + gui_manager:get_dt());
		end
	end
end

function ManagerGUIPolyMessage()
	--Poly Message GUI
	gui_manager:set_is_paused(true);
	gui_manager:set_visibility_message(true);
	-- Fins que no apretes ENTER no marxa
	gui_manager:set_is_displayed_console(true);
--	gui_manager:set_image('MessageGUI','PolyPlatform');
	if act2in:do_action_from_lua("Enter") then
		gui_manager:set_is_displayed_poly_message(false);
		gui_manager:set_is_displayed_console(false);
		gui_manager:set_first_poly_message_displayed(true);
		gui_manager:set_visibility_message(false);
		gui_manager:set_is_paused(false);
	end	
end

function ManagerGUIUnlockMessage()
	--Unlock Message GUI
	gui_manager:set_is_paused(true);
	gui_manager:set_visibility_message(true);
	-- Fins que no apretes ENTER no marxa
	gui_manager:set_is_displayed_console(true);
	if inputm:has_game_pad(1) then
		gui_manager:set_image('MessageGUI','UnlockCardController');
	else
		gui_manager:set_image('MessageGUI','UnlockCardKeyboard');
	end
	if act2in:do_action_from_lua("Enter") then
		gui_manager:set_is_displayed_unlock_message(false);
		gui_manager:set_is_displayed_console(false);
		gui_manager:set_visibility_message(false);
		gui_manager:set_is_paused(false);
	end	
end

function GameOver()
	--GameOver	
	if GameoverCount > 5 then
		GameoverCount = 0;
		reset_game();
		gui_manager:set_is_gameover(false);
		gui_manager:set_visible_mouse(true);
		CCoreLuaWrapper().m_CoreInstance:get_graphics_manager():set_is_GUI_displayed(true);
		local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(piky_layer, player_controller.m_Room):get_resource(piky_mesh_name);
		playerRenderableMesh:remove_action(8)
		gui_manager:pop_windows();
		player.can_move = false;
	else
		GameoverCount = GameoverCount + gui_manager:get_dt();
	end
end
