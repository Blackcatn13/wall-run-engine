 coreInstance = CCoreLuaWrapper().m_CoreInstance
 instance = CLuaGlobalsWrapper().m_CoreInstance
 physx_manager = coreInstance:get_phisics_manager()
 player = Player:get_instance()
 player_controller = coreInstance:get_player_controller()
 renderable_objects_layer_manager = coreInstance:get_renderable_object_layer_manager()
 trigger_manager = coreInstance:get_trigger_manager()
 gui_manager = coreInstance:getGUIManager()
 luaUtil = CCMathLuaUtils()
 act2in = coreInstance:get_action_to_input()
 cam_Controller = coreInstance.m_CameraController
 lightM = coreInstance:get_light_manager()
 enemy_manager =  coreInstance:get_enemy_manager()
 

