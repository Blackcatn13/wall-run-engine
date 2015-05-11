
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
	end
end

function get_sticker(sticker_name, img_name)
	local trigger_name = sticker_name .. "_UserData"
	local trigger = trigger_manager:get_resource(trigger_name)
	if trigger.m_IsSwitched == false then
		player.add_sticker(img_name)
		deactivate_collectible(trigger,"collectible", sticker_name)
	end
end

function deactivate_collectible (_trigger, layer_name, obj_name)
	_trigger.m_IsSwitched = true
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(obj_name)
	object.m_Printable = false
end