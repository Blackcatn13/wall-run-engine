local GameoverCount = 0;

function onUpdateWindowDisplayGUI()
	if act2in:do_action_from_lua("ShowLifeGUI") then
		if gui_manager:get_is_displayed_heart() == false then
			gui_manager:set_is_displayed_heart(true);
		end
		if gui_manager:get_is_displayed_pixelite() == false then
			gui_manager:set_is_displayed_pixelite(true);
		end
	end
	
	--local playerRenderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource("Piky")
	
	--Si el valor de isDisplayed es TRUE quiere decir que se muestra y, al cabo de 3 segundos, se ocultar�.
	if gui_manager:get_is_displayed_heart() == true then
		ManagerGUIHearts();
	end
	if gui_manager:get_is_displayed_pixelite() == true then
		ManagerGUIPixelites();
	end
	--[[if gui_manager:get_is_displayed_poly_message() == true and gui_manager:get_first_poly_message_displayed() == false then
		ManagerGUIPolyMessage();
	end]]--
	if gui_manager:get_is_displayed_unlock_message() == true then
		ManagerGUIUnlockMessage();
	end
	
	if gui_manager:get_is_gameover() == true then
		GameOver();
	end
	
end

function ManagerGUIHearts()
	local pos = gui_manager:get_position_y_element("VidesGUI");
	
	local count = gui_manager:get_count_heart();
	
	local positionMin = 84;
	local positionMax = 108;
	local positionX = 27;
	
	--Vides GUI
	-- Posici� antiga (horitzontal per sota de la vida): x = 2 ,y = 15
	if pos > positionMin and count == 0.0 then
		gui_manager:set_position_element("VidesGUI", positionX, pos - (65 * gui_manager:get_dt()));
	else
		if count >= 3.0 then
			-- Valor antic: pos > -20
			if pos < positionMax then
				gui_manager:set_position_element("VidesGUI", positionX, pos + (25 * gui_manager:get_dt()));
			else
				gui_manager:set_is_displayed_heart(false);
				gui_manager:set_count_heart(0.0);
			end
		else
			gui_manager:set_position_element("VidesGUI", positionX, positionMin);
			gui_manager:set_count_heart(count + gui_manager:get_dt());
		end
	end
end

function ManagerGUIPixelites()
	local posPixelites = gui_manager:get_position_y_element("PixeliteGUI");
	local posPixelitesText = gui_manager:get_position_y_element("PixeliteNumber");
	
	local count = gui_manager:get_count_pixelite();
	
	local positionMin = 84;
	local positionMax = 108;
	local positionXGUI = 45;
	local positionXNum = 49.5;
		
	--Pixelite GUI
	-- Posici� antiga (vertical a dalt la pantalla): x = 45/49.5 ,y = 4/6 (textura/text)
	if posPixelites > positionMin and count == 0.0 then
		gui_manager:set_position_element("PixeliteGUI", positionXGUI, posPixelites - (65 * gui_manager:get_dt()));
		gui_manager:set_position_element("PixeliteNumber", positionXNum, posPixelitesText - (65 * gui_manager:get_dt()));
	else
		if count >= 3.0 then
			-- Valor antic: posPixelites > -18
			if posPixelites < positionMax then
				gui_manager:set_position_element("PixeliteGUI", positionXGUI, posPixelites + (25 * gui_manager:get_dt()));
				gui_manager:set_position_element("PixeliteNumber", positionXNum, posPixelitesText + (25 * gui_manager:get_dt()));
			else
				gui_manager:set_is_displayed_pixelite(false);
				gui_manager:set_count_pixelite(0.0);
			end
		else
			gui_manager:set_position_element("PixeliteGUI", positionXGUI, positionMin);
			gui_manager:set_position_element("PixeliteNumber", positionXNum, positionMin+2);
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
	gui_manager:set_image('MessageGUI','PolyPlatform');
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
	gui_manager:set_image('MessageGUI','UnlockCard');
	if act2in:do_action_from_lua("Enter") then
		gui_manager:set_is_displayed_unlock_message(false);
		gui_manager:set_is_displayed_console(false);
		gui_manager:set_visibility_message(false);
		gui_manager:set_is_paused(false);
	end	
end

function GameOver()
	--GameOver	
	if GameoverCount > 3 then
		GameoverCount = 0;
		reset_game();
		gui_manager:set_is_gameover(false);
		gui_manager:set_visible_mouse(true);
		graphics_manager:set_is_GUI_displayed(true);
		local playerRenderableMesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource("Piky");
		playerRenderableMesh:remove_action(8)
		gui_manager:pop_windows();
	else
		GameoverCount = GameoverCount + gui_manager:get_dt();
	end
end
