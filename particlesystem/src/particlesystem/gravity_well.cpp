#include <particlesystem/gravity_well.h>
#include <particlesystem/transform.hpp>
#include <cmath>

namespace particlesystem {

GravityWell::GravityWell(const glm::vec2& position)
    : position_(position)
    , radius_(100.0f) {
}

void GravityWell::setPosition(const glm::vec2& position) {
    position_ = position;
}

const glm::vec2& GravityWell::getPosition() const {
    return position_;
}

void GravityWell::setRadius(float radius) {
    radius_ = radius;
}

float GravityWell::getRadius() const {
    return radius_;
}

void GravityWell::apply(Particle& particle) {
    if (!enabled_ || !particle.alive) {
        return;
    }
    
    // Calculate direction to gravity well
    glm::vec2 direction = position_ - particle.position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    // Avoid divide by zero and extreme forces when very close
    if (distance < 0.1f) {
        return;
    }
    
    // Normalize the direction
    direction = normalize(direction);
    
    // Calculate force strength based on distance
    float forceMagnitude;
    if (distance < radius_) {
        // Linear force within the radius
        forceMagnitude = strength_;
    } else {
        // Inverse square law outside the radius
        float distanceRatio = radius_ / distance;
        forceMagnitude = strength_ * distanceRatio * distanceRatio;
    }
    
    // Apply the force
    particle.force += direction * forceMagnitude;
}

} // namespace particlesystem 