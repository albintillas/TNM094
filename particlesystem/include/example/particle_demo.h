#pragma once

#include <particlesystem/all.h>
#include <memory>
#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace example {

enum class PlacementMode {
    None,
    UniformEmitter,
    DirectionalEmitter,
    ExplosionEmitter,
    GravityWell,
    Wind
};

class ParticleDemo {
public:
    ParticleDemo();
    ~ParticleDemo() = default;
    
    // Update the demo with time and mouse position
    void update(double time, float dt, const glm::vec2& mousePos);
    
    // Get particle data for rendering
    const std::vector<glm::vec2>& getPositions() const;
    const std::vector<glm::vec4>& getColors() const;
    const std::vector<float>& getSizes() const;
    
    // Get marker data for rendering emitters and effects
    const std::vector<glm::vec2>& getMarkerPositions() const;
    const std::vector<glm::vec4>& getMarkerColors() const;
    const std::vector<float>& getMarkerSizes() const;
    
    // GUI interaction methods
    void renderUI();
    void handleMouseClick(const glm::vec2& mousePos);
    void setPlacementMode(PlacementMode mode);
    
private:
    // Helper methods for creating emitters and effects
    void createUniformEmitter(const glm::vec2& position);
    void createDirectionalEmitter(const glm::vec2& position);
    void createExplosionEmitter(const glm::vec2& position);
    void createGravityWell(const glm::vec2& position);
    void createWind(const glm::vec2& position);
    
    // Select the closest emitter or effect to the mouse position
    void selectObjectAtPosition(const glm::vec2& position);
    
    // Update the marker positions and colors
    void updateMarkers();
    
    // Helper method to keep particles within bounds
    void keepParticlesWithinBounds();
    
    // The particle system
    ps::ParticleSystem system_;
    
    // Particle rendering data
    std::vector<glm::vec2> positions_;
    std::vector<glm::vec4> colors_;
    std::vector<float> sizes_;
    
    // Marker rendering data for emitters and effects
    std::vector<glm::vec2> markerPositions_;
    std::vector<glm::vec4> markerColors_;
    std::vector<float> markerSizes_;
    
    // Emitters
    std::vector<std::shared_ptr<ps::Emitter>> emitters_;
    
    // Effects
    std::vector<std::shared_ptr<ps::Effect>> effects_;
    
    // Placement mode
    PlacementMode placementMode_;
    
    // Selected object
    enum class SelectedType { None, Emitter, Effect } selectedType_;
    size_t selectedIndex_;
    
    // Colors for different emitter/effect types
    static constexpr glm::vec4 UNIFORM_EMITTER_COLOR = {0.2f, 0.8f, 0.2f, 1.0f};
    static constexpr glm::vec4 DIRECTIONAL_EMITTER_COLOR = {0.2f, 0.2f, 0.8f, 1.0f};
    static constexpr glm::vec4 EXPLOSION_EMITTER_COLOR = {0.8f, 0.2f, 0.2f, 1.0f};
    static constexpr glm::vec4 GRAVITY_WELL_COLOR = {0.8f, 0.2f, 0.8f, 1.0f};
    static constexpr glm::vec4 WIND_COLOR = {0.2f, 0.8f, 0.8f, 1.0f};
    static constexpr glm::vec4 SELECTED_COLOR = {1.0f, 1.0f, 1.0f, 1.0f};
    
    // Boundary settings
    bool useBoundaries_;
    float boundaryRestitution_;  // Bounce factor (0-1) for particles hitting boundaries
};

} // namespace example 