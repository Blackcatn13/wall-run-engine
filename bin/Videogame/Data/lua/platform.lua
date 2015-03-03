
Platform = {}
Platform_mt = { __index = Platform}
coreInstance = CCoreLuaWrapper().m_CoreInstance
phisics_manager = coreInstance:get_phisics_manager()

function Platform:create()
    local new_inst = {}    -- the new instance
    setmetatable( new_inst, Platform_mt ) -- all instances share the same metatable
    return new_inst
end

function Platform:className()
    local name = "Testing-Chanchullo_Lua"
	local n1 = CNamed();
	n1:set_name(name);
end

