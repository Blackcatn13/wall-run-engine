Player = {}

local num_hearts = 0
local num_lives = 0
local instance = 0
local last_checkpoint = nil
local last_spikes_position = nil


function Player.new()
    local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.MAXHEARTS = 3
	self.MAXLIVES = 3
	self.num_hearts = self.MAXHEARTS
	self.num_lives = self.MAXLIVES
	self.instance = self
	self.last_spikes_position = Vect3f(0.0, 0.0, 0.0)
	self.coreInstance = CCoreLuaWrapper().m_CoreInstance;
	
	------	 PLAYER FUNCTIONS -----
	function self.get_player_controller()
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		local player_controller = coreInstance:get_player_controller().m_PhysicController;
		return player_controller 
	end

	function self.player_take_damage()
		self.num_hearts = self.num_hearts -1
		
		self.coreInstance:trace("Taking damage")
		if self.num_hearts == 0 then
			self.player_die()
		end
	end

	function self.player_die()
		--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		self.coreInstance:trace("player dies")
		self.num_lives = self.num_lives -1
		if self.num_lives == 0 then
			--game over
			self.coreInstance:trace("game over")
		else
			--actualizar gui
			self.num_hearts = self.MAXHEARTS
			--cargar ultimo chekpoint
			if self.last_checkpoint ~= nil then
				self.last_checkpoint.load_checkpoint(self)
				self.coreInstance:trace("checkpoint loaded")
			else 
				
				self.coreInstance:trace("reset level")
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

