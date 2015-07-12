CheckPoint = {}
--CheckPoint_mt = { __index = CheckPoint}

local last_position = Vect3f(0.0, 0.0, 0.0)
local is_activated = false
function CheckPoint.new()
   local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.is_activated = false
	self.name = ""
	--self.yaw = 0.0
	------	 CHECKPOINT FUNCTIONS -----
	function self.set_checkpoint(player, trigger_name)
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
	
	return self
end


