#include <example/particle_demo.h>
#include <glm/gtc/constants.hpp>
#include <particlesystem/transform.hpp>
#include <algorithm>
#include <imgui.h>

namespace example {

ParticleDemo::ParticleDemo()
    : placementMode_(PlacementMode::None), 
      selectedType_(SelectedType::None), 
      selectedIndex_(0),
      useBoundaries_(true),
      boundaryRestitution_(0.8f) {
    
    // Initialize the particle system with no emitters
    // Boundaries are now handled directly in the keepParticlesWithinBounds method
    
    // Preallocate rendering data
    positions_.reserve(1000);
    colors_.reserve(1000);
    sizes_.reserve(1000);
    
    // Preallocate marker data
    markerPositions_.reserve(100);
    markerColors_.reserve(100);
    markerSizes_.reserve(100);
}

void ParticleDemo::update(double time, float dt, const glm::vec2& mousePos) {
    // Update all emitters and effects
    for (auto& emitter : emitters_) {
        // Check if it's an explosion emitter that needs to be triggered
        auto explosionEmitter = std::dynamic_pointer_cast<ps::ExplosionEmitter>(emitter);
        if (explosionEmitter) {
            // Explosion emitters need to be triggered to emit particles
            explosionEmitter->trigger();
        }
    }
    
    // Update all wind effects
    for (auto& effect : effects_) {
        auto wind = std::dynamic_pointer_cast<ps::Wind>(effect);
        if (wind) {
            wind->update(static_cast<float>(time));
        }
    }
    
    // Update the particle system
    system_.update(dt);
    
    // Apply boundary constraints if enabled
    if (useBoundaries_) {
        keepParticlesWithinBounds();
    }
    
    // Get particle data for rendering
    system_.getParticleData(positions_, colors_, sizes_);
    
    // Update markers for emitters and effects
    updateMarkers();
}

void ParticleDemo::keepParticlesWithinBounds() {
    // Get all particles from the system
    const auto& particles = system_.getParticles();
    
    // Make a copy we can modify (since getParticles() returns a const reference)
    std::vector<ps::Particle> updatedParticles = particles;
    
    // Loop through all particles and keep them within bounds [-1, 1] in both x and y
    for (auto& particle : updatedParticles) {
        if (!particle.alive) continue;
        
        bool bounced = false;
        
        // Check and handle X boundaries
        if (particle.position.x < -1.0f) {
            particle.position.x = -1.0f;
            if (particle.velocity.x < 0) {
                particle.velocity.x = -particle.velocity.x * boundaryRestitution_;
                bounced = true;
            }
        }
        else if (particle.position.x > 1.0f) {
            particle.position.x = 1.0f;
            if (particle.velocity.x > 0) {
                particle.velocity.x = -particle.velocity.x * boundaryRestitution_;
                bounced = true;
            }
        }
        
        // Check and handle Y boundaries
        if (particle.position.y < -1.0f) {
            particle.position.y = -1.0f;
            if (particle.velocity.y < 0) {
                particle.velocity.y = -particle.velocity.y * boundaryRestitution_;
                bounced = true;
            }
        }
        else if (particle.position.y > 1.0f) {
            particle.position.y = 1.0f;
            if (particle.velocity.y > 0) {
                particle.velocity.y = -particle.velocity.y * boundaryRestitution_;
                bounced = true;
            }
        }
    }
    
    // Update the particle system with our boundary-corrected particles
    system_.setParticles(updatedParticles);
}

const std::vector<glm::vec2>& ParticleDemo::getPositions() const {
    return positions_;
}

const std::vector<glm::vec4>& ParticleDemo::getColors() const {
    return colors_;
}

const std::vector<float>& ParticleDemo::getSizes() const {
    return sizes_;
}

const std::vector<glm::vec2>& ParticleDemo::getMarkerPositions() const {
    return markerPositions_;
}

const std::vector<glm::vec4>& ParticleDemo::getMarkerColors() const {
    return markerColors_;
}

const std::vector<float>& ParticleDemo::getMarkerSizes() const {
    return markerSizes_;
}

void ParticleDemo::updateMarkers() {
    // Clear previous markers
    markerPositions_.clear();
    markerColors_.clear();
    markerSizes_.clear();
    
    // Add emitter markers
    for (size_t i = 0; i < emitters_.size(); ++i) {
        markerPositions_.push_back(emitters_[i]->getPosition());
        
        // Selected emitter has a different color/size
        if (selectedType_ == SelectedType::Emitter && selectedIndex_ == i) {
            markerColors_.push_back(SELECTED_COLOR);
            markerSizes_.push_back(10.25f);  // Larger marker for selected object
        } else {
            // All emitters are blue
            markerColors_.push_back(glm::vec4(0.2f, 0.2f, 0.9f, 1.0f));  // Blue color
            markerSizes_.push_back(10.20f);  // Bigger marker
        }
    }
    
    // Add effect markers
    for (size_t i = 0; i < effects_.size(); ++i) {
        // Determine effect position (different effects store positions differently)
        glm::vec2 position;
        bool hasPosition = false;
        
        auto gravityWell = std::dynamic_pointer_cast<ps::GravityWell>(effects_[i]);
        if (gravityWell) {
            position = gravityWell->getPosition();
            hasPosition = true;
        }
        
        // Add special case for wind
        auto wind = std::dynamic_pointer_cast<ps::Wind>(effects_[i]);
        if (wind) {
            // Get the stored position
            if (wind->hasPosition()) {
                position = wind->getPosition();
                hasPosition = true;
            } else {
                // Fallback to center-right if no position stored
                position = glm::vec2(0.8f, 0.0f);
                hasPosition = true;
            }
            
            // If it's selected, we'll draw an arrow as well
            if (selectedType_ == SelectedType::Effect && selectedIndex_ == i) {
                // Draw the main marker
                markerPositions_.push_back(position);
                markerColors_.push_back(SELECTED_COLOR);
                markerSizes_.push_back(10.25f);
                
                // Add an arrow to show direction
                glm::vec2 dir = wind->getDirection();
                // Normalize the direction if it's not zero
                float len = ps::length(dir);
                if (len > 0.001f) {
                    dir = dir / len;
                    // Add arrow shaft
                    glm::vec2 arrowEnd = position + dir * 0.15f;
                    markerPositions_.push_back(arrowEnd);
                    markerColors_.push_back(SELECTED_COLOR);
                    markerSizes_.push_back(5.0f);
                }
            } else {
                markerPositions_.push_back(position);
                markerColors_.push_back(glm::vec4(0.9f, 0.2f, 0.2f, 1.0f));  // Red color
                markerSizes_.push_back(10.20f);
            }
            continue; // Skip the rest of the loop for wind
        }
        
        // Skip effects without a clear position
        if (!hasPosition) continue;
        
        markerPositions_.push_back(position);
        
        // Selected effect has a different color/size
        if (selectedType_ == SelectedType::Effect && selectedIndex_ == i) {
            markerColors_.push_back(SELECTED_COLOR);
            markerSizes_.push_back(10.25f);  // Larger marker for selected object
        } else {
            // All effects are red
            markerColors_.push_back(glm::vec4(0.9f, 0.2f, 0.2f, 1.0f));  // Red color
            markerSizes_.push_back(10.20f);  // Bigger marker
        }
    }
}

void ParticleDemo::handleMouseClick(const glm::vec2& mousePos) {
    // If we're in placement mode, create the appropriate emitter or effect
    if (placementMode_ != PlacementMode::None) {
        switch (placementMode_) {
            case PlacementMode::UniformEmitter:
                createUniformEmitter(mousePos);
                break;
            case PlacementMode::DirectionalEmitter:
                createDirectionalEmitter(mousePos);
                break;
            case PlacementMode::ExplosionEmitter:
                createExplosionEmitter(mousePos);
                break;
            case PlacementMode::GravityWell:
                createGravityWell(mousePos);
                break;
            case PlacementMode::Wind:
                createWind(mousePos);
                break;
            default:
                break;
        }
        
        // After placing, go back to selection mode
        placementMode_ = PlacementMode::None;
    } else {
        // Otherwise, try to select an object at the clicked position
        selectObjectAtPosition(mousePos);
    }
}

void ParticleDemo::setPlacementMode(PlacementMode mode) {
    placementMode_ = mode;
    // Clear selection when changing to placement mode
    selectedType_ = SelectedType::None;
}

void ParticleDemo::createUniformEmitter(const glm::vec2& position) {
    auto emitter = std::make_shared<ps::UniformEmitter>(position);
    emitter->setRate(20.0f);
    emitter->setSpeedRange(0.1f, 0.3f);
    emitter->setLifetimeRange(3.0f, 5.0f);
    
    system_.addEmitter(emitter);
    emitters_.push_back(emitter);
    
    // Select the newly created emitter
    selectedType_ = SelectedType::Emitter;
    selectedIndex_ = emitters_.size() - 1;
}

void ParticleDemo::createDirectionalEmitter(const glm::vec2& position) {
    // Default direction is upward
    auto emitter = std::make_shared<ps::DirectionalEmitter>(position, glm::vec2(0.0f, 1.0f));
    emitter->setRate(15.0f);
    emitter->setSpread(glm::pi<float>() / 12.0f);  // 15 degrees
    emitter->setSpeedRange(0.2f, 0.4f);
    emitter->setLifetimeRange(3.0f, 6.0f);
    
    system_.addEmitter(emitter);
    emitters_.push_back(emitter);
    
    // Select the newly created emitter
    selectedType_ = SelectedType::Emitter;
    selectedIndex_ = emitters_.size() - 1;
}

void ParticleDemo::createExplosionEmitter(const glm::vec2& position) {
    auto emitter = std::make_shared<ps::ExplosionEmitter>(position);
    emitter->setParticleCount(20);  
    emitter->setSpeedRange(0.3f, 0.7f);
    emitter->setLifetimeRange(1.5f, 2.5f);
    
    system_.addEmitter(emitter);
    emitters_.push_back(emitter);
    
    // Select the newly created emitter
    selectedType_ = SelectedType::Emitter;
    selectedIndex_ = emitters_.size() - 1;
}

void ParticleDemo::createGravityWell(const glm::vec2& position) {
    auto gravityWell = std::make_shared<ps::GravityWell>(position);
    gravityWell->setStrength(0.1f);
    gravityWell->setRadius(0.5f);
    
    system_.addEffect(gravityWell);
    effects_.push_back(gravityWell);
    
    // Select the newly created effect
    selectedType_ = SelectedType::Effect;
    selectedIndex_ = effects_.size() - 1;
}

void ParticleDemo::createWind(const glm::vec2& position) {
    // Create a wind effect pointing rightward by default
    // But store the position where it was created
    auto wind = std::make_shared<ps::Wind>(glm::vec2(1.0f, 0.0f));
    wind->setStrength(0.05f);
    wind->setVarying(true);
    
    // Store the position for visualization
    wind->setPosition(position);
    
    system_.addEffect(wind);
    effects_.push_back(wind);
    
    // Select the newly created effect
    selectedType_ = SelectedType::Effect;
    selectedIndex_ = effects_.size() - 1;
}

void ParticleDemo::selectObjectAtPosition(const glm::vec2& position) {
    // Selection distance threshold
    constexpr float SELECTION_THRESHOLD = 0.1f;
    
    // Try to select an emitter
    float closestDistEmitter = SELECTION_THRESHOLD;
    size_t closestEmitterIdx = 0;
    bool foundEmitter = false;
    
    for (size_t i = 0; i < emitters_.size(); ++i) {
        float dist = ps::length(emitters_[i]->getPosition() - position);
        if (dist < closestDistEmitter) {
            closestDistEmitter = dist;
            closestEmitterIdx = i;
            foundEmitter = true;
        }
    }
    
    // Try to select an effect
    float closestDistEffect = SELECTION_THRESHOLD;
    size_t closestEffectIdx = 0;
    bool foundEffect = false;
    
    for (size_t i = 0; i < effects_.size(); ++i) {
        // Try to get position (different effects store positions differently)
        glm::vec2 effectPos;
        bool hasPosition = false;
        
        auto gravityWell = std::dynamic_pointer_cast<ps::GravityWell>(effects_[i]);
        if (gravityWell) {
            effectPos = gravityWell->getPosition();
            hasPosition = true;
        }
        
        // Check for wind effects
        auto wind = std::dynamic_pointer_cast<ps::Wind>(effects_[i]);
        if (wind && wind->hasPosition()) {
            effectPos = wind->getPosition();
            hasPosition = true;
        }
        
        // Skip effects without a clear position
        if (!hasPosition) continue;
        
        float dist = ps::length(effectPos - position);
        if (dist < closestDistEffect) {
            closestDistEffect = dist;
            closestEffectIdx = i;
            foundEffect = true;
        }
    }
    
    // Select the closest object (emitter or effect)
    if (foundEmitter && foundEffect) {
        if (closestDistEmitter <= closestDistEffect) {
            selectedType_ = SelectedType::Emitter;
            selectedIndex_ = closestEmitterIdx;
        } else {
            selectedType_ = SelectedType::Effect;
            selectedIndex_ = closestEffectIdx;
        }
    } else if (foundEmitter) {
        selectedType_ = SelectedType::Emitter;
        selectedIndex_ = closestEmitterIdx;
    } else if (foundEffect) {
        selectedType_ = SelectedType::Effect;
        selectedIndex_ = closestEffectIdx;
    } else {
        // Nothing selected
        selectedType_ = SelectedType::None;
    }
}

void ParticleDemo::renderUI() {
    // Always show available creation options
    if (placementMode_ == PlacementMode::None) {
        // Display available emitter and effect types to create
        if (ImGui::TreeNode("Add Emitter")) {
            if (ImGui::Button("Uniform Emitter")) {
                setPlacementMode(PlacementMode::UniformEmitter);
            }
            if (ImGui::Button("Directional Emitter")) {
                setPlacementMode(PlacementMode::DirectionalEmitter);
            }
            if (ImGui::Button("Explosion Emitter")) {
                setPlacementMode(PlacementMode::ExplosionEmitter);
            }
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("Add Effect")) {
            if (ImGui::Button("Gravity Well")) {
                setPlacementMode(PlacementMode::GravityWell);
            }
            if (ImGui::Button("Wind")) {
                setPlacementMode(PlacementMode::Wind);
            }
            ImGui::TreePop();
        }
    } else {
        // Show placement instructions and cancel button
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Click to place the object");
        if (ImGui::Button("Cancel Placement")) {
            placementMode_ = PlacementMode::None;
        }
    }
    
    ImGui::Separator();
    
    // Show properties of the selected object
    if (selectedType_ == SelectedType::Emitter && selectedIndex_ < emitters_.size()) {
        auto& emitter = emitters_[selectedIndex_];
        
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Selected Emitter");
        
        // Common emitter properties
        float rate = emitter->getRate();
        if (ImGui::SliderFloat("Emission Rate", &rate, 1.0f, 100.0f)) {
            emitter->setRate(rate);
        }
        
        // Position editing
        glm::vec2 position = emitter->getPosition();
        if (ImGui::SliderFloat2("Position", &position.x, -1.0f, 1.0f)) {
            emitter->setPosition(position);
        }
        
        // Type-specific properties
        auto uniformEmitter = std::dynamic_pointer_cast<ps::UniformEmitter>(emitter);
        if (uniformEmitter) {
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Uniform Emitter");
            
            // Would need access to the private members, so we'll recreate them
            static float minSpeed = 0.1f;
            static float maxSpeed = 0.3f;
            static float minLifetime = 3.0f;
            static float maxLifetime = 5.0f;
            
            if (ImGui::SliderFloat("Min Speed", &minSpeed, 0.01f, maxSpeed)) {
                uniformEmitter->setSpeedRange(minSpeed, maxSpeed);
            }
            if (ImGui::SliderFloat("Max Speed", &maxSpeed, minSpeed, 1.0f)) {
                uniformEmitter->setSpeedRange(minSpeed, maxSpeed);
            }
            if (ImGui::SliderFloat("Min Lifetime", &minLifetime, 0.1f, maxLifetime)) {
                uniformEmitter->setLifetimeRange(minLifetime, maxLifetime);
            }
            if (ImGui::SliderFloat("Max Lifetime", &maxLifetime, minLifetime, 10.0f)) {
                uniformEmitter->setLifetimeRange(minLifetime, maxLifetime);
            }
        }
        
        auto directionalEmitter = std::dynamic_pointer_cast<ps::DirectionalEmitter>(emitter);
        if (directionalEmitter) {
            ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.8f, 1.0f), "Directional Emitter");
            
            glm::vec2 direction = directionalEmitter->getDirection();
            if (ImGui::SliderFloat2("Direction", &direction.x, -1.0f, 1.0f)) {
                directionalEmitter->setDirection(direction);
            }
            
            float spread = directionalEmitter->getSpread();
            if (ImGui::SliderFloat("Spread (radians)", &spread, 0.0f, glm::pi<float>())) {
                directionalEmitter->setSpread(spread);
            }
            
            // Would need access to the private members, so we'll recreate them
            static float minSpeed = 0.2f;
            static float maxSpeed = 0.4f;
            static float minLifetime = 3.0f;
            static float maxLifetime = 6.0f;
            
            if (ImGui::SliderFloat("Min Speed", &minSpeed, 0.01f, maxSpeed)) {
                directionalEmitter->setSpeedRange(minSpeed, maxSpeed);
            }
            if (ImGui::SliderFloat("Max Speed", &maxSpeed, minSpeed, 1.0f)) {
                directionalEmitter->setSpeedRange(minSpeed, maxSpeed);
            }
            if (ImGui::SliderFloat("Min Lifetime", &minLifetime, 0.1f, maxLifetime)) {
                directionalEmitter->setLifetimeRange(minLifetime, maxLifetime);
            }
            if (ImGui::SliderFloat("Max Lifetime", &maxLifetime, minLifetime, 10.0f)) {
                directionalEmitter->setLifetimeRange(minLifetime, maxLifetime);
            }
        }
        
        auto explosionEmitter = std::dynamic_pointer_cast<ps::ExplosionEmitter>(emitter);
        if (explosionEmitter) {
            ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "Explosion Emitter");
            
            int particleCount = explosionEmitter->getParticleCount();
            if (ImGui::SliderInt("Particle Count", &particleCount, 1, 100)) {
                explosionEmitter->setParticleCount(particleCount);
            }
            
            // Would need access to the private members, so we'll recreate them
            static float minSpeed = 0.3f;
            static float maxSpeed = 0.7f;
            static float minLifetime = 1.5f;
            static float maxLifetime = 2.5f;
            
            if (ImGui::SliderFloat("Min Speed", &minSpeed, 0.01f, maxSpeed)) {
                explosionEmitter->setSpeedRange(minSpeed, maxSpeed);
            }
            if (ImGui::SliderFloat("Max Speed", &maxSpeed, minSpeed, 1.0f)) {
                explosionEmitter->setSpeedRange(minSpeed, maxSpeed);
            }
            if (ImGui::SliderFloat("Min Lifetime", &minLifetime, 0.1f, maxLifetime)) {
                explosionEmitter->setLifetimeRange(minLifetime, maxLifetime);
            }
            if (ImGui::SliderFloat("Max Lifetime", &maxLifetime, minLifetime, 10.0f)) {
                explosionEmitter->setLifetimeRange(minLifetime, maxLifetime);
            }
            
            if (ImGui::Button("Trigger Explosion")) {
                explosionEmitter->trigger();
            }
        }
        
        // Delete button
        if (ImGui::Button("Delete Emitter")) {
            system_.removeEmitter(emitter);
            emitters_.erase(emitters_.begin() + selectedIndex_);
            selectedType_ = SelectedType::None;
        }
    }
    else if (selectedType_ == SelectedType::Effect && selectedIndex_ < effects_.size()) {
        auto& effect = effects_[selectedIndex_];
        
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Selected Effect");
        
        // Common effect properties
        float strength = effect->getStrength();
        if (ImGui::SliderFloat("Strength", &strength, 0.001f, 1.0f)) {
            effect->setStrength(strength);
        }
        
        bool enabled = effect->isEnabled();
        if (ImGui::Checkbox("Enabled", &enabled)) {
            effect->setEnabled(enabled);
        }
        
        // Type-specific properties
        auto gravityWell = std::dynamic_pointer_cast<ps::GravityWell>(effect);
        if (gravityWell) {
            ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.8f, 1.0f), "Gravity Well");
            
            glm::vec2 position = gravityWell->getPosition();
            if (ImGui::SliderFloat2("Position", &position.x, -1.0f, 1.0f)) {
                gravityWell->setPosition(position);
            }
            
            float radius = gravityWell->getRadius();
            if (ImGui::SliderFloat("Radius", &radius, 0.01f, 2.0f)) {
                gravityWell->setRadius(radius);
            }
        }
        
        auto wind = std::dynamic_pointer_cast<ps::Wind>(effect);
        if (wind) {
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.8f, 1.0f), "Wind");
            
            glm::vec2 direction = wind->getDirection();
            if (ImGui::SliderFloat2("Direction", &direction.x, -1.0f, 1.0f)) {
                wind->setDirection(direction);
            }
            
            bool varying = wind->isVarying();
            if (ImGui::Checkbox("Varying", &varying)) {
                wind->setVarying(varying);
            }
        }
        
        // Delete button
        if (ImGui::Button("Delete Effect")) {
            system_.removeEffect(effect);
            effects_.erase(effects_.begin() + selectedIndex_);
            selectedType_ = SelectedType::None;
        }
    }
}

} // namespace example 