 --Managers
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
cinematic_controller = coreInstance:get_cinematic_controller()
sound_manager = CCoreLuaWrapper().m_CoreInstance:getWWSoundManager()
puzzle_manager = coreInstance:get_puzzle_manager()

--Fin Managers
CCoreLuaWrapper().m_CoreInstance:getTickCounter():addTick("lava", 0.05, 1000);
boss_mesh_name = "ChuckyBoss"
boss_miks_enabled = false
boss_projectile_returned = false
boss_projectile_returned_by_chucky = false

--Collectible variables
CollectibleVisibleName = ""

--Variables de Control
first_time_main_menu = true
chuky_last_room = 0;
transition_effect = TransitionEffect()
gui_manager.m_sTransitionEffect = transition_effect
fade_done = false
first_load = true
jump_enabled = true
all_boss_miks_killed = true
boss_miks_killed = 0
piky_mesh_name = "Piky" --Piky o SuperPiky
powerup_name = "disparoSuperPiky"
current_shot_type = "rock"  -- rock /powerup
inicial_speed_change = 5
current_speed_change = 5
max_speed_change = 10
actual_speed_change = 0
transition_super_piky = false
chucky_reapeared = false
fade_step = 0 --0: Not fade, 1: first fade, 2: black_screen, 3: second fade

--Animaciones del Player
--Piky
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