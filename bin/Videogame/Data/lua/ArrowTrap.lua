ArrowTrap = {}
ArrowTrap.__index = ArrowTrap
trap_vector = {}

function ArrowTrap.new(name, ro_list, direction, speed, time_out, axis)
	local self = setmetatable({}, ArrowTrap)
	self.name = name
	self.ro_list = ro_list	
	self.direction = direction
	self.speed = speed
	self.time_out = time_out
	self.original_position_x = self.ro_list[1]:get_position().x
	self.original_position_z = self.ro_list[1]:get_position().z
	self.damage_axis = axis
	
	table.insert(trap_vector, self)
	return self
end

function ArrowTrap:move()
	for i = 1, table.maxn(self.ro_list) do
		self.ro_list[i]:set_position( self.ro_list[i]:get_position() + self.direction * self.speed * coreInstance.m_ElapsedTime)
	end

end

function ArrowTrap:reset_position()
	for i = 1, table.maxn(self.ro_list) do
		self.ro_list[i]:set_position(Vect3f(self.original_position_x, self.ro_list[i]:get_position().y, self.original_position_z) )
		--coreInstance:trace("Reset Arrow Position ".. i .." Position =" .. tonumber(self.original_position_x) ..", " ..tonumber(self.ro_list[i]:get_position().y)..", " ..tonumber(self.original_position_z))
	end
end
