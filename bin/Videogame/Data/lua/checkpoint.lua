CheckPoint = {}
--CheckPoint_mt = { __index = CheckPoint}

local last_position = Vect3f(0.0, 0.0, 0.0)
local is_activated = false
function CheckPoint.new()
   local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.is_activated = false
	self.name = ""
	------	 CHECKPOINT FUNCTIONS -----
	function self.set_checkpoint(player, trigger_name)
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		if self.is_activated == false then
			local player_controller = player.get_player_controller()
			self.last_position = player_controller:get_position()
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
		local player_controller = player:get_player_controller()
		player_controller:set_position(self.last_position)
	end
	
	return self
end


