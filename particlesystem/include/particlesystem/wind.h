#pragma once

#include <particlesystem/effect.h>
#include <glm/vec2.hpp>

namespace particlesystem {

class Wind : public Effect {
public:
    Wind(const glm::vec2& direction);
    ~Wind() override = default;
    
    // Set/get the direction of the wind
    void setDirection(const glm::vec2& direction);
    const glm::vec2& getDirection() const;
    
    // Set/get whether the wind varies with time
    void setVarying(bool varying);
    bool isVarying() const;
    
    // Position data for visualization/interaction
    void setPosition(const glm::vec2& position);
    const glm::vec2& getPosition() const;
    bool hasPosition() const;
    
    // Implementation of the apply method
    void apply(Particle& particle) override;
    
    // Update wind variation based on time (if varying is enabled)
    void update(float time);

private:
    glm::vec2 direction_;
    glm::vec2 currentDirection_;
    bool varying_;
    glm::vec2 position_;
    bool hasPosition_;
};

} // namespace particlesystem 