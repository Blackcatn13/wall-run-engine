local is_init=true;
local topPosition = -1000;
local inLoop = false;
local inDoubleLoop = false;
local inDoubleJump = false;
local executedDoubleStart = false;
local executedDoubleLoop = false;
local executedDoubleEnd = false;
local _land = false;
local _fallingAnimation = false;
local _fallPosition = Vect3f(-10000, -10000, -10000);
local timer = 0.0
local player_moving = false
local move_3D = true
local lastCamYaw = -10;
local extraRotationContinue = 0;
local lastJoystickYaw = -10;
local _isQuiet = false;
local _gravityForce = -50;
local _actualGravityForce = 0;
local hole_timer = 0.0
local max_hole_falling = 2.0;
local iman_force = 20;

--////////////////////////////////////////////////////////
-- GLOBAL PARAMETERS
--////////////////////////////////////////////////////////
local AttackGravityStart = 0.30;
local AtackGravityModifier = 0.0002;
local m_AttackGravity = AttackGravityStart;
local AttackStartSpeed = 30;
local AttackSpeed = AttackStartSpeed;
local AttackDismin = 0.5;
local canAttack = true;
local contador = 0;
local timeAttack = 0.6;
local jumpTime = 0;
local m_damageFeedBackSpeedStart = 20;
local m_damageFeedBackSpeed = 20;
local m_damageFeedBackSpeedDismin = 40;

--////////////////////////////
--SUPER PIKY
--////////////////////////////
local super_piky_time = 30.0


--////////////////////////////////////////////////////////
-- PARAMETERS
--////////////////////////////////////////////////////////
player_controller.m_Gravity = 9;						--Gravedad que afecta al personaje cuando cae.
player_controller.m_GravityJump = 11;					--Gravedad que afecta cuando el personaje está impulsándose hacia arriba en el salto.
player_controller.m_Speed = 13;							--Velocidad de movimiento.
player_controller.m_JumpSpeed = 10;						--Velocidad de movimiento en el salto.
player_controller.m_JumpForce = 5.5;					--Fuerza de salto, impulso.
superjumForce = 10;						 				--SUPERSALTO CHEAT
player_controller.m_AttackForce = 0.7;					--Impulse force for the attack.
player_controller.m_PhysicController:set_step(0.5); 	--Altura que puede superar (escalones).
local AirTime = 0.7;									-- Time into the air, playing air loop
local m_damageTime = 0.3;
local acceleration = 0;
local accelerationTime = 1;
local accelerationTimeJump = 0.8;
local deccelerationTime = 0.5;
local deccelerationTimeJump = 0.8;
local inertia = Vect3f(0,0,0);
local inertiaJump = Vect3f(0,0,0);
local percentAirMovement = 0.75;

local m_ReduceCollider = 0.75;
--////////////////////////////////////////////////////////
function update_pause()
	if act2in:do_action_from_lua("PauseGame") and gui_manager.m_sCurrentWindows == "Play.xml" and not cinematic_controller.m_executing and not gui_manager:get_is_gameover() and not player.is_dead then 
		if not gui_manager:get_is_paused() then
			gui_manager:activate_pause(true);
			gui_manager:set_is_paused(true);
		else
			gui_manager:activate_pause(false);
			gui_manager:set_is_paused(false);
		end
	end
end

function on_update_player_lua(l_ElapsedTime)

	local active_camera = cam_Controller:get_active_camera();
	local currentWP = nil
	local nextWP = nil
	if active_camera.m_currentWaypoint ~= nil then
		currentWP = active_camera:get_path_point(active_camera.m_currentWaypoint)
		nextWP = active_camera:get_path_point(active_camera.m_nextWaypoint)
	end
	local camObject = active_camera.m_pObject3D;
	local directionalLight = lightM:get_resource("ShadowLight");
	local layer = "player"
	if player.vanishing then
		layer = "vanishing"
	end

	
	local playerRenderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(layer, player_controller.m_Room):get_resource(piky_mesh_name);

	
	if gui_manager:get_is_displayed_console() == false and gui_manager.m_sCurrentWindows == "Play.xml" and player.can_move and not cinematic_controller.m_executing then
	

		if is_init == true then
			is_init = false
			player_controller.m_is3D = true
		end
		
		if act2in:do_action_from_lua("StartBoss") then
			start_boss()
		end
		
		if act2in:do_action_from_lua("SuperPiky") then
			start_super_piky(playerRenderable)	
		end
	
		if transition_super_piky and not playerRenderable:is_action_animation_active() then
			transition_super_piky = false
			player.set_super_piky(true)
			local emitter4 = particle_manager:get_resource(playerRenderable.m_ParticleEmitter4)
			--emitter4.m_vPos = playerRenderable:get_position()
			emitter4:set_visible(false)
		end
	
		if player.super_piky_active then 
			if player.super_piky_timer < super_piky_time then
				player.super_piky_timer = player.super_piky_timer+ l_ElapsedTime
				--coreInstance:trace("Super Piky time: "..tostring(player.super_piky_timer))
			
			else
				player.set_super_piky(false)
				player.super_piky_timer = 0.0
				--coreInstance:trace("Bye Super Piky")
			end
		end
	
		if act2in:do_action_from_lua("ChangeDimension") then
			if player_controller.m_is3D == true then 
				player_controller.m_is3D = false;
				local cam = coreInstance.m_CameraController:get_resource("3DCam");
				cam.m_eTypeCamera = 5;
				coreInstance.m_CameraController:set_active_camera("3DCam");
			else
				player_controller.m_is3D = true;
				local cam = coreInstance.m_CameraController:get_resource("3DCam");
				cam.m_eTypeCamera = 6;
				coreInstance.m_CameraController:set_active_camera("3DCam");
			end
		end
		-- MOVER LA LUZ DEL SHADOWMAP
		local lightDirection = directionalLight:get_direction();
		local altura = directionalLight:get_valoryfijo();
		local offsetx = lightDirection.x*altura;
		local offsetz = lightDirection.z*altura;
		directionalLight:set_position(Vect3f(player_controller:get_position().x-offsetx,player_controller:get_position().y+altura,player_controller:get_position().z-offsetz));
		local dir3D = active_camera:get_direction();
		local dirYaw = camObject:get_yaw();
		if (lastCamYaw == -10) then
			lastCamYaw = dirYaw;
		end
		local dirNor = Vect3f(math.cos(dirYaw + (math.pi/2)), 0, (math.sin(dirYaw + (math.pi/2))));
		dir3D.y = 0;
		dir3D = luaUtil:normalize(dir3D);
		dirNor.y = 0;
		dirNor = luaUtil:normalize(dirNor);
		local mov = Vect3f(0,0,0);
		local PlayerYaw =  - dirYaw + 1.57;
		
		player.pressed_return = false
		
		-- Si dañan al player
		if player.is_hit == true and not player.hurt_by_spikes and not transition_super_piky then
			if (player.is_hit_reset_first == true) then
				player_controller.m_isJumping = false;
				player_controller.m_isDoubleJumping = false;
				_land = false;
				player_controller.m_ableToIman = false;
				playerRenderable:clear_cycle(anim_idle,0);
				playerRenderable:clear_cycle(anim_run,0);
				playerRenderable:clear_cycle(anim_jump_loop,0);
				playerRenderable:clear_cycle(anim_jump_end,0);
				playerRenderable:clear_cycle(anim_DJump_03,0);
				playerRenderable:clear_cycle(anim_DJump_04,0);
				playerRenderable:remove_action(anim_jump_start);
				playerRenderable:remove_action(anim_DJump_01);
				playerRenderable:remove_action(anim_DJump_02);
			end
			if timer > m_damageTime then
				m_damageFeedBackSpeed = m_damageFeedBackSpeed - m_damageFeedBackSpeedDismin * l_ElapsedTime
			end
			if m_damageFeedBackSpeed < 0.2 then
				m_damageFeedBackSpeed = 0.2
			end
			timer = timer + l_ElapsedTime
			mov = player.vector_damage * m_damageFeedBackSpeed * l_ElapsedTime			
		end
		
		
		if act2in:do_action_from_lua("StopMovePlayerBack") or act2in:do_action_from_lua("StopMovePlayerForward")  then
				move_3D = true 
		end
		
		
		local auxyaw = 0;
		--///////////////////////////////////////////////////////////
		-- Movimiento del Player en las distintas direcciones. 
		--///////////////////////////////////////////////////////////
		if (player_controller.m_isJumpingMoving == false) and (player_controller.m_isAttack == false) and (player.is_hit == false and not player.is_dead) and player.super_piky_attack == false then
			local y_axis = 0.0
			local x_axis = 0.0
			if not playerRenderable.m_VanishActive and not player.hurt_by_spikes and not gui_manager:is_transition_effect_active() and not transition_super_piky then
				if inputm:has_game_pad(1) then
					auxAxisYMoved = act2in:do_action_from_lua("MoveForward", y_axis);
					auxAxisXMoved = act2in:do_action_from_lua("MoveRigth", x_axis);
					y_axis = inputm:get_game_pad_left_thumb_y_deflection(1);
					x_axis = inputm:get_game_pad_left_thumb_x_deflection(1);
					if (auxAxisXMoved or auxAxisYMoved) and player.activating_triggers == false then
						player.activating_triggers = true
					end
				else
					local moving_forward = 0;
					local moving_back = 0;
					local moving_right = 0;
					local moving_left = 0;
					auxForward = act2in:do_action_from_lua("MoveForward")
					auxBackward = act2in:do_action_from_lua("MoveBack")
					auxRight = act2in:do_action_from_lua("MoveRigth")
					auxLeft = act2in:do_action_from_lua("MoveLeft")
					
					if auxForward then
						moving_forward = 1;
					end
					if auxBackward then
						moving_back = 1;
					end
					if auxRight then
						moving_right = 1;
					end
					if auxLeft then
						moving_left = 1;
					end
					if (auxForward or auxBackward or auxRight or auxLeft ) and player.activating_triggers == false then
						player.activating_triggers = true
					end
					y_axis = moving_forward - moving_back;
					x_axis = moving_right - moving_left;
				end
			end
			local changedCamYaw = dirYaw - lastCamYaw;
			if y_axis == 0 and x_axis == 0 then
				cosyaw = 0;
				sinyaw = 0;
				extraRotationContinue = 0;
				if acceleration > 0 then
					local auxValue = 0;
					if player_controller.m_isJumping or player_controller.m_isDoubleJumping or _land then
						auxValue = 1 / deccelerationTimeJump;
					else
						auxValue = 1 / deccelerationTime;
					end
					acceleration = acceleration - (auxValue * l_ElapsedTime);
				else
					if acceleration < 0 then
						acceleration = 0;
					end
				end
				_isQuiet = true;
			else
				_isQuiet = false;
				if math.abs(changedCamYaw) > 1 and math.abs(changedCamYaw) < 4 then
					if extraRotationContinue ~= 0 then
						extraRotationContinue=0;
					else
						if changedCamYaw > 0 then
							extraRotationContinue = 1.57;--de 3d a 2d
						else
							extraRotationContinue = -1.57;--de 2d a 3d
						end
					end
				end
				if player_controller.m_is3D == false then
					y_axis = 0;
				end
				auxyaw = math.atan2(x_axis,y_axis);
				if (lastJoystickYaw == -10) then
					lastJoystickYaw = auxyaw;
				else
					if extraRotationContinue ~= 0 then
						local changedJoyYaw = auxyaw - lastJoystickYaw;
						if math.abs(changedJoyYaw) > 0.3 then
							extraRotationContinue = 0;
						elseif extraRotationContinue > 0 then
							extraRotationContinue = extraRotationContinue - math.abs(changedJoyYaw);
							if math.abs(extraRotationContinue) < 0.3 then
								extraRotationContinue = 0;
							end
						else
							extraRotationContinue = extraRotationContinue + math.abs(changedJoyYaw);
							if math.abs(extraRotationContinue) < 0.3 then
								extraRotationContinue = 0;
							end
						end
					end
				end
				local worldauxyaw = auxyaw + extraRotationContinue;
				worldauxyaw = worldauxyaw +1.57;
				cosyaw = math.cos(worldauxyaw); -- derecha 1 izquierda -1
				sinyaw = math.sin(worldauxyaw); -- alante 1 atras -1
				mov = dir3D * sinyaw + dirNor * cosyaw;
				if player_controller.m_is3D == false and extraRotationContinue == 0 and x_axis == 0 then
					mov = Vect3f(0,0,0);
				end
				if mov.x ~= 0 or mov.z ~= 0 then
					local player_yaw = math.atan2(mov.x,mov.z);
					mov = luaUtil:normalize(mov);
					player_controller:set_yaw(player_yaw);
				end
				player_controller.m_Direction3D = mov;
				mov = mov * l_ElapsedTime * player_controller.m_Speed;
				inertia = Vect3f(mov.x,0,mov.z);
				if acceleration < 1 then
					local auxValue = 0;
					if player_controller.m_isJumping or player_controller.m_isDoubleJumping or _land then
						auxValue = 1 / accelerationTimeJump;
					else
						auxValue = 1 / accelerationTime;
					end
					acceleration = acceleration + auxValue * l_ElapsedTime;
				else
					if acceleration > 1 then
						acceleration = 1;
					end
				end
				if player_controller.m_isJumping == true or player_controller.m_isDoubleJumping or _land then
					mov = mov * percentAirMovement;
				end
				if (y_axis > 0) then
					player_controller.m_isTurned = false;
					if player_controller.m_is3D == true then
						player.going_back = false;
					end
				elseif (y_axis < 0) then
					player_controller.m_isTurned = true;
					if player_controller.m_is3D == true then
						player.going_back = true;
					end
				end
			end
		end
		
		--ACTUALIZAR LASTCAMYAW Y LASTJOYSTICKYAW, SI SE QUIEREN USAR ABAJO HAY QUE ACTUALIZARLOS MAS TARDE
		lastCamYaw = dirYaw;
		lastJoystickYaw = auxyaw;
		
		

		-- --///////////////////////////////////////////////////////////
		-- Gravedad que siempre afecta al Player. 
		--///////////////////////////////////////////////////////////
		mov.y = -player_controller.m_Gravity * l_ElapsedTime * player_controller.m_isOnPlatform;
		
		--local dist_to_flooraux = get_distance_to_floor(player_controller:get_position());
		
		--///////////////////////////////////////////////////////////
		-- Cuando el Player está saltando, su velocidad dependerá del tipo de salto realizado. 
		--///////////////////////////////////////////////////////////
		local dist_to_floor = get_distance_to_floor(player_controller:get_position());
		if dist_to_floor > 3 and _land == false and player_controller.m_isJumping == false and player_controller.m_isDoubleJumping == false and player.is_hit == false then
			_land = true;
			player_controller.m_ableToIman = true;
			_fallingAnimation = false;		
			playerRenderable:clear_cycle(anim_idle,0.1);
			playerRenderable:clear_cycle(anim_run,0.1);
			playerRenderable:blend_cycle(anim_jump_loop,1,0.1);
		end
		if _land then
			if inDoubleJump == true then
				if executedDoubleLoop == false then
					if (not playerRenderable:is_action_animation_active()) then
						playerRenderable:remove_action(anim_DJump_02);
						playerRenderable:updateSkeleton(l_ElapsedTime);
						playerRenderable:blend_cycle(anim_DJump_03,1,0.1);
						executedDoubleLoop = true;
					end
				end
				if executedDoubleLoop == true and executedDoubleEnd == false then
					if dist_to_floor < 17 then
						playerRenderable:clear_cycle(anim_DJump_03,0.2);
						playerRenderable:updateSkeleton(l_ElapsedTime);
						playerRenderable:blend_cycle(anim_DJump_04,1,0.1);
						executedDoubleEnd = true;
					end
				end
				if executedDoubleEnd == true then
					if player_controller.m_isGrounded then
						_land = false;
						player_controller.m_ableToIman = false;
						inertiaJump = Vect3f(0,0,0);
						player.use_iman = false;
						player.iman_pos = Vect3f(0,0,0);
						player.on_air = false;
						inDoubleJump = false;
						playerRenderable:clear_cycle(anim_DJump_04,0.2);
					end
				end
			else
				if _fallingAnimation == false then
					if dist_to_floor < 17 then
							playerRenderable:clear_cycle(anim_jump_loop,0.2);
							playerRenderable:updateSkeleton(l_ElapsedTime);
							playerRenderable:blend_cycle(anim_jump_end,1,0.1);
							_fallingAnimation = true;
					end
				end
				if player_controller.m_isGrounded then
					_land = false;
					player_controller.m_ableToIman = false;
					inertiaJump = Vect3f(0,0,0);
					player.use_iman = false;
					player.iman_pos = Vect3f(0,0,0);
					player.on_air = false;
					if _fallingAnimation == false then
						playerRenderable:clear_cycle(anim_jump_loop,0.2);
					else
						playerRenderable:clear_cycle(anim_jump_end,0.2);
					end
				end
			end
		end
		if (player_controller.m_isJumping == true) or (player_controller.m_isFalling) then
			jumpTime = jumpTime + l_ElapsedTime;

			if player_controller.m_isJumping then
				mov.y = playerRenderable:getBoneMovement().y;
			end
			if inLoop then
				mov.y = 0;
			end
			if (not playerRenderable:is_cycle_animation_active()) and inLoop == false then
				player_controller.m_isFalling = true;
				jumpTime = 0;
			end
			if player_controller.m_JumpingTime > AirTime then
				
				inLoop = false;
				_land = true;
				player_controller.m_ableToIman = true;
				player_controller.m_isJumping = false;
				
			else
				if player_controller.m_isFalling then
					local positionOld = playerRenderable:get_position();
					local auxPosition = playerRenderable:getAnimationBonePosition().y; 
					local newPosition = Vect3f(positionOld.x, auxPosition, positionOld.z);
					playerRenderable:set_position(newPosition);
					playerRenderable:blend_cycle(anim_jump_loop,1,0.5);
					playerRenderable:updateSkeleton(l_ElapsedTime);
					playerRenderable:remove_action(anim_jump_start);
					playerRenderable:clear_cycle(anim_idle,0);
					playerRenderable:clear_cycle(anim_run,0);
					playerRenderable:updateSkeleton(l_ElapsedTime);
					player_controller.m_isFalling = false;
					inLoop = true;
				end
				player_controller.m_JumpingTime = player_controller.m_JumpingTime + l_ElapsedTime;
			end
		end
		if player_controller.m_executeDoubleJump == true and inDoubleJump == false then
			player_controller.m_executeDoubleJump = false;
			playerRenderable:clear_cycle(anim_idle,0);
			playerRenderable:clear_cycle(anim_run,0);
			playerRenderable:clear_cycle(anim_jump_loop,0);
			playerRenderable:clear_cycle(anim_jump_end,0);
			playerRenderable:remove_action(anim_jump_start);
			playerRenderable:execute_action(anim_DJump_01,0.1,0,1,true);
			player_controller.m_isDoubleJumping = true;
			player_controller.m_isJumping = false;
			_land = false;
			player_controller.m_ableToIman = false;
			player.use_iman = false;
			player.iman_pos = Vect3f(0,0,0);
			player.on_air = true;
			inDoubleJump = true;
			executedDoubleStart = false;
			executedDoubleLoop = false;
			executedDoubleEnd = false;
			_fallingAnimation = false;
		end
		if player_controller.m_isDoubleJumping then
			mov.y = playerRenderable:getBoneMovement().y;
			if (not playerRenderable:is_action_animation_active()) and executedDoubleStart == false then
				player_controller.m_isDoubleJumping = false;
				_land = true;
				player_controller.m_ableToIman = true;
				local positionOld = playerRenderable:get_position();
				local auxPosition = playerRenderable:getAnimationBonePosition().y; 
				local newPosition = Vect3f(positionOld.x, auxPosition, positionOld.z);
				playerRenderable:set_position(newPosition);
				playerRenderable:remove_action(anim_DJump_01);
				playerRenderable:execute_action(anim_DJump_02,0.1,0,1,true);
				playerRenderable:updateSkeleton(l_ElapsedTime);
				executedDoubleStart = true;
			end
		end
		if (player_controller.m_isJumping == true or player_controller.m_isDoubleJumping == true or _land == true) and _isQuiet == false then
			inertiaJump = Vect3f(mov.x,0,mov.z);
		end
		
		if player_controller.m_isAttack == false then
			contador = contador + l_ElapsedTime;
		end
		if contador > timeAttack then
			canAttack = true;
		end
		local emitter = particle_manager:get_resource(playerRenderable.m_ParticleEmitter)
		if player_controller.m_isAttack == true then
			if false then
				player_controller.m_isAttack = false;
			else
				if player.can_finish_atack then
					--local emitter = particle_manager:get_resource(playerRenderable.m_ParticleEmitter)
					if player_controller.m_CurrentAttackForce > 0.5 then					
						local prev_y = mov.y
						player_controller.m_CurrentAttackForce = player_controller.m_CurrentAttackForce - (m_AttackGravity * l_ElapsedTime);
						mov = player_controller.m_Direction3D * player_controller.m_CurrentAttackForce * AttackSpeed * l_ElapsedTime;
						if AttackSpeed > 0.05 then
							AttackSpeed = AttackSpeed - AttackDismin * l_ElapsedTime;
						end
						mov.y = prev_y;
						m_AttackGravity = m_AttackGravity + AtackGravityModifier * l_ElapsedTime;
						
						emitter.m_vPos = playerRenderable:get_position()+ playerRenderable.m_EmitterOffset
					else
						local mesh = playerRenderable;
						mesh:set_position(player_controller:get_position());
						player_controller.m_isAttack = false;
						player.get_player_controller():update_character_extents(true, m_ReduceCollider);
						emitter:set_visible(false)
					end
				else -- En el caso de estar entre triggers para impedir fin de ataque
					local prev_y = mov.y
					mov = player_controller.m_Direction3D * player_controller.m_CurrentAttackForce * AttackSpeed * l_ElapsedTime
					mov.y = prev_y
					emitter.m_vPos = playerRenderable:get_position()+ playerRenderable.m_EmitterOffset
				end
			end
		else
			if emitter:get_visible() == true and not player.super_piky_active then
				emitter.m_vPos = playerRenderable:get_position()+ playerRenderable.m_EmitterOffset
				emitter:set_visible(false)
			end
		end
		
		--///////////////////////////////////////////////////////////
		-- Acción de saltar del Player. Puede realizar 2 saltos distintos (de longitud, y salto vertical). 
		--///////////////////////////////////////////////////////////
		if (act2in:do_action_from_lua("Jump")) and (player_controller.m_isJumping == false and player_controller.m_isDoubleJumping == false and _land == false and player_controller.m_isAttack == false and player.is_hit == false and player_controller.m_isGrounded and not player.is_dead) and not playerRenderable.m_VanishActive and jump_enabled and not gui_manager:is_transition_effect_active() and not player.super_piky_active and not transition_super_piky then
			player_controller.m_JumpingTime = 0;
			playerRenderable:clear_cycle(anim_idle,0.2);
			playerRenderable:clear_cycle(anim_run,0.2);
			playerRenderable:updateSkeleton(l_ElapsedTime);
			playerRenderable:execute_action(anim_jump_start,0.1,0,1,true);
			playerRenderable:updateSkeleton(l_ElapsedTime);
			player_controller.m_isJumping = true;
			player.on_air = true;
			_fallingAnimation = false;
			inertiaJump = Vect3f(inertia.x, inertia.y, inertia.z);
			--player_controller.m_executeDoubleJump = true;
		end
		
		--///////////////////////////////////////////////////////////
		-- Acción de atacar del Player. Realiza un impulso hacia adelante. 
		--/////////////////////////////////////////////////////////// 
		if (act2in:do_action_from_lua("Attack") and not player_controller.m_isJumping and not player_controller.m_isDoubleJumping and not _land and player_controller.m_isAttack == false and canAttack == true and player.is_hit == false and player.attack_enabled and not player.is_dead) and not playerRenderable.m_VanishActive and not gui_manager:is_transition_effect_active() then --) and (player_controller.m_isAttack == false) then			
			if not player.super_piky_active then
				canAttack = false;
				player.get_player_controller():update_character_extents(false, m_ReduceCollider);
				contador = 0;
				local prev_y = mov.y
				m_AttackGravity = AttackGravityStart;
				AttackSpeed = AttackStartSpeed;
				player_controller.m_CurrentAttackForce = player_controller.m_AttackForce;
				player_controller.m_Direction3D =  get_attack_direction(player_controller.m_Direction3D, player_controller.m_PhysicController:get_position())
				mov = player_controller.m_Direction3D * player_controller.m_CurrentAttackForce * AttackSpeed * l_ElapsedTime;
				mov.y = prev_y;
				player_controller.m_isAttack = true;
				playerRenderable:execute_action(anim_attack,0,0.3,1,false);
				local emitter = particle_manager:get_resource(playerRenderable.m_ParticleEmitter)
				emitter.m_vPos = playerRenderable:get_position()+ playerRenderable.m_EmitterOffset
				emitter:set_visible(true)
			else
				player.pressed_return = true
				--playerRenderable:execute_action(anim_attack,0,0.3,1,false);
			end
		end
		
		if player.execute_return_pr then
			player.execute_return_pr = false;
			player.super_piky_attack = true;
			playerRenderable:clear_cycle(anim_idle,0);
			playerRenderable:clear_cycle(anim_run,0);
			playerRenderable:execute_action(anim_attack,0,0.3,1,false);
		end
		
		if player.super_piky_attack then
			if not playerRenderable:is_action_animation_active() then
				player.super_piky_attack = false;
			end
		end
	
		if not player_controller.m_isGrounded then
			if  player_controller.m_isAttack then	-- Aqui ir poniendo siempre que se tenga que parar alguna acción si no está grounded
				player.get_player_controller():update_character_extents(true, m_ReduceCollider);
				player_controller.m_isAttack = false
			end
		end
		
		--ACELERACION/INERCIA (SUELO)
		local stopping = false;
		if not player_controller.m_isJumping and not player_controller.m_isDoubleJumping and not _land and player_controller.m_isAttack == false and not player.is_hit and not player.is_dead and not playerRenderable.m_VanishActive and player.super_piky_attack == false then
			if _isQuiet == false then
				local xValue = mov.x * acceleration;
				local zValue = mov.z * acceleration;
				mov = Vect3f(xValue, mov.y, zValue);
			else
				stopping = true;
				if acceleration > 0 then
					mov = inertia * acceleration;
				end
			end
		else
			inertia = Vect3f(0,0,0);
		end
		
		--ACELERACION/INERCIA (AIRE)
		if player_controller.m_isJumping or player_controller.m_isDoubleJumping or _land then
			if _isQuiet == false then
				local xValue = mov.x * acceleration;
				local zValue = mov.z * acceleration;
				mov = Vect3f(xValue,mov.y, zValue);
			else
				if acceleration > 0 then
					local inertX = inertiaJump.x * acceleration;
					local inertZ = inertiaJump.z * acceleration;
					mov = Vect3f(inertX, mov.y, inertZ);
				end
			end
		end
		
		-- GRAVITY
		if(_land)then
			_actualGravityForce = _actualGravityForce + (_gravityForce * l_ElapsedTime);
			mov.y = mov.y + _actualGravityForce * l_ElapsedTime;
		else
			_actualGravityForce = 0;
		end
		
		-- IMAN
		if _land and player.use_iman and (player.iman_pos.x ~= 0 or player.iman_pos.y ~= 0 or player.iman_pos.z ~= 0) then
			local playerPosZX = Vect3f(player_controller:get_position().x, 0, player_controller:get_position().z);
			local imanZX = Vect3f(player.iman_pos.x,0,player.iman_pos.z);
			local distancePlayerImanX = math.abs(get_distance_between_points(playerPosZX,imanZX));
			local distancePlayerImanY = math.abs(player_controller:get_position().y, player.iman_pos.y);
			if (distancePlayerImanX > 0.1) then
				local movimientoAux = Vect3f(imanZX.x-playerPosZX.x, 0, imanZX.z-playerPosZX.z);
				local variableMultiplicadora = iman_force;
				if distancePlayerImanY > 0 then
					variableMultiplicadora = (iman_force * distancePlayerImanX) / distancePlayerImanY;
				end
				coreInstance:trace("vM: "..tostring(variableMultiplicadora));
				movimientoAux = movimientoAux * l_ElapsedTime * variableMultiplicadora;
				mov = Vect3f(movimientoAux.x, mov.y, movimientoAux.z);
			end
		end
		
		-- MOVER AL PLAYERCONTROLLER
		if player_controller.m_isJumping or player_controller.m_isDoubleJumping then
			player_controller:move(mov, l_ElapsedTime);
		else
			player_controller:is_grounded(mov,l_ElapsedTime,false);
		end
		
		-- FIJAR AL PLAYER EN 2D
		if player_controller.m_is3D == false then
			if currentWP ~= nil and nextWP ~= nil then
				camPathVector = nextWP - currentWP
				local playerVec = (player_controller.m_PhysicController:get_position()) - currentWP
				camPathVector.y = 0
				playerVec.y = 0
				if playerVec.x ~= 0 or playerVec.y ~= 0 or playerVec.z ~= 0 then
					local modul = playerVec:length()
					playerVec:normalize(1)
					camPathVector:normalize(1)
					local dot = camPathVector * playerVec
					local moviment = dot * modul
					local playerPos = currentWP + (camPathVector * moviment)
					player_controller.m_PhysicController:set_position(Vect3f(playerPos.x,player_controller.m_PhysicController:get_position().y,playerPos.z))
				end
			end
		end
		
		player_controller:set_position(player_controller.m_PhysicController:get_position());
		if not player.hurt_by_spikes then
			move_character_controller_mesh(player_controller, player_controller:get_position(), player_controller.m_isJumping, player_controller.m_isDoubleJumping);
		end
		if not player_controller.m_isJumping and not player_controller.m_isDoubleJumping and not _land and player.super_piky_attack == false then
			if (mov.x == 0 and mov.z == 0 or stopping) and player.is_hit == false then
				playerRenderable:clear_cycle(anim_run,deccelerationTime);
				playerRenderable:blend_cycle(anim_idle,1,deccelerationTime+0.2);
			else
				if player.is_hit then
					--AQUI VA LA ANIMACION DE RECIBIR DAMAGE
					playerRenderable:clear_cycle(anim_idle,0);
					playerRenderable:clear_cycle(anim_run,0);
					if player.playing_hit == false then
						player.playing_hit = true
						if not player.hurt_by_spikes then
							playerRenderable:execute_action(anim_hurt,0,0.3,1,true);
						else
							player.hurt_by_spikes = false
						end
						inputm:set_game_pad_right_motor_speed(15000, 1);
					else
						if not playerRenderable:is_cycle_animation_active() and not player.hurt_by_spikes then
							player.is_hit = false
							player.playing_hit = false
							inputm:set_game_pad_right_motor_speed(0, 1);
							timer = 0.0
							playerRenderable:remove_action(anim_hurt);
							playerRenderable:blend_cycle(anim_idle,1,0);
							m_damageFeedBackSpeed = m_damageFeedBackSpeedStart
							--coreInstance:trace("Player hit "..tostring(player.is_hit))
						end
					end
				else
					--playerRenderable:clear_cycle(0,0);
					playerRenderable:clear_cycle(anim_idle,accelerationTime);
					playerRenderable:blend_cycle(anim_run,1,accelerationTime);
				end
			end
		end
		
	--[[	if player.hurt_by_spikes then
			local emitter3 = particle_manager:get_resource(playerRenderable.m_ParticleEmitter3)
				if not playerRenderable:is_action_animation_active() then
					--coreInstance:trace(tostring(player.num_hearts))		
					emitter3:set_visible(false)
				--	player.player_take_damage(Vect3f(0,0,0), 0)
				else
									
					local pos_spikes = Vect3f(0.0, 0.0, 0.0)
					--pos_spikes = pos_spikes + player.get_player_controller():get_position()
					pos_spikes = pos_spikes + playerRenderable:get_position()
					--coreInstance:trace(tostring(pos_spikes.x)..", "..tostring(pos_spikes.y)..", "..tostring(pos_spikes.z))
					local emitterOffset = playerRenderable.m_EmitterOffset3
					--pos = pos + playerRenderable:getAnimationBonePosition() 
					pos_spikes.y = pos_spikes.y + 20 *l_ElapsedTime
					playerRenderable:set_position(pos_spikes)
					--player.get_player_controller():set_position(pos_spikes)
					pos_spikes = pos_spikes + emitterOffset
					emitter3.m_vPos = pos_spikes
				end
			end
		]]
		if player.is_dead and fade_step == 0 then
			--coreInstance:trace("Dead ")	
			if not playerRenderable:is_action_animation_active() and not player.dead_in_hole then
					--coreInstance:trace("Dead ".. tostring(player.dead_in_hole))
					--player.check_death_actions()
					fade(2)
					fade_step = 1
			else
				--coreInstance:trace("Animation is runing")
				if player.has_ass_burned then
					coreInstance:trace("ass burned")
					local emitter2 = particle_manager:get_resource(playerRenderable.m_ParticleEmitter2)
					local pos = Vect3f(0.0, 0.0, 0.0);
					pos = pos + player.get_player_controller():get_position()
					local emitterOffset = playerRenderable.m_EmitterOffset2
					pos.y = pos.y + playerRenderable:getAnimationBonePosition().y
					pos = pos + emitterOffset
					emitter2.m_vPos = pos;
				end
				
				if player.hurt_by_spikes then
					local emitter3 = particle_manager:get_resource(playerRenderable.m_ParticleEmitter3)
					local pos_spikes = Vect3f(0.0, 0.0, 0.0)
					--pos_spikes = pos_spikes + player.get_player_controller():get_position()
					pos_spikes = pos_spikes + playerRenderable:get_position()
					--coreInstance:trace(tostring(pos_spikes.x)..", "..tostring(pos_spikes.y)..", "..tostring(pos_spikes.z))
					local emitterOffset = playerRenderable.m_EmitterOffset3
					--pos = pos + playerRenderable:getAnimationBonePosition() 
					pos_spikes.y = pos_spikes.y + 20 *l_ElapsedTime
					playerRenderable:set_position(pos_spikes)
					--player.get_player_controller():set_position(pos_spikes)
					pos_spikes = pos_spikes + emitterOffset
					emitter3.m_vPos = pos_spikes
				end
			end
			if player.dead_in_hole then
				--coreInstance:trace("Piky en boquetee" .. tostring(hole_timer))
				if hole_timer < max_hole_falling then
					hole_timer = hole_timer + l_ElapsedTime
				--	coreInstance:trace("Hole Timer: " .. tostring(hole_timer))
					
				else
					hole_timer = 0.0
					player.dead_in_hole = false
					--player.check_death_actions()
					fade(2)
					fade_step = 1
				end
			end
		end
		if inputm:has_game_pad(1) and act2in:do_action_from_lua("EnableJump") and not jump_enabled then
			jump_enabled = true
		end
	end --Final if Console displayed.
	return 0;
end

function move_character_controller_mesh(_player, _position, _jumping, _doubleJumping)
	local layer = "player"
	if player.vanishing then
		layer = "vanishing"
	end
	local mesh = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str_and_room(layer, player_controller.m_Room):get_resource(piky_mesh_name)
	mesh:set_yaw(_player:get_yaw() + math.pi)
	local pos;
	if (_jumping and not inLoop) or _doubleJumping then
		pos = mesh:getAnimationBonePosition().y;
		local oldPosition = mesh:get_position();
		local mesh_position = Vect3f(_position.x, oldPosition.y, _position.z)
		mesh:set_position(mesh_position)
	else
		pos = mesh:getBonePosition().y;
		local mesh_position = Vect3f(_position.x, _position.y - pos - 0.2, _position.z)
		mesh:set_position(mesh_position)
	end
	coreInstance:getWWSoundManager():SetListenerPosition(mesh:get_position(), mesh:get_position(), Vect3f(0,1,0))
	
end

function get_attack_direction(_directionplayer, _positionplayer)
	local result = coreInstance:get_enemy_manager():get_direction_enemy(_directionplayer, _positionplayer, 20.0, 0.5)
	return result
end