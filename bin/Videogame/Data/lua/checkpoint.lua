CheckPoint = {}
--CheckPoint_mt = { __index = CheckPoint}

local last_position = Vect3f(0.0, 0.0, 0.0)
local is_activated = false
function CheckPoint.new()
   local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.is_activated = false
	self.name = ""
	self.renderable_stand = nil
	
	--self.yaw = 0.0
	------	 CHECKPOINT FUNCTIONS -----
	function self.set_checkpoint(player, trigger_name, renderable_checkpoint)
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		if self.is_activated == false then
		
			--player_controller.m_PhysicController:set_position(self.original_position)
		--player_controller:set_yaw(self.original_yaw)
		
			--local player_controller = player.get_player_controller()
			self.last_position = player_controller.m_PhysicController:get_position()
			--self.yaw = player_controller:get_yaw()
			coreInstance:trace("checkpoint con player position")
			player.last_checkpoint = self
			coreInstance:trace("last checkpoint set")
			self.is_activated = true
			self.name = trigger_name
			self.renderable_stand = renderable_checkpoint
			if self.renderable_stand ~= nil then
				coreInstance:trace("Cambiando renderables del checkpoint")
				self.change_checkpoint_renderables()
			end
			
			coreInstance:trace("checkpoint activated")
			table.insert(player.visited_checkpoints, self)
			
			coreInstance:trace("checkpoint set")
		end
	end

	function self.load_checkpoint(player)
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
			coreInstance:trace("activating checkpoint!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1")
		--local player_controller = player:get_player_controller()
		player_controller.m_PhysicController:set_position(self.last_position)
		local active_camera = cam_Controller:get_active_camera()
		local cam_object =  active_camera.m_pObject3D
		local PlayerYaw =  - cam_object:get_yaw() + 1.57
		if player_controller.m_is3D == true then
			player_controller:set_yaw(PlayerYaw)
		else
			player_controller.m_isTurned = false
			player_controller:set_yaw(PlayerYaw + 1.57); -- 90º
		end
		
		--player_controller:set_yaw(self.yaw)
	end
	
	function self.change_checkpoint_renderables()
		if (player_controller.m_Room -1 > 0) then 
			renderable_objects_layer_manager:change_between_vectors("solid", "CheckPointEnabledStand001",player_controller.m_Room -1, player_controller.m_Room)
			renderable_objects_layer_manager:change_between_vectors("glow", "CheckPointEnabledSphere", player_controller.m_Room-1, player_controller.m_Room)
		else 
			renderable_objects_layer_manager:change_between_vectors("solid", "CheckPointEnabledStand001", player_controller.m_Room)
			renderable_objects_layer_manager:change_between_vectors("glow", "CheckPointEnabledSphere",  player_controller.m_Room)
		end
		local check_point_activated_stand = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("solid", player_controller.m_Room):get_resource("CheckPointEnabledStand001")
		local check_point_activated_sphere = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("glow", player_controller.m_Room):get_resource("CheckPointEnabledSphere")
		self.renderable_stand:set_visible(false)
		self.renderable_stand.m_Printable = false
		check_point_activated_sphere:set_position(self.renderable_stand:get_position())
		check_point_activated_stand:set_position(self.renderable_stand:get_position())
		check_point_activated_stand:set_yaw(self.renderable_stand:get_yaw())
			
	end
	
	return self
end


