function onUpdateWindowDisplayGUI()
	if act2in:do_action_from_lua("ShowLifeGUI") then
		if gui_manager:get_is_displayed() == false then
			gui_manager:set_is_displayed(true);
		end
	end
	
	local pos = gui_manager:get_position_x_element("VidesGUI");
	
	--Si el valor de isDisplayed es TRUE quiere decir que se muestra y, al cabo de 3 segundos, se ocultará.
	if gui_manager:get_is_displayed() == true then
		local count = gui_manager:get_count();
		if pos < 2 and count == 0.0 then
			gui_manager:set_position_element("VidesGUI", pos + (65 * gui_manager:get_dt()), 15);
		else
			if count >= 3.0 then
				if pos > -20 then
					gui_manager:set_position_element("VidesGUI", pos - (25 * gui_manager:get_dt()), 15);
				else
					gui_manager:set_is_displayed(false);
					gui_manager:set_count(0.0);
				end
			else
				gui_manager:set_count(count + gui_manager:get_dt());
			end
		end
	end
end
