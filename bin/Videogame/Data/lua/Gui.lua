function onUpdateWindowDisplayGUI()
	if act2in:do_action_from_lua("ShowLifeGUI") then
		if gui_manager:get_is_displayed_heart() == false then
			gui_manager:set_is_displayed_heart(true);
		end
		if gui_manager:get_is_displayed_pixelite() == false then
			gui_manager:set_is_displayed_pixelite(true);
		end
	end
	
	--Si el valor de isDisplayed es TRUE quiere decir que se muestra y, al cabo de 3 segundos, se ocultará.
	if gui_manager:get_is_displayed_heart() == true then
		ManagerGUIHearts();
	end
	if gui_manager:get_is_displayed_pixelite() == true then
		ManagerGUIPixelites();
	end
	
end

function ManagerGUIHearts()
	local pos = gui_manager:get_position_y_element("VidesGUI");
	
	local count = gui_manager:get_count_heart();
	
	--Vides GUI
	-- Posició antiga (horitzontal per sota de la vida): x = 2 ,y = 15
	if pos > 84 and count == 0.0 then
		gui_manager:set_position_element("VidesGUI", 27, pos - (65 * gui_manager:get_dt()));
	else
		if count >= 3.0 then
			-- Valor antic: pos > -20
			if pos < 96 then
				gui_manager:set_position_element("VidesGUI", 27, pos + (25 * gui_manager:get_dt()));
			else
				gui_manager:set_is_displayed_heart(false);
				gui_manager:set_count_heart(0.0);
			end
		else
			gui_manager:set_position_element("VidesGUI", 27, 84);
			gui_manager:set_count_heart(count + gui_manager:get_dt());
		end
	end
end

function ManagerGUIPixelites()
	local posPixelites = gui_manager:get_position_y_element("PixeliteGUI");
	local posPixelitesText = gui_manager:get_position_y_element("PixeliteNumber");
	
	local count = gui_manager:get_count_pixelite();
		
	--Pixelite GUI
	-- Posició antiga (vertical a dalt la pantalla): x = 45/49.5 ,y = 4/6 (textura/text)
	if posPixelites > 84 and count == 0.0 then
		gui_manager:set_position_element("PixeliteGUI", 45, posPixelites - (65 * gui_manager:get_dt()));
		gui_manager:set_position_element("PixeliteNumber", 49.5, posPixelitesText - (65 * gui_manager:get_dt()));
	else
		if count >= 3.0 then
			-- Valor antic: posPixelites > -18
			if posPixelites < 96 then
				gui_manager:set_position_element("PixeliteGUI", 45, posPixelites + (25 * gui_manager:get_dt()));
				gui_manager:set_position_element("PixeliteNumber", 49.5, posPixelitesText + (25 * gui_manager:get_dt()));
			else
				gui_manager:set_is_displayed_pixelite(false);
				gui_manager:set_count_pixelite(0.0);
			end
		else
			gui_manager:set_position_element("PixeliteGUI", 45, 84);
			gui_manager:set_position_element("PixeliteNumber", 49.5, 86);
			gui_manager:set_count_pixelite(count + gui_manager:get_dt());
		end
	end
end
