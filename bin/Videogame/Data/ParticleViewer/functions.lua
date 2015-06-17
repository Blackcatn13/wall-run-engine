function toClip()
     local id = tonumber(CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('EmitterNumber'))
     -- Get the particle emissor
     local particleEmissor = CCoreLuaWrapper().m_CoreInstance:getParticleManager():get_resource_by_id(id)
     -- Get the emissor type
     local emissorType
     if (particleEmissor.m_Type == 'EMITTER_ESF') then
          emissorType = 'EMITTER_ESF'
     elseif (particleEmissor.m_Type == 'EMITTER_PLANE') then
          emissorType = 'EMITTER_PLANE'
     end
     -- Get the MinSpeedValue
     local MinSpeedValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MinSpeed')
     MinSpeedValue = tonumber(MinSpeedValue)
     if (MinSpeedValue ~= nil) then
          if (MinSpeedValue >= 0) then
               MinSpeedValue = 0.001
          end
     else
          MinSpeedValue = 0.001
     end
     local MinSpeed = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MinSpeedSlider') * MinSpeedValue
     -- Get the MaxSpeedValue
     local MaxSpeedValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MaxSpeed')
     MaxSpeedValue = tonumber(MaxSpeedValue)
     if (MaxSpeedValue ~= nil) then
          if (MaxSpeedValue >= 0) then
               MaxSpeedValue = 0.001
          end
     else
          MaxSpeedValue = 0.001
     end
     local MaxSpeed = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MaxSpeedSlider') * MaxSpeedValue
     -- Get the MaxParticlesValue
     MaxParticles = particleEmissor.m_MaxParticles
     -- Get the MinEmissionTime
     local MinEmissionTimeValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MinEmissionTime')
     MinEmissionTimeValue = tonumber(MinEmissionTimeValue)
     if (MinEmissionTimeValue ~= nil) then
          if (MinEmissionTimeValue >= 0) then
               MinEmissionTimeValue = 0.001
          end
     else
          MinEmissionTimeValue = 0.001
     end
     local MinEmissionTime = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MinEmissionTimeSlider') * MinEmissionTimeValue
     -- Get the MaxEmissionTime
     local MaxEmissionTimeValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MaxEmissionTime')
     MaxEmissionTimeValue = tonumber(MaxEmissionTimeValue)
     if (MaxEmissionTimeValue ~= nil) then
          if (MaxEmissionTimeValue >= 0) then
               MaxEmissionTimeValue = 0.001
          end
     else
          MaxEmissionTimeValue = 0.001
     end
     local MaxEmissionTime = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MaxEmissionTimeSlider') * MaxEmissionTimeValue
     -- Get the MinAge
     local MinAgeValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MinAge')
     MinAgeValue = tonumber(MinAgeValue)
     if (MinAgeValue ~= nil) then
          if (MinAgeValue >= 0) then
               MinAgeValue = 0.001
          end
     else
          MinAgeValue = 0.001
     end
     local MinAge = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MinAgeSlider') * MinAgeValue
     -- Get the MaxAge
     local MaxAgeValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MaxAge')
     MaxAgeValue = tonumber(MaxAgeValue)
     if (MaxAgeValue ~= nil) then
          if (MaxAgeValue >= 0) then
               MaxAgeValue = 0.001
          end
     else
          MaxAgeValue = 0.001
     end
     local MaxAge = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MaxAgeSlider') * MaxAgeValue
     -- Get the MinSize
     local MinSizeValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MinSize')
     MinSizeValue = tonumber(MinSizeValue)
     if (MinSizeValue ~= nil) then
          if (MinSizeValue >= 0) then
               MinSizeValue = 0.001
          end
     else
          MinSizeValue = 0.001
     end
     local MinSize = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MinSizeSlider') * MinSizeValue
     -- Get the MaxSize
     local MaxSizeValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MaxSize')
     MaxSizeValue = tonumber(MaxSizeValue)
     if (MaxSizeValue ~= nil) then
          if (MaxSizeValue >= 0) then
               MaxSizeValue = 0.001
          end
     else
          MaxSizeValue = 0.001
     end
     local MaxSize = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MaxSizeSlider') * MaxSizeValue
     -- Get TextureName

     -- Get the Gravity
     local GravityValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('Gravity')
     GravityValue = tonumber(GravityValue)
     if (GravityValue ~= nil) then
          if (GravityValue >= 0) then
               GravityValue = 0.001
          end
     else
          GravityValue = 0.001
     end
     local Gravity = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('GravitySlider') * GravityValue
     -- Get the MinVelocidadOndulacion
     local MinVelocidadOndulacionValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MinVelocidadOndulacion')
     MinVelocidadOndulacionValue = tonumber(MinVelocidadOndulacionValue)
     if (MinVelocidadOndulacionValue ~= nil) then
          if (MinVelocidadOndulacionValue >= 0) then
               MinVelocidadOndulacionValue = 0.001
          end
     else
          MinVelocidadOndulacionValue = 0.001
     end
     local MinVelocidadOndulacion = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MinVelocidadOndulacionSlider') * MinVelocidadOndulacionValue
     -- Get the MaxVelocidadOndulacion
     local MaxVelocidadOndulacionValue = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getEditableTextBox('MaxVelocidadOndulacion')
     MaxVelocidadOndulacionValue = tonumber(MaxVelocidadOndulacionValue)
     if (MaxVelocidadOndulacionValue ~= nil) then
          if (MaxVelocidadOndulacionValue >= 0) then
               MaxVelocidadOndulacionValue = 0.001
          end
     else
          MaxVelocidadOndulacionValue = 0.001
     end
     local MaxVelocidadOndulacion = CCoreLuaWrapper().m_CoreInstance:getGUIManager():getSliderState('MaxVelocidadOndulacionSlider') * MaxVelocidadOndulacionValue
     -- Generate text to copy
     local text = '<emitter name="' .. particleEmissor:get_name() .. '" type="' .. emissorType .. '" min_speed="' .. tostring(MinSpeed) .. 
     '" max_speed="' .. tostring(MaxSpeed) .. '" max_particles="' .. tostring(MaxParticles) .. '" min_emission_time="' .. tostring(MinEmissionTime) .. 
     '" max_emission_time="' .. tostring(MaxEmissionTime) .. '" min_age="' .. tostring(MinAge) .. '" max_age="' .. tostring(MaxAge) .. 
     '" min_size="' .. tostring(MinSize) .. '" max_size="' .. tostring(MaxSize) .. 
     '" color_1="0.0 0.0 0.0 0.0" color_2="1.0 1.0 1.0 0.0" v_Pos="0.0 0.0 0.0" spawn_dir1="0.0 1 0.0" spawn_dir2="1 0.0 1" texture="' .. 
     particleEmissor.m_sTexture .. '" Gravity="' .. tostring(Gravity) .. '" min_speed_ondulacion="' .. tostring(MinVelocidadOndulacion) .. 
     '" max_speed_ondulacion="' .. tostring(MaxVelocidadOndulacion) .. '" ondulacion_vec1="0.5 0.0 0.5" ondulacion_vec2="1.2 0.0 1.2"/>' 

     CCoreLuaWrapper().m_CoreInstance:copy_to_clipboard(text)
end