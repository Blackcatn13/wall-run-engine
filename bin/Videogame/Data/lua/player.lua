Player = {}

local num_hearts = 0
local num_lives = 0
local instance = 0
local last_checkpoint = nil

function Player.new()
    local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.num_hearts = 3
	self.num_lives = 3
	self.instance = self
	
	------	 PLAYER FUNCTIONS -----
	function self.get_player_controller()
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		local player_controller = coreInstance:get_player_controller().m_PhysicController;
		return player_controller 
	end

	function self.player_damage()
		num_hearts = num_hearts -1
		if num_hearts == 0 then
			self.player_die()
		end
	end

	function self.player_die()
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		self.num_lives = self.num_lives -1
		if self.num_lives == 0 then
			--game over
			coreInstance:trace("game over")
		else
			--actualizar gui
			--cargar ultimo chekpoint
			if self.last_checkpoint ~= nil then
				self.last_checkpoint.load_checkpoint(self)
				local coreInstance = CCoreLuaWrapper().m_CoreInstance;
				coreInstance:trace("checkpoint loaded")
			else 
				
				coreInstance:trace("reset level")
				--reset level platforms + enemies
			end
			
		end

	end
    return self
end
-----	GETTERS & SETTERS -----	
function Player:get_instance()
	if instance == 0 then
		instance = Player.new()
	end

	return instance
end

