 coreInstance = CCoreLuaWrapper().m_CoreInstance
 instance = CLuaGlobalsWrapper().m_CoreInstance
 physx_manager = coreInstance:get_phisics_manager()
 graphics_manager = coreInstance:get_graphics_manager()
 player = Player:get_instance()
 player_controller = coreInstance:get_player_controller()
 renderable_objects_layer_manager = coreInstance:get_renderable_object_layer_manager()
 trigger_manager = coreInstance:get_trigger_manager()
 gui_manager = coreInstance:getGUIManager()
 luaUtil = CCMathLuaUtils()
 act2in = coreInstance:get_action_to_input()
 inputm = coreInstance:get_input_manager()
 cam_Controller = coreInstance.m_CameraController
 lightM = coreInstance:get_light_manager()
 enemy_manager = coreInstance:get_enemy_manager()
 --collectible_manager = coreInstance:getCollectibleManager()
 collectible_manager = nil
 particle_manager = coreInstance:getParticleManager()
 billboard_manager = coreInstance:get_billboard_manager()
chuky_last_room = 0;
cinematic_controller = coreInstance:get_cinematic_controller()
sound_manager = CCoreLuaWrapper().m_CoreInstance:getWWSoundManager()
CCoreLuaWrapper().m_CoreInstance:getTickCounter():addTick("lava", 0.05, 1000);
puzzle_manager = coreInstance:get_puzzle_manager()