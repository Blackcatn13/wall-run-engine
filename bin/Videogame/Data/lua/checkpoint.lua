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
	self.camera_waypoint = nil
	
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
			player.last_checkpoint = self
			self.is_activated = true
			self.name = trigger_name
			self.renderable_stand = renderable_checkpoint
			if self.renderable_stand ~= nil then
				self.change_checkpoint_renderables()
			end
			
			--Camera Waypoint
			local cam = cam_Controller:get_active_camera()
			local temp_waypoint = cam.m_CurrentWaypoint
			if temp_waypoint ~= nil and temp_waypoint > 0 then
				 temp_waypoint = temp_waypoint -1
			end
			if (cam.m_eTypeCamera == 5 or cam.m_eTypeCamera == 6) and cam.m_currentWaypoint ~= nil  then
				self.camera_waypoint = temp_waypoint
			end
			table.insert(player.visited_checkpoints, self)
		end
	end

	function self.load_checkpoint(player)
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		--local player_controller = player:get_player_controller()
		player_controller.m_PhysicController:set_position(self.last_position)
		local active_camera = cam_Controller:get_active_camera()
		--Restore Camera Waypoint
		if (active_camera.m_eTypeCamera == 5 or active_camera.m_eTypeCamera == 6) and self.camera_waypoint ~= nil then
			active_camera.m_currentWaypoint	= self.camera_waypoint 
		end
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
		local check_point_activated_stand = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("solid", self.renderable_stand.m_Room):get_resource("CheckPointEnabledStand001")
		
		if (check_point_activated_stand == nil ) then 
			local previous_room = last_checkpoint_room
			--if last_checkpoint_room == nil then
				--previous_room = self.renderable_stand.m_Room -1
			--end
			renderable_objects_layer_manager:change_between_vectors("solid", "CheckPointEnabledStand001",previous_room, self.renderable_stand.m_Room)
			renderable_objects_layer_manager:change_between_vectors("glow", "CheckPointEnabledSphere",previous_room, self.renderable_stand.m_Room)
		
			last_checkpoint_room = self.renderable_stand.m_Room
			--coreInstance:trace("Last CheckPoint Room: "..tostring(last_checkpoint_room))
			check_point_activated_stand = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("solid", self.renderable_stand.m_Room):get_resource("CheckPointEnabledStand001")
		end
		
		local check_point_activated_sphere = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("glow", self.renderable_stand.m_Room):get_resource("CheckPointEnabledSphere")

		self.renderable_stand:set_visible(false)
		self.renderable_stand.m_Printable = false
		if(check_point_activated_sphere ~= nil) then
			check_point_activated_sphere:set_position(self.renderable_stand:get_position())
			check_point_activated_stand:set_position(self.renderable_stand:get_position())
			check_point_activated_stand:set_yaw(self.renderable_stand:get_yaw())
		end
			
	end
	
	return self
end

function reset_checkpoints()
	for i = 1, table.getn(player.visited_checkpoints) do
		local renderable = player.visited_checkpoints[i].renderable_stand
		if renderable ~= nil then
			renderable:set_visible(true)
			renderable.m_Printable = true
			
		end
		--if i == table.getn(player.visited_checkpoints) then
				
		--	end
	end
	renderable_objects_layer_manager:change_between_vectors("solid", "CheckPointEnabledStand001", last_checkpoint_room, 1)
	renderable_objects_layer_manager:change_between_vectors("glow", "CheckPointEnabledSphere", last_checkpoint_room, 1)
	last_checkpoint_room = 1
	player.last_checkpoint = nil
	clear_array(player.visited_checkpoints)
end

