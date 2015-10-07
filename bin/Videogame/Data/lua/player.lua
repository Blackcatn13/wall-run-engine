Player = {}

local num_hearts = 0
local num_lifes = 0
local instance = 0
--local last_checkpoint = nil
local last_spikes_position = nil
--local visited_checkpoints = {}


function Player.new()
    local self = {}    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.MAXHEARTS = 3
	self.MAXLIFES = 3
	self.num_hearts = self.MAXHEARTS
	self.num_lifes = self.MAXLIFES
	self.instance = self
	self.pixelites = 0
	self.last_spikes_position = Vect3f(0.0, 0.0, 0.0)
	self.spikes_yaw = 0
	self.hurt_by_spikes = false
	self.coreInstance = CCoreLuaWrapper().m_CoreInstance;
	self.visited_checkpoints = {}
	self.score = 0
	self.stickers = 0
	self.is_hit = false
	self.is_hit_reset_first = false
	self.vector_damage = Vect3f(0.0,0.0,0.0)
	self.iman_pos = Vect3f(0.0,0.0,0.0)
	self.use_iman = false
	self.enemies_killed = 0
	self.enemy_puzzle_active = false
	self.going_back = false
	self.playing_hit = false
	self.zone = 1
	self.original_position = Vect3f(55.784378, 1.0, 0.617737)
	self.original_yaw = 0.0
	self.attack_enabled = false
	self.game_over = false
	self.crush_down = false
	self.crush_up = false
	self.crush_left = false
	self.crush_right = false
	self.can_finish_atack = true
	self.on_air = false
	self.is_activating_poly = false
	self.activating_triggers = true
	self.is_dead = false
	self.has_ass_burned = false
	self.can_move = true
	self.last_checkpoint = nil
	self.inside_obstacle_area = false
	self.vanishing = false
	self.dead_in_hole = false
	self.super_piky_active = false
	self.super_piky_timer = 0.0
	self.super_piky_attack = false
	self.count_pixelites = 0.0
	
	------	 PLAYER FUNCTIONS -----
	
	function self.set_super_piky(super_piky)
		if player_controller.m_Room == 0 then
			local playerRenderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name)	
			local position = playerRenderable:get_position()
			local yaw = playerRenderable:get_yaw()

			playerRenderable:set_visible(false)
		
			if super_piky then
				piky_mesh_name = "SuperPiky"
				--Actualizar con las animaciones de SuperPiky
				anim_idle =	0
				anim_run = 1
				anim_jump_start = 192
				anim_jump_loop = 3
				anim_jump_end = 4
				anim_attack = 2--5
				anim_hurt = 6
				anim_poly = 7
				anim_death = 8
				anim_death_retry = 9
				anim_DJump_01 = 10
				anim_DJump_02 = 11
				anim_DJump_03 = 12
				anim_DJump_04 = 13
				anim_Burn = 14
				anim_BurnJump = 15
				
			else
				piky_mesh_name = "Piky"
				--Actualizar con las animaciones de Piky
				anim_idle =	0
				anim_run = 1
				anim_jump_start = 2
				anim_jump_loop = 3
				anim_jump_end = 4
				anim_attack = 5
				anim_hurt = 6
				anim_poly = 7
				anim_death = 8
				anim_death_retry = 9
				anim_DJump_01 = 10
				anim_DJump_02 = 11
				anim_DJump_03 = 12
				anim_DJump_04 = 13
				anim_Burn = 14
				anim_BurnJump = 15
			end
			
			playerRenderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room("player", 0):get_resource(piky_mesh_name)	
			if playerRenderable ~= nil then
				player_controller.m_RenderableObject = playerRenderable
				playerRenderable:set_position(position)
				playerRenderable:set_yaw(yaw)
				playerRenderable:set_visible(true)
			end	
			self.super_piky_active = super_piky
		end
	end
		
	function self.set_room(room_number)
		player_controller.m_Room = room_number
	end
	function self.add_pixelites(inc_pixelites)
		self.pixelites = self.pixelites + inc_pixelites
		self.count_pixelites = self.count_pixelites + inc_pixelites
		if self.count_pixelites >= 50 then
			self.extra_life()
			self.count_pixelites = self.count_pixelites -50
		end
		--self.coreInstance:trace("Pixelite value: "..tostring(inc_pixelites))
		--self.coreInstance:trace("Num Pixelites: "..tostring(self.pixelites))
	end
	
	function self.extra_life()
		self.num_lifes = self.num_lifes +1
		gui_manager:set_num_heart( self.num_lifes )
		gui_manager:set_is_displayed_heart(true)
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
	
	function self.set_initial_position(yaw, position)
		self.original_position = position
		self.original_yaw = yaw
	end
	
	function self.reset_position()
		player_controller.m_PhysicController:set_position(self.original_position)
		player_controller:set_yaw(self.original_yaw)
		if player_controller.m_Room ~= 1 then
			set_player_room("1", true)
		end
	end
	
	function self.get_player_controller()
		local player_controller = self.coreInstance:get_player_controller().m_PhysicController;
		return player_controller 
	end

	function self.get_controller()
		return self.coreInstance:get_player_controller()
	end
	
	function self.player_take_damage(direction)
		if self.is_hit == false and not self.is_dead then
			if (direction.x ~= 0 or direction.y ~= 0 or direction.z ~= 0) and not self.hurt_by_spikes then
				self.vector_damage = direction
			end
			self.num_hearts = self.num_hearts -1
			self.coreInstance:trace("Taking damage. Ouch! I lost a heart.")     
			self.is_hit = true
			self.is_hit_reset_first = true
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
				if self.hurt_by_spikes then
					self.get_player_controller():set_position(self.last_spikes_position)
				end
				self.player_die()
				
			elseif self.hurt_by_spikes then
				self.get_player_controller():set_position(self.last_spikes_position)
			end
		end
	end

	function self.player_die()
		if not self.is_dead then --Si ya está muerto no se va a morir más, QUE NO ES UN ZOMBIE
			
			self.activating_triggers = false
			--self.remove_animations()
			self.is_dead = true
			local renderable_piky_mesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name)
			self.remove_animations(renderable_piky_mesh)
			if not self.has_ass_burned then
				if not self.dead_in_hole then -- Si cae por una agujero no tiene animacion de caida
					if self.num_lifes == 1 then
						renderable_piky_mesh:execute_action(anim_death,0,0.3,1,true) --animacion de muerto GameOver
					else
						renderable_piky_mesh:execute_action(anim_death_retry,0,0.3,1,false) --animacion de muerto
					end								
				end
			else
				renderable_piky_mesh:execute_action(anim_BurnJump,0,0.3,1,true) -- animacion de piky tostada
				local emitter2 = particle_manager:get_resource(renderable_piky_mesh.m_ParticleEmitter2)
				if emitter2:get_visible() == false then
					emitter2:set_visible(true)
					emitter2.m_vPos = renderable_piky_mesh:get_position() + renderable_piky_mesh.m_EmitterOffset2
				end
			end
			local coreInstance = CCoreLuaWrapper().m_CoreInstance;
			self.coreInstance:trace("player dies")
			self.num_lifes = self.num_lifes - 1
			--[[if self.num_lifes > 0 then
				if not self.has_ass_burned and not self.dead_in_hole then
					fade(6)
				elseif self.has_ass_burned and not self.dead_in_hole then
					fade(4)
				else
					fade(3)
				end
			else
				fade(4)
			end]]
			--gui_manager:set_is_displayed_heart(true);
			gui_manager:set_count_heart(0.0);
			gui_manager:set_num_heart( self.num_lifes );	
			local emitter3 = particle_manager:get_resource(renderable_piky_mesh.m_ParticleEmitter3)
			emitter3:set_visible(false)
			
		end
		
		--self.check_death_actions()
	end
	
	function self.check_visited_checkpoints(trigger_name)
		-- recorrer array de checkpoints visitados del player
		-- si coincide con este checkpoint retornar true
	--	self.coreInstance:trace("Searching CheckPoint")
		for i = 1, table.getn(self.visited_checkpoints) do
			self.coreInstance:trace(".")
			if self.visited_checkpoints[i].name == trigger_name then
				--self.coreInstance:trace("Encontrado!!")
				return true
			end	
		end
		--self.coreInstance:trace("No Encontrado")
		return false
	end
	
	function self.check_death_actions()
		local renderable_piky_mesh = renderable_objects_layer_manager:get_renderable_objects_manager_by_str_and_room("player", player_controller.m_Room):get_resource(piky_mesh_name)
		local emitter2 = particle_manager:get_resource(renderable_piky_mesh.m_ParticleEmitter2)
		if emitter2:get_visible() == true then
			emitter2:set_visible(false)
		end
		--Chuky Desaparece
		ChuckyDesapears()
		renderable_piky_mesh:remove_action(anim_death)
		renderable_piky_mesh:remove_action(anim_BurnJump)
		local boss = enemy_manager:get_enemy("ChuckyBoss")
	
		
		if self.num_lifes == 0 then
			--game over
			self.coreInstance:trace("game over")
			local game_over_pos = Vect3f(925, 0, -1.5)
			set_screen_menu(game_over_pos, 9)
			gui_manager:set_is_gameover(true);
			self.game_over = true
		else
			self.coreInstance:trace(tostring(self.num_lifes))
			gui_manager:set_image('LifeGUI','Life3')
			--actualizar gui
			self.num_hearts = self.MAXHEARTS
			--cargar ultimo chekpoint
			if self.last_checkpoint ~= nil then
				self.last_checkpoint.load_checkpoint(self)
				self.coreInstance:trace("checkpoint loaded")
			--else 
				
			--	self.coreInstance:trace("reset level")
				--reset level platforms + enemies
			end
			
		end
		--coreInstance:trace("Player dying zone: ".. tostring(self.zone))
		if tostring(self.zone) == "4.0" or tostring(self.zone) == "7.0" then
			--coreInstance:trace("Player died in zone 4.0")
			--enable_breaking_platform("Plataforma_Fragil")
			 restore_broken_platforms_by_layer("breakable")
		--[[elseif tostring(self.zone) == "3.0" then
			reset_wall_trap(0, "WALL_TRAP1_RIGHT")
			reset_wall_trap(0, "WALL_TRAP1_LEFT")]]
		end
		
		self.is_dead = false
		self.has_ass_burned = false
		self.set_super_piky(false)
		--self.activating_triggers = true
		if boss.m_BossRunning and self.num_lifes > 0 then
			start_boss()
		end
	end
	
	function self.remove_animations(_renderable)
		if _renderable:is_action_animation_active() then
			_renderable:remove_action(anim_jump_start)
			_renderable:remove_action(anim_attack)
			_renderable:remove_action(anim_hurt)
			_renderable:remove_action(anim_poly)
			_renderable:remove_action(anim_death)
			_renderable:remove_action(anim_BurnJump)
		end
		if _renderable:is_cycle_animation_active() then
			_renderable:clear_cycle(anim_idle,0.1) 
			_renderable:clear_cycle(anim_run,0.1)
			_renderable:clear_cycle(anim_jump_loop,0.1)
			_renderable:clear_cycle(anim_jump_end,0.1)
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



