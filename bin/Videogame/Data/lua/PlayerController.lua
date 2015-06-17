--local coreInstance = CCoreLuaWrapper().m_CoreInstance;
local is_init=true;
local topPosition = -1000;
local inLoop = false;
local _land = false;
local _fallPosition = Vect3f(-10000, -10000, -10000);
local timer = 0.0
local player_moving = false
local move_3D = true
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
--////////////////////////////////////////////////////////

--coreInstance:get_player_controller().m_mesh = coreInstance:get_renderable_object_layer_manager():get_default_renderable_object_manager():get_resource("SpongePicky");
--coreInstance:get_player_controller().m_mesh:set_yaw(coreInstance:get_player_controller():get_yaw());
function on_update_player_lua(l_ElapsedTime)

	--[[local luaUtil = CCMathLuaUtils();
	local act2in = coreInstance:get_action_to_input();
	local cam_Controller = coreInstance.m_CameraController;--]]
	local active_camera = cam_Controller:get_active_camera();
	local currentWP = nil
	local nextWP = nil
	if active_camera.m_currentWaypoint ~= nil then
		currentWP = active_camera:get_path_point(active_camera.m_currentWaypoint)
		nextWP = active_camera:get_path_point(active_camera.m_nextWaypoint)
	end
	local camObject = active_camera.m_pObject3D;
	--local player_controller = coreInstance:get_player_controller();
	--local lightM = coreInstance:get_light_manager();
	local directionalLight = lightM:get_resource("ShadowLight");
	local playerRenderable = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("player"):get_resource("Piky");
	
	if gui_manager:get_is_displayed_console() == false and gui_manager.m_sCurrentWindows == "Play.xml" then
	
		if act2in:do_action_from_lua("PauseGame") then
			gui_manager:push_windows('Main.xml')
			gui_manager:set_visible_mouse(true)
		end

		if is_init == true then
			is_init = false
			player_controller.m_is3D = true
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
		local altura = directionalLight:get_position().y - player_controller:get_position().y;
		local offsetx = lightDirection.x*altura;
		local offsetz = lightDirection.z*altura;
		directionalLight:set_position(Vect3f(player_controller:get_position().x-offsetx,player_controller:get_position().y+altura,player_controller:get_position().z-offsetz));
		--directionalLight:set_position(Vect3f(player_controller:get_position().x,player_controller:get_position().y+altura,player_controller:get_position().z));
		local dir3D = active_camera:get_direction();
		local dirYaw = camObject:get_yaw();
		local dirNor = Vect3f(math.cos(dirYaw + (math.pi/2)), 0, (math.sin(dirYaw + (math.pi/2))));
		dir3D.y = 0;
		dir3D = luaUtil:normalize(dir3D);
		dirNor.y = 0;
		dirNor = luaUtil:normalize(dirNor);
		local mov = Vect3f(0,0,0);
		local PlayerYaw =  - dirYaw + 1.57;
		
		-- Si dañan al player
		if player.is_hit == true then
			if timer > m_damageTime then
				m_damageFeedBackSpeed = m_damageFeedBackSpeed - m_damageFeedBackSpeedDismin * l_ElapsedTime
			end
			if m_damageFeedBackSpeed < 0.2 then
				m_damageFeedBackSpeed = 0.2
			end
			timer = timer + l_ElapsedTime
			mov = player.vector_damage * m_damageFeedBackSpeed * l_ElapsedTime
			--coreInstance:trace("Time "..tostring(timer))
			--if timer > m_damageTime then
			
		end
		
		
		if act2in:do_action_from_lua("StopMovePlayerBack") or act2in:do_action_from_lua("StopMovePlayerForward")  then
				move_3D = true 
		end
		
		
		
		--///////////////////////////////////////////////////////////
		-- Movimiento del Player en las distintas direcciones. 
		--///////////////////////////////////////////////////////////
		if (player_controller.m_isJumpingMoving == false) and (player_controller.m_isAttack == false) and (player.is_hit == false) then
			if act2in:do_action_from_lua("MoveForward") then
				if player_controller.m_is3D == true then
					player.going_back = false;
					mov = mov + dir3D * player_controller.m_Speed * l_ElapsedTime;
					player_controller.m_Direction3D = dir3D;
					if act2in:do_action_from_lua("MoveRigth")  then
						player_controller:set_yaw(PlayerYaw + 0.7854); -- 45º
						mov = mov - dirNor * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = dir3D - dirNor;
						player_controller.m_isTurned = false;
						player_controller.m_JumpType = 5;
						if move_3D == false then
							move_3D = true
						end
					elseif act2in:do_action_from_lua("MoveLeft")  then
						player_controller:set_yaw(PlayerYaw + 5.497); -- 315º
						mov = mov + dirNor * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = dir3D + dirNor;
						player_controller.m_isTurned = true;
						player_controller.m_JumpType = 8;
						if move_3D == false then
							move_3D = true
						end
					else
						player_controller:set_yaw(PlayerYaw); -- 0º
						player_controller.m_JumpType = 1;
					end
					if player_controller.m_isJumping == true then
						mov = mov * 0.75;
					end
				elseif player_controller.m_is3D == false then
					player_controller:set_yaw(PlayerYaw);
					mov = mov - dirNor * player_controller.m_Speed * l_ElapsedTime;
					player_controller.m_Direction3D = Vect3f(0,0,0) - dirNor;
					--move_3D = false
					if player_controller.m_isJumping == true then
						mov = mov * 0.75;
					end
					player_controller.m_isTurned = false;
					
				end
			elseif act2in:do_action_from_lua("MoveBack") then
				if player_controller.m_is3D == true then
					player.going_back = true;
					mov = mov - dir3D * player_controller.m_Speed * l_ElapsedTime;
					player_controller.m_Direction3D = Vect3f(0,0,0) - dir3D;
					if act2in:do_action_from_lua("MoveRigth")  then
						player_controller:set_yaw(PlayerYaw + 2.356); -- 135º
						mov = mov - dirNor * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = Vect3f(0,0,0) - dir3D - dirNor;
						player_controller.m_isTurned = false;
						player_controller.m_JumpType = 6;
						if move_3D == false then
							move_3D = true
						end
					elseif act2in:do_action_from_lua("MoveLeft") then
						player_controller:set_yaw(PlayerYaw + 3.926); -- 225º
						mov = mov + dirNor * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = Vect3f(0,0,0) - dir3D + dirNor;
						player_controller.m_isTurned = true;
						player_controller.m_JumpType = 7;
						if move_3D == false then
							move_3D = true
						end
					else
						player_controller:set_yaw(PlayerYaw + 3.1415); -- 180º
						player_controller.m_JumpType = 3;
					end
					if player_controller.m_isJumping == true then
						mov = mov * 0.75;
					end
				elseif player_controller.m_is3D == false then
					player_controller:set_yaw(PlayerYaw);
					mov = mov + dirNor * player_controller.m_Speed * l_ElapsedTime;
					player_controller.m_Direction3D = Vect3f(0,0,0) + dirNor;
					--move_3D = false
					if player_controller.m_isJumping == true then
						mov = mov * 0.75;
					end
					player_controller.m_isTurned = true;
					
				end
			elseif act2in:do_action_from_lua("MoveRigth")  then
				if player_controller.m_is3D == true then	
					player_controller:set_yaw(PlayerYaw + 1.57); -- 90º
					player_controller.m_JumpType = 2;
					if move_3D == true then
						mov = mov - dirNor * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = Vect3f(0,0,0) - dirNor;
					else
						player_controller:set_yaw(PlayerYaw);
						mov = mov + dir3D * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = Vect3f(0,0,0) + dir3D;
					end
				else
					player_controller:set_yaw(PlayerYaw); -- 0º
					move_3D = false
					mov = mov - dirNor * player_controller.m_Speed * l_ElapsedTime;
					player_controller.m_Direction3D = Vect3f(0,0,0) - dirNor;
				end
				if player_controller.m_isJumping == true then
					mov = mov * 0.75;
				end
				player_controller.m_isTurned = false;
				
			elseif act2in:do_action_from_lua("MoveLeft")   then
				if player_controller.m_is3D == true then	
					player_controller:set_yaw(PlayerYaw + 4.712); -- 270º
					player_controller.m_JumpType = 4;
					if move_3D == true then
						mov = mov + dirNor * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = Vect3f(0,0,0) + dirNor;
					else
						player_controller:set_yaw(PlayerYaw + 3.1415); -- 180º
						mov = mov - dir3D * player_controller.m_Speed * l_ElapsedTime;
						player_controller.m_Direction3D = Vect3f(0,0,0) - dir3D;
					
					end
				else
					player_controller:set_yaw(PlayerYaw + 3.1415); -- 180º
					move_3D = false
					mov = mov + dirNor * player_controller.m_Speed * l_ElapsedTime;
					player_controller.m_Direction3D = dirNor;
				end	
				
				if player_controller.m_isJumping == true then
					mov = mov * 0.75;
				end
				player_controller.m_isTurned = true;
			
			else
				--player_controller:set_yaw(PlayerYaw);
				player_controller.m_JumpType = 0;
			end
		end
		
		if player_controller.m_is3D == false then
			if player_controller.m_isTurned == false then
				player_controller:set_yaw(PlayerYaw + 1.57); -- 90º
			else
				player_controller:set_yaw(PlayerYaw + 4.712); -- 270º
			end
		end

		-- --///////////////////////////////////////////////////////////
		-- Gravedad que siempre afecta al Player. 
		--///////////////////////////////////////////////////////////
		mov.y = -player_controller.m_Gravity * l_ElapsedTime * player_controller.m_isOnPlatform;
		
		--///////////////////////////////////////////////////////////
		-- Cuando el Player está saltando, su velocidad dependerá del tipo de salto realizado. 
		--///////////////////////////////////////////////////////////
		if _land then
			--local info = SCollisionInfo();
			--local _dirRay = Vect3f(0,-1,0);
			--coreInstance:get_phisics_manager():raycast_closest_actor(player_controller:get_position() - 0.4, _dirRay, 1, info, 1000);
			--_fallPosition = info.m_CollisionPoint;
			--local fallPos = player_controller:get_position();
			--local distance = (fallPos.x - _fallPosition.x) * (fallPos.x - _fallPosition.x) + (fallPos.y - _fallPosition.y) * (fallPos.y - _fallPosition.y) + (fallPos.z - _fallPosition.z) * (fallPos.z - _fallPosition.z);
			--coreInstance:trace(tostring(distance))
			--if distance <= 1.8 then
			if player_controller.m_isGrounded then
				_land = false;
				playerRenderable:clear_cycle(4,0);
			end
		end
		if (player_controller.m_isJumping == true) or (player_controller.m_isFalling) then
				jumpTime = jumpTime + l_ElapsedTime;
			--if player_controller.m_CurrentJumpForce < 0 then
				--player_controller.m_CurrentJumpForce = player_controller.m_CurrentJumpForce - (player_controller.m_Gravity * l_ElapsedTime);
			--else
				--player_controller.m_CurrentJumpForce = player_controller.m_CurrentJumpForce - (player_controller.m_GravityJump * l_ElapsedTime);
			--end

			if player_controller.m_isJumping then
				mov.y = playerRenderable:getBoneMovement().y;
			end
			if inLoop then
				mov.y = 0;
			end
			if (not playerRenderable:is_cycle_animation_active()) and inLoop == false then
				player_controller.m_isFalling = true;
				player_controller.m_isJumpingMoving = false;
				jumpTime = 0;
			end
			if player_controller.m_JumpingTime > AirTime then
				player_controller.m_isJumping = false;
				inLoop = false;
				topPosition = -1000;
				_land = true;
				--local info = SCollisionInfo();
				--local _dirRay = Vect3f(0,-1,0);
				--coreInstance:get_phisics_manager():raycast_closest_actor(player_controller:get_position() - 0.4, _dirRay, 1, info, 1000);
				playerRenderable:clear_cycle(3,0.5);
				playerRenderable:updateSkeleton(l_ElapsedTime);
				--playerRenderable:execute_action(4,0,0.3,1,false);
				playerRenderable:blend_cycle(4,1,0.3);
				--_fallPosition = info.m_CollisionPoint;
			else
				if player_controller.m_isFalling then
					local positionOld = playerRenderable:get_position();
					local auxPosition = playerRenderable:getAnimationBonePosition().y; 
					local newPosition = Vect3f(positionOld.x, auxPosition, positionOld.z);
					playerRenderable:set_position(newPosition);
					playerRenderable:blend_cycle(3,1,0.5);
					playerRenderable:updateSkeleton(l_ElapsedTime);
					playerRenderable:remove_action(2);
					playerRenderable:clear_cycle(0,0);
					playerRenderable:clear_cycle(1,0);
					playerRenderable:updateSkeleton(l_ElapsedTime);
					player_controller.m_isFalling = false;
					inLoop = true;
				end
				player_controller.m_JumpingTime = player_controller.m_JumpingTime + l_ElapsedTime;
			end
			if player_controller.m_isJumpingMoving == true then
				if player_controller.m_is3D == false then
					if player_controller.m_isTurned == false then
						--mov = mov - dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov - dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					else
						--mov = mov + dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov + dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					end
				else
					if player_controller.m_JumpType == 1 then --Forward
						--mov = mov + dir3D * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov + dir3D * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 2 then --Right
						--mov = mov - dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov - dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 3 then --Back
						--mov = mov - dir3D * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov - dir3D * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 4 then --Left
						--mov = mov + dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov + dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 5 then --Forward-Right
						--mov = mov + dir3D * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						--mov = mov - dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov + dir3D * player_controller.m_JumpSpeed * l_ElapsedTime;
						mov = mov - dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 6 then --Back-Right
						--mov = mov - dir3D * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						--mov = mov - dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov - dir3D * player_controller.m_JumpSpeed * l_ElapsedTime;
						mov = mov - dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 7 then --Back-Left
						--mov = mov - dir3D * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						--mov = mov + dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov - dir3D * player_controller.m_JumpSpeed * l_ElapsedTime;
						mov = mov + dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					elseif player_controller.m_JumpType == 8 then --Forward-Left
						--mov = mov + dir3D * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						--mov = mov + dirNor * player_controller.m_JumpForce*1.5 * l_ElapsedTime;
						mov = mov + dir3D * player_controller.m_JumpSpeed * l_ElapsedTime;
						mov = mov + dirNor * player_controller.m_JumpSpeed * l_ElapsedTime;
					end
				end
			end
		end
		if player_controller.m_isAttack == false then
			contador = contador + l_ElapsedTime;
		end
		if contador > timeAttack then
			canAttack = true;
		end
		
		if player_controller.m_isAttack == true then
			--coreInstance:trace("ATACANDOOO");
			if false then
				player_controller.m_isAttack = false;
			else
				if player_controller.m_CurrentAttackForce > 0.5 then	
					player_controller.m_CurrentAttackForce = player_controller.m_CurrentAttackForce - (m_AttackGravity * l_ElapsedTime);
					mov = player_controller.m_Direction3D * player_controller.m_CurrentAttackForce * AttackSpeed * l_ElapsedTime;
					if AttackSpeed > 0.05 then
						AttackSpeed = AttackSpeed - AttackDismin * l_ElapsedTime;
					end
					mov.y = 0.0;
					m_AttackGravity = m_AttackGravity + AtackGravityModifier * l_ElapsedTime;
					--l_ElapsedTime = l_ElapsedTime / AtackGravityModifier;
					--AtackGravityModifier = AtackGravityModifier * 2;
				else
					local mesh = playerRenderable;
					mesh:set_position(player_controller:get_position());
					player_controller.m_isAttack = false;
				end
			end
		end
		
		--///////////////////////////////////////////////////////////
		-- Acción de saltar del Player. Puede realizar 2 saltos distintos (de longitud, y salto vertical). 
		--///////////////////////////////////////////////////////////
		if (act2in:do_action_from_lua("Jump")) and (player_controller.m_isJumping == false and _land == false and player_controller.m_isAttack == false and player.is_hit == false and player_controller.m_isGrounded) then
			--coreInstance:trace(tostring(player_controller.m_isJumping));
			--coreInstance:trace(tostring(_land));
			--coreInstance:getWWSoundManager():PlayEvent("Jump", "Piky");
			player_controller.m_JumpingTime = 0;
			playerRenderable:clear_cycle(0,0.2);
			playerRenderable:clear_cycle(1,0.2);
			playerRenderable:updateSkeleton(l_ElapsedTime);
			playerRenderable:execute_action(2,0.1,0,1,true);
			playerRenderable:updateSkeleton(l_ElapsedTime);
			player_controller.m_isJumping = true;
			--player_controller.m_CurrentJumpForce = player_controller.m_JumpForce;
			if player_controller.m_is3D == false then
				if act2in:do_action_from_lua("MoveRigth") then
					player_controller.m_isJumpingMoving = true;
				elseif act2in:do_action_from_lua("MoveLeft") then
					player_controller.m_isJumpingMoving = true;
				end
			else
				if player_controller.m_JumpType >= 1 then
					player_controller.m_isJumpingMoving = true;
				end
			end
			if (act2in:do_action_from_lua("SuperJump")) then
				player_controller.m_CurrentJumpForce = superjumForce*2;
			end
		end
		
		--///////////////////////////////////////////////////////////
		-- Acción de atacar del Player. Realiza un impulso hacia adelante. 
		--/////////////////////////////////////////////////////////// 
		if (act2in:do_action_from_lua("Attack") and not player_controller.m_isJumping and not _land and player_controller.m_isAttack == false and canAttack == true and player.is_hit == false) then--) and (player_controller.m_isAttack == false) then
			canAttack = false;
			contador = 0;
			m_AttackGravity = AttackGravityStart;
			AttackSpeed = AttackStartSpeed;
			player_controller.m_CurrentAttackForce = player_controller.m_AttackForce;
			player_controller.m_Direction3D =  get_attack_direction(player_controller.m_Direction3D, player_controller.m_PhysicController:get_position())
			mov = player_controller.m_Direction3D * player_controller.m_CurrentAttackForce * AttackSpeed * l_ElapsedTime;
			mov.y = 0.0;
			player_controller.m_isAttack = true;
			playerRenderable:execute_action(5,0,0.3,1,false);
		end
		
		
		
		if player_controller.m_isJumping then
			player_controller:move(mov, l_ElapsedTime);
		else
			player_controller:is_grounded(mov,l_ElapsedTime);
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
		--coreInstance:getWWSoundManager():SetListenerPosition(player_controller.m_PhysicController:get_position(), Vect3f(1,0,0), Vect3f(0,1,0));
		move_character_controller_mesh(player_controller, player_controller:get_position(), player_controller.m_isJumping);
		if not player_controller.m_isJumping and not _land then
			if mov.x == 0 and mov.z == 0 then
				playerRenderable:clear_cycle(1,0);
				playerRenderable:blend_cycle(0,1,0);
			else
				if player.is_hit then
					--AQUI VA LA ANIMACION DE RECIBIR DAMAGE
					playerRenderable:clear_cycle(0,0);
					playerRenderable:clear_cycle(1,0);
					coreInstance:trace(tostring(player_controller.m_isGrounded));
					if player.playing_hit == false then
						player.playing_hit = true
						playerRenderable:execute_action(6,0,0.3,1,true);
					else
						if not playerRenderable:is_cycle_animation_active() then
							player.is_hit = false
							player.playing_hit = false
							timer = 0.0
							playerRenderable:remove_action(6);
							playerRenderable:blend_cycle(0,1,0);
							m_damageFeedBackSpeed = m_damageFeedBackSpeedStart
							--coreInstance:trace("Player hit "..tostring(player.is_hit))
						end
					end
				else
					playerRenderable:clear_cycle(0,0);
					playerRenderable:blend_cycle(1,1,0);
				end
			end
		end
	end --Final if Console displayed.
	return 0;
end

function move_character_controller_mesh(_player, _position, _jumping)
	local mesh = coreInstance:get_renderable_object_layer_manager():get_renderable_objects_manager_by_str("player"):get_resource("Piky")
	mesh:set_yaw(_player:get_yaw() + math.pi)
	local pos;
	if _jumping and not inLoop then
		pos = mesh:getAnimationBonePosition().y;
		local oldPosition = mesh:get_position();
		local mesh_position = Vect3f(_position.x, oldPosition.y, _position.z)
		mesh:set_position(mesh_position)
	else
		pos = mesh:getBonePosition().y;
		local mesh_position = Vect3f(_position.x, _position.y - pos - 0.2, _position.z)
		mesh:set_position(mesh_position)
	end
	
end

function get_attack_direction(_directionplayer, _positionplayer)
	--local result = Vect3f(-1.0,0,0)
	--local result = Vect3f(_directionplayer.x, _directionplayer.y, _directionplayer.z)
	local result = coreInstance:get_enemy_manager():get_direction_enemy(_directionplayer, _positionplayer, 20.0, 0.5)
	return result
end