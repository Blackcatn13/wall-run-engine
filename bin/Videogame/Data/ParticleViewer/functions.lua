 coreInstance = CCoreLuaWrapper().m_CoreInstance
 instance = CLuaGlobalsWrapper().m_CoreInstance
 gui_manager = coreInstance:getGUIManager()
 particle_manager = coreInstance:getParticleManager()
 billboard_manager = coreInstance:get_billboard_manager()

function toClip()
	coreInstance:trace("To Clipboard")
     local id = tonumber(gui_manager:getEditableTextBox('EmitterNumber'))
     -- Get the particle emissor
     local particleEmissor = particle_manager:get_resource_by_id(id)
     -- Get the emissor type
     local emissorType = "ESF"
     if (particleEmissor.m_Type == CParticleEmitter.EMITTER_PLANE) then
          emissorType = "PLANE"
	 elseif (particleEmissor.m_Type == CParticleEmitter.EMITTER_ESF1) then
		  emissorType = "ESF1"
     end
	 coreInstance:trace(emissorType)
     -- Get the MinSpeedValue
     local MinSpeedValue = gui_manager:getEditableTextBox('MinSpeed')
     MinSpeedValue = tonumber(MinSpeedValue)
     if (MinSpeedValue ~= nil) then
          if (MinSpeedValue >= 0) then
               MinSpeedValue = 0.001
          end
     else
          MinSpeedValue = 0.001
     end
     local MinSpeed = gui_manager:getSliderState('MinSpeedSlider') * MinSpeedValue
     -- Get the MaxSpeedValue
     local MaxSpeedValue = gui_manager:getEditableTextBox('MaxSpeed')
     MaxSpeedValue = tonumber(MaxSpeedValue)
     if (MaxSpeedValue ~= nil) then
          if (MaxSpeedValue >= 0) then
               MaxSpeedValue = 0.001
          end
     else
          MaxSpeedValue = 0.001
     end
     local MaxSpeed = gui_manager:getSliderState('MaxSpeedSlider') * MaxSpeedValue
     -- Get the MaxParticlesValue
     MaxParticles = particleEmissor.m_MaxParticles
     -- Get the MinEmissionTime
     local MinEmissionTimeValue = gui_manager:getEditableTextBox('MinEmissionTime')
     MinEmissionTimeValue = tonumber(MinEmissionTimeValue)
     if (MinEmissionTimeValue ~= nil) then
          if (MinEmissionTimeValue >= 0) then
               MinEmissionTimeValue = 0.001
          end
     else
          MinEmissionTimeValue = 0.001
     end
     local MinEmissionTime = gui_manager:getSliderState('MinEmissionTimeSlider') * MinEmissionTimeValue
     -- Get the MaxEmissionTime
     local MaxEmissionTimeValue = gui_manager:getEditableTextBox('MaxEmissionTime')
     MaxEmissionTimeValue = tonumber(MaxEmissionTimeValue)
     if (MaxEmissionTimeValue ~= nil) then
          if (MaxEmissionTimeValue >= 0) then
               MaxEmissionTimeValue = 0.001
          end
     else
          MaxEmissionTimeValue = 0.001
     end
     local MaxEmissionTime = gui_manager:getSliderState('MaxEmissionTimeSlider') * MaxEmissionTimeValue
     -- Get the MinAge
     local MinAgeValue = gui_manager:getEditableTextBox('MinAge')
     MinAgeValue = tonumber(MinAgeValue)
     if (MinAgeValue ~= nil) then
          if (MinAgeValue >= 0) then
               MinAgeValue = 0.001
          end
     else
          MinAgeValue = 0.001
     end
     local MinAge = gui_manager:getSliderState('MinAgeSlider') * MinAgeValue
     -- Get the MaxAge
     local MaxAgeValue = gui_manager:getEditableTextBox('MaxAge')
     MaxAgeValue = tonumber(MaxAgeValue)
     if (MaxAgeValue ~= nil) then
          if (MaxAgeValue >= 0) then
               MaxAgeValue = 0.001
          end
     else
          MaxAgeValue = 0.001
     end
     local MaxAge = gui_manager:getSliderState('MaxAgeSlider') * MaxAgeValue
     -- Get the MinSize
     local MinSizeValue = gui_manager:getEditableTextBox('MinSize')
     MinSizeValue = tonumber(MinSizeValue)
     if (MinSizeValue ~= nil) then
          if (MinSizeValue >= 0) then
               MinSizeValue = 0.001
          end
     else
          MinSizeValue = 0.001
     end
     local MinSize = gui_manager:getSliderState('MinSizeSlider') * MinSizeValue
     -- Get the MaxSize
     local MaxSizeValue = gui_manager:getEditableTextBox('MaxSize')
     MaxSizeValue = tonumber(MaxSizeValue)
     if (MaxSizeValue ~= nil) then
          if (MaxSizeValue >= 0) then
               MaxSizeValue = 0.001
          end
     else
          MaxSizeValue = 0.001
     end
     local MaxSize = gui_manager:getSliderState('MaxSizeSlider') * MaxSizeValue
     -- Get TextureName

     -- Get the Gravity
     local GravityValue = gui_manager:getEditableTextBox('Gravity')
     GravityValue = tonumber(GravityValue)
     if (GravityValue ~= nil) then
          if (GravityValue >= 0) then
               GravityValue = 0.001
          end
     else
          GravityValue = 0.001
     end
     local Gravity = gui_manager:getSliderState('GravitySlider') * GravityValue
     -- Get the MinVelocidadOndulacion
     local MinVelocidadOndulacionValue = gui_manager:getEditableTextBox('MinVelocidadOndulacion')
     MinVelocidadOndulacionValue = tonumber(MinVelocidadOndulacionValue)
     if (MinVelocidadOndulacionValue ~= nil) then
          if (MinVelocidadOndulacionValue >= 0) then
               MinVelocidadOndulacionValue = 0.001
          end
     else
          MinVelocidadOndulacionValue = 0.001
     end
     local MinVelocidadOndulacion = gui_manager:getSliderState('MinVelocidadOndulacionSlider') * MinVelocidadOndulacionValue
     -- Get the MaxVelocidadOndulacion
     local MaxVelocidadOndulacionValue = gui_manager:getEditableTextBox('MaxVelocidadOndulacion')
     MaxVelocidadOndulacionValue = tonumber(MaxVelocidadOndulacionValue)
     if (MaxVelocidadOndulacionValue ~= nil) then
          if (MaxVelocidadOndulacionValue >= 0) then
               MaxVelocidadOndulacionValue = 0.001
          end
     else
          MaxVelocidadOndulacionValue = 0.001
     end
     local MaxVelocidadOndulacion = gui_manager:getSliderState('MaxVelocidadOndulacionSlider') * MaxVelocidadOndulacionValue
     -- Generate text to copy
     local text = '<emitter name="' .. particleEmissor:get_name() .. '" type="' .. emissorType .. '" min_speed="' .. format_value(MinSpeed) .. 
     '" max_speed="' .. format_value(MaxSpeed) .. '" max_particles="' .. tostring(MaxParticles) .. '" min_emission_time="' .. format_value(MinEmissionTime) .. 
     '" max_emission_time="' .. format_value(MaxEmissionTime) .. '" min_age="' .. format_value(MinAge) .. '" max_age="' .. format_value(MaxAge) .. 
     '" min_size="' .. format_value(MinSize) .. '" max_size="' .. format_value(MaxSize) .. 
     '" color_1="0.0 0.0 0.0 0.0" color_2="1.0 1.0 1.0 0.0" v_Pos="0.0 0.0 0.0" spawn_dir1="0.0 1 0.0" spawn_dir2="1 0.0 1" texture="' .. 
     particleEmissor.m_sTexture .. '" Gravity="' .. format_value(Gravity) .. '" min_speed_ondulacion="' .. format_value(MinVelocidadOndulacion) .. 
     '" max_speed_ondulacion="' .. format_value(MaxVelocidadOndulacion) .. '" ondulacion_vec1="0.5 0.0 0.5" ondulacion_vec2="1.2 0.0 1.2"/>' 

     coreInstance:copy_to_clipboard(text)
end

function format_value(value)
 return string.format("%.5f", value)
end