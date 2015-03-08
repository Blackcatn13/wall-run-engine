local coreInstance = CCoreLuaWrapper().m_CoreInstance
local physx_manager = coreInstance:get_phisics_manager()
local rol_manager = coreInstance:get_renderable_object_layer_manager()

function init_platform(name, user_data_name, size, position)
	local platform = rol_manager:get_default_renderable_object_manager():get_resource(name)
	platform:insert_platform(user_data_name, size, position)
	coreInstance:trace(name)
	return platform
end

--function init_breakable_platform(name, user_data_name, size, position, trigger_name)
--	local platform = rol_manager:get_default_renderable_object_manager():get_resource(name)
--	platform:insert_platform_into_map(name, breakable_platform)
--	return breakable_platform
--end

function update_break_platform(current_time, max_time, platform_name)

	if current_time >= max_time then
		local platform = rol_manager:get_default_renderable_object_manager():get_resource(platform_name)
		platform:disable_platform()
	end
end