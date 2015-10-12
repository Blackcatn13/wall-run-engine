
function roll_object(objectName, layer_name, dt)
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str(layer_name)
	local object = object_manager:get_resource(objectName)
	local new_yaw = 0
--	if object:get_visible() then
		new_yaw = object:get_yaw()+ 1* dt
		
		object:set_yaw(new_yaw)
---	end
	return new_yaw
end

function roll_collectible(objectName, layer_name, dt)
	local temp_yaw = roll_object(objectName, layer_name, dt)
	if temp_yaw ~= 0 then
		local trigger = trigger_manager:get_resource(objectName.."_UserData")
		coreInstance:trace("Collectible trigger position Antes: "..tostring(trigger:get_position().x)..", "..tostring(trigger:get_position().y)..", "..tostring(trigger:get_position().z))
		trigger:rotate_by_angles(temp_yaw)
		coreInstance:trace("Collectible trigger position Despues: "..tostring(trigger:get_position().x)..", "..tostring(trigger:get_position().y)..", "..tostring(trigger:get_position().z))
	end
end


function get_pixelite(pixelite_name, value)
	local trigger_name = pixelite_name .. "_UserData"
	local trigger = trigger_manager:get_resource(trigger_name)
	if trigger.m_IsSwitched == false then
		player.add_pixelites(tonumber(value))
		deactivate_collectible(trigger,"collectible", pixelite_name)
		
		gui_manager:set_is_displayed_pixelite(true);
		gui_manager:set_count_pixelite(0.0);
		gui_manager:set_num_pixelite(tonumber(player.pixelites));
	end
end

function check_collectibles()
	for i = 1, table.maxn(Cards_Vector) do
		if Cards_Vector[i].unlocked == true then
			local image_name = "Collectible"..tostring(i)
			coreInstance:trace(image_name)
			--gui_manager:set_image(image_name,'ImgUnLocked')
			gui_manager:set_visible_element(image_name, true);
		end
	end
end

function reset_cards()
	for i = 1, table.maxn(Cards_Vector) do
		if Cards_Vector[i].unlocked == true then
			Cards_Vector[i].unlocked = false
			local image_name = "Collectible"..tostring(i)
			coreInstance:trace(image_name)
			--gui_manager:set_image(image_name,'ImgLocked')
			gui_manager:set_visible_element(image_name, false);
		end
	end
end

function get_sticker(sticker_name, img_name)
	local trigger_name = sticker_name .. "_UserData"
	local trigger = trigger_manager:get_resource(trigger_name)
	if trigger.m_IsSwitched == false --[[and player_controller.m_isGrounded ]]then
		if player.stickers == 0 then
			gui_manager:set_is_displayed_unlock_message(true);
			if inputm:has_game_pad(1) then
				jump_enabled = false
			end
		end	
		player.add_sticker(img_name)
		coreInstance:getWWSoundManager():PlayEvent("GetCard", "Music");
		deactivate_collectible(trigger,"collectible", sticker_name)
	end
end

function deactivate_collectible (_trigger, layer_name, obj_name)
	deactivate_collectible_by_room(_trigger, layer_name, obj_name, player_controller.m_Room)
end

function activate_collectible (_trigger, layer_name, obj_name)
	activate_collectible_by_room(_trigger, layer_name, obj_name, player_controller.m_Room)
end


function deactivate_collectible_by_room (_trigger, layer_name, obj_name, room_number)
	_trigger.m_IsSwitched = true
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(layer_name, room_number)
	local object = object_manager:get_resource(obj_name)
	if object ~= nil then
		object.m_Printable = false
		object:set_visible(false);
		local emitter = object.m_ParticleEmitter
		if (emitter ~= "") then
			particle_manager:get_resource(emitter):set_visible(false);
		end
		local emitter2_name = object.m_ParticleEmitter2
		if emitter2_name ~= "" then
			local emitter2 = particle_manager:get_resource(emitter2_name)
			if emitter2 ~= nil then
				emitter2.m_vPos = object:get_position()
				emitter2.m_FireParticles = true
			end
		end
	end
end

function activate_collectible_by_room (_trigger, layer_name, obj_name, room_number)
	if(_trigger ~= nil) then
		_trigger.m_IsSwitched = false
	end
	local object_manager = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room(layer_name, room_number)
	local object = object_manager:get_resource(obj_name)
	if object ~= nil then
		object.m_Printable = true
		local emitter = object.m_ParticleEmitter
		if (emitter ~= "") then
			coreInstance:getParticleManager():get_resource(emitter):set_visible(true);
		end
	end
end

