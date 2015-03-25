Enemy = {}

function Enemy.new(_position, user_data_name)
    local self = CObject3D()    -- the new instance
   -- setmetatable( new_inst, Player_mt ) -- all instances share the same metatable
	self.position = _position
	self.gravity = 13.0
	self.speed = 0.5
	self.turn_speed = 2.0
	self.jump_force =1.5
    self.current_jump_force = 0
	self.is_jumping = false
	
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
	function self.move_to(dt, point)
		if point:distance(self.position) >= 2 then
			local direction = point - self.position
			local diff = Vect3f(1,0,0).rotate_y(self:get_yaw())
			local angle = get_angle_diff(direction, diff)
			self:set_yaw(self:get_yaw()-angle * self.turn_speed * dt)
			self.phisic_controller:move(Vect3f(1,0,0).rotate_y(self:get_yaw())*self.speed*dt)
			self:set_position(self.phisic_controller:get_position())
			self.malla:set_yaw(self:get_yaw())
			self.malla:set_position(self.phisic_controller:get_position())
			
		end
	end
	
	function get_angle_diff(A, B)
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
	
	function self.die()
		--TODO

	end
	
	
end
