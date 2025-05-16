#include <particlesystem/particle.h>

namespace particlesystem {

Particle::Particle()
    : position(0.0f, 0.0f)
    , velocity(0.0f, 0.0f)
    , force(0.0f, 0.0f)
    , lifetime(0.0f)
    , alive(false) {
}

void Particle::update(float dt) {
    // Forward Euler integration
    velocity += force * dt;
    position += velocity * dt;
    
    // Update lifetime
    lifetime -= dt;
    if (lifetime <= 0.0f) {
        alive = false;
    }
}

void Particle::resetForce() {
    force = glm::vec2(0.0f, 0.0f);
}

} // namespace particlesystem 