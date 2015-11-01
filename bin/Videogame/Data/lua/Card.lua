Card = {}
Card.__index = Card
local coreInstance = CCoreLuaWrapper().m_CoreInstance;
Cards_Vector = {}

function Card.new(name, path)
	if collectible_manager == nil then
		collectible_manager = coreInstance:getCollectibleManager()
	end
	local self = setmetatable({}, Card)
	self.name = name
	self.path = path
	self.unlocked = false
	table.insert(Cards_Vector, self)
	return self
end

function Card:set_unlocked(unlocked)
	self.unlocked = unlocked
end

function Card:get_path ()
	return self.path
end

function Card:get_name ()
	return self.name
end


--[[ Class Tests
stick = Card.new("card1", "Data/imgs/img1.png")
stick:get_path()
stick:set_unlocked(true)

stick2 = Card.new("card2", "Data/imgs/img2.png")
stick2:get_path()
stick2:set_unlocked(true)

stick3 = Card.new("card3", "Data/imgs/img2.png")
stick3:get_path()
stick3:set_unlocked(false)

coreInstance:trace("NumCards: "..tostring(table.maxn(Cards_Vector)))

table.remove(Cards_Vector,1)

Cards_Vector[1]:get_name()
--]]