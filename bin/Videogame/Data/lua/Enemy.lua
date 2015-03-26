Enemy = {}

function Enemy.new(_position, user_data_name)
    local self = CAIController()    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.position = _position
	self.gravity = 13.0
	self.speed = 0.5
	self.turn_speed = 2.0
	self.jump_force =1.5
    self.current_jump_force = 0
	self.is_jumping = false
	self.current_time= 0
	self.wp = Vect3f(-3.0,2.0,15.0)
	
	self.coreInstance = CCoreLuaWrapper().m_CoreInstance
	self.physics_manager = self.coreInstance:get_phisics_manager()
	self.phisic_controller =self.physics_manager:create_spherical_phisic_controller(0.5, 0.25, 0.87, 0.1, 0.3, 0, "Enemy1", Vect3f(-15, 5, -15), -self.gravity, Vect3f(1.0,0.0,0.0))
	self.phisic_controller:set_position(self.position)
	self.malla = self.coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("Enemy4")
	if self.malla ~= nil then
		self.malla:set_yaw(self:get_yaw())
		self.malla:set_position(self.phisic_controller:get_position())
	end
	
	-----,-	 ENEMY FUNCTIONS -----
	--[[function self.move_to(dt, x,y,z)
	local lalala = Vect3f(x, y, z)
		if lalala:distance(self.position) >= 2 then
			local direction = lalala - self.position
			self.coreInstance:trace("YAW!!!!!!!")
			self.coreInstance:trace(tostring(self:get_yaw()))
			local diff = Vect3f(1,0,0).rotate_y(self:get_yaw())
			local angle = self.get_angle_diff(direction, diff)
			self:set_yaw(self:get_yaw()-angle * self.turn_speed * dt)
			self.phisic_controller:move(Vect3f(1,0,0).rotate_y(self:get_yaw())*self.speed*dt)
			self:set_position(self.phisic_controller:get_position())
			self.malla:set_yaw(self:get_yaw())
			self.malla:set_position(self.phisic_controller:get_position())
			
		end
	end
	
	function self.get_angle_diff(A, B)
		local angle = A*B
		angle = math.acos(math.rad(angle))
		local dir = (A^B).y
		if dir >=0 then
			dir = 1
		else
			dir = -1
		end
		return (angle*dir);
	end
	--]]
	function self.die()
		--TODO

	end
	
	return self
end

local enemy
local instance = CLuaGlobalsWrapper().m_CoreInstance;
function set_enemy(_enemy)
	enemy = _enemy
end

function enemy_enter_stopped()
	
	return 0
end

function enemy_exit_stopped()
	enemy.current_time = 0
end

function enemy_update_stopped(ElapsedTime, doComprobation)
	if enemy.current_time >= 0.1 then
		instance.m_string = "Andar_WP"
	end
	enemy.current_time = enemy.current_time +1 * ElapsedTime
end


function enemy_enter_moving()

end

function enemy_exit_moving()
	instance.m_string = "Buscar_next_WP"
	enemy.current_time = 0
end

function enemy_update_moving(ElapsedTime, doComprobation)
	enemy.coreInstance:trace("Moving Enemy!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	enemy.coreInstance:trace(tostring(enemy.wp.z))
	enemy:move_to(ElapsedTime, enemy.wp)
	
	--local player_position = core:get_player_controller():get_position()
	--core:trace(tostring(player_position.x));
	
	local wp_distance = get_distance_between_points(enemy:get_position(), enemy.wp)
	if wp_distance < 16 then
		enemy.coreInstance:trace("A buscar WP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
		instance.m_string = "Buscar_next_WP"
	end
		
end

function enemy_enter_calcwp()
	enemy.coreInstance:trace("Calculando WP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	if enemy.wp.z == -15 then
		enemy.wp = Vect3f(-3.0,2.0,15.0)
	else
		enemy.wp = Vect3f(-3.0,2.0,-15.0)
	end
	instance.m_string = "Andar_WP"

end

function enemy_exit_calcwp()
--core:trace("Saliendo Buscar_next_WP");
-- OnExit Buscar_next_WP
	enemy.current_time = 0

end

function enemy_update_calcwp(ElapsedTime, doComprobation)
	enemy.coreInstance:trace("Calculando WP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	--[[if enemy.wp.z == 15 then
		enemy.wp = Vect3f(2.0,2.0,-15.0)
	else
		enemy.wp = Vect3f(2.0,2.0,15.0)
	end--]]
	instance.m_string = "Andar_WP"
end


