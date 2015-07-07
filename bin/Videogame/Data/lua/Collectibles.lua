
function roll_object(objectName, layer_name, dt)
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(objectName)
	local new_yaw = object:get_yaw()+ 1* dt
	object:set_yaw(new_yaw)
end

function get_pixelite(pixelite_name)
	local trigger_name = pixelite_name .. "_UserData"
	local trigger = trigger_manager:get_resource(trigger_name)
	if trigger.m_IsSwitched == false then
		player.add_pixelites(1)
		deactivate_collectible(trigger,"collectible", pixelite_name)
		
		gui_manager:set_is_displayed_pixelite(true);
		gui_manager:set_count_pixelite(0.0);
		gui_manager:set_num_pixelite( gui_manager:get_num_pixelite() + 1 );
	end
end

function check_collectibles()
	for i = 1, table.maxn(Cards_Vector) do
		if Cards_Vector[i].unlocked == true then
			local image_name = "Collectible"..tostring(i)
			coreInstance:trace(image_name)
			gui_manager:set_image(image_name,'ImgUnLocked')
		end
	end
end

function get_sticker(sticker_name, img_name)
	local trigger_name = sticker_name .. "_UserData"
	local trigger = trigger_manager:get_resource(trigger_name)
	if trigger.m_IsSwitched == false and player_controller.m_isGrounded then
		player.add_sticker(img_name)
		deactivate_collectible(trigger,"collectible", sticker_name)
		gui_manager:set_is_displayed_unlock_message(true);
	end
end

function deactivate_collectible (_trigger, layer_name, obj_name)
	_trigger.m_IsSwitched = true
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(obj_name)
	object.m_Printable = false
	object:set_visible(false);
	local emitter = object.m_ParticleEmitter
	if (emitter ~= "") then
		coreInstance:getParticleManager():get_resource(emitter):set_visible(false);
	end
end

function activate_collectible (_trigger, layer_name, obj_name)
	_trigger.m_IsSwitched = false
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(obj_name)
	object.m_Printable = true
	local emitter = object.m_ParticleEmitter
	if (emitter ~= "") then
		coreInstance:getParticleManager():get_resource(emitter):set_visible(true);
	end
end

