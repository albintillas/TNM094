#pragma once

#include <particlesystem/effect.h>
#include <glm/vec2.hpp>

namespace particlesystem {

class GravityWell : public Effect {
public:
    GravityWell(const glm::vec2& position);
    ~GravityWell() override = default;
    
    // Set/get the position of the gravity well
    void setPosition(const glm::vec2& position);
    const glm::vec2& getPosition() const;
    
    // Set/get the radius of influence (beyond this distance, force decreases with distance squared)
    void setRadius(float radius);
    float getRadius() const;
    
    // Implementation of the apply method
    void apply(Particle& particle) override;

private:
    glm::vec2 position_;
    float radius_;
};

} // namespace particlesystem 