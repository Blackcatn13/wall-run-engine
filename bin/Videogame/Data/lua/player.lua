Player = {}

local num_hearts = 0
local num_lives = 0
local instance = 0
local last_checkpoint = nil
local last_spikes_position = nil
--local visited_checkpoints = {}


function Player.new()
    local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.MAXHEARTS = 3
	self.MAXLIVES = 3
	self.num_hearts = self.MAXHEARTS
	self.num_lives = self.MAXLIVES
	self.instance = self
	self.pixelites = 0
	self.last_spikes_position = Vect3f(0.0, 0.0, 0.0)
	self.coreInstance = CCoreLuaWrapper().m_CoreInstance;
	self.visited_checkpoints = {}
	self.score = 0
	self.stickers = 0
	self.is_hit = false
	
	------	 PLAYER FUNCTIONS -----
	function self.add_pixelites(inc_pixelites)
		self.pixelites = self.pixelites + inc_pixelites
		self.coreInstance:trace("Num Pixelites: "..tostring(self.pixelites))
	end
	
	function self.add_sticker(img_name)
		self.stickers = self.stickers + 1
		unlock_image(img_name)
		self.coreInstance:trace("Num Stickers: "..tostring(self.stickers))
	end
	
	function self.inc_score(value)
		self.score = self.score + value
		self.coreInstance:trace("Score: "..tostring(self.score))
	end
	
	function self.get_player_controller()
		local player_controller = self.coreInstance:get_player_controller().m_PhysicController;
		return player_controller 
	end

	function self.get_controller()
		return self.coreInstance:get_player_controller()
	end
	
	function self.player_take_damage()
		if self.is_hit == false then
			self.num_hearts = self.num_hearts -1
			self.coreInstance:trace("Taking damage. Ouch! I lost a heart.")     
			self.is_hit = true
			if gui_manager:get_active_image('LifeGUI') == 'Life3' then
				gui_manager:set_image('LifeGUI','Life2')
			else
				if gui_manager:get_active_image('LifeGUI') == 'Life2' then
					gui_manager:set_image('LifeGUI','Life1')
				else
					if gui_manager:get_active_image('LifeGUI') == 'Life1' then
						gui_manager:set_image('LifeGUI','Life0')
					end
				end	
			end
			if self.num_hearts == 0 then
				--self.num_hearts = 3
				self.player_die()
				--self.coreInstance:trace("PLAYER DEEEEEEEEEEEEEEEEEEEEEEEEEAD")
			end
		end
	end

	function self.player_die()
		local coreInstance = CCoreLuaWrapper().m_CoreInstance;
		self.coreInstance:trace("player dies")
		self.num_lives = self.num_lives -1
		gui_manager:set_image('LifeGUI','Life3')
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
	
	function self.check_visited_checkpoints(trigger_name)
		-- recorrer array de checkpoints visitados del player
		-- si coincide con este checkpoint retornar true
		self.coreInstance:trace("Searching CheckPoint")
		for i = 1, table.getn(self.visited_checkpoints) do
			self.coreInstance:trace(".")
			if self.visited_checkpoints[i].name == trigger_name then
				self.coreInstance:trace("Encontrado!!")
				return true
			end	
		end
		self.coreInstance:trace("No Encontrado")
		return false
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

