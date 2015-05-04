local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local textureManager = coreInstance:get_texture_manager();
local texture = textureManager:get_texture_resource("./Data/g_Textures/celshade.tga", false);