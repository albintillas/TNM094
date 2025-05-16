#include <particlesystem/directional_emitter.h>
#include <particlesystem/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace particlesystem {

DirectionalEmitter::DirectionalEmitter(const glm::vec2& position, const glm::vec2& direction)
    : Emitter(position)
    , direction_(normalize(direction))
    , spread_(glm::pi<float>() / 8.0f)  // Default 45 degrees spread
    , minSpeed_(1.0f)
    , maxSpeed_(2.0f)
    , minLifetime_(1.0f)
    , maxLifetime_(3.0f)
    , spreadDistribution_(-spread_, spread_)
    , speedDistribution_(minSpeed_, maxSpeed_)
    , lifetimeDistribution_(minLifetime_, maxLifetime_) {
    
    // Initialize random number generator with a random seed
    std::random_device rd;
    generator_ = std::mt19937(rd());
}

void DirectionalEmitter::setDirection(const glm::vec2& direction) {
    direction_ = normalize(direction);
}

const glm::vec2& DirectionalEmitter::getDirection() const {
    return direction_;
}

void DirectionalEmitter::setSpread(float spread) {
    spread_ = spread;
    spreadDistribution_ = std::uniform_real_distribution<float>(-spread_, spread_);
}

float DirectionalEmitter::getSpread() const {
    return spread_;
}

void DirectionalEmitter::setSpeedRange(float minSpeed, float maxSpeed) {
    minSpeed_ = minSpeed;
    maxSpeed_ = maxSpeed;
    speedDistribution_ = std::uniform_real_distribution<float>(minSpeed_, maxSpeed_);
}

void DirectionalEmitter::setLifetimeRange(float minLifetime, float maxLifetime) {
    minLifetime_ = minLifetime;
    maxLifetime_ = maxLifetime;
    lifetimeDistribution_ = std::uniform_real_distribution<float>(minLifetime_, maxLifetime_);
}

void DirectionalEmitter::emit(std::vector<Particle>& particles, float dt) {
    // Accumulate time to control emission rate
    accumulator_ += dt;
    
    // Calculate how many particles to emit this frame
    const float particlesPerSecond = rate_;
    const float timePerParticle = 1.0f / particlesPerSecond;
    
    // Create particles until we've used up our accumulator time
    while (accumulator_ >= timePerParticle) {
        // Find a dead particle to reuse
        bool particleEmitted = false;
        
        for (auto& particle : particles) {
            if (!particle.alive) {
                // Generate random deviation angle, speed, and lifetime
                float angleOffset = spreadDistribution_(generator_);
                float speed = speedDistribution_(generator_);
                float lifetime = lifetimeDistribution_(generator_);
                
                // Rotate the base direction by the random deviation angle using our custom rotate
                glm::vec2 particleDirection = rotate(direction_, angleOffset);
                
                // Reset the particle
                particle.position = position_;
                particle.velocity = particleDirection * speed;
                particle.force = glm::vec2(0.0f, 0.0f);
                particle.lifetime = lifetime;
                particle.alive = true;
                
                particleEmitted = true;
                break;
            }
        }
        
        // If we couldn't find a dead particle, create a new one
        if (!particleEmitted && particles.size() < 10000) {  // Limit to prevent excessive particles
            Particle newParticle;
            
            // Generate random deviation angle, speed, and lifetime
            float angleOffset = spreadDistribution_(generator_);
            float speed = speedDistribution_(generator_);
            float lifetime = lifetimeDistribution_(generator_);
            
            // Rotate the base direction by the random deviation angle using our custom rotate
            glm::vec2 particleDirection = rotate(direction_, angleOffset);
            
            // Initialize the new particle
            newParticle.position = position_;
            newParticle.velocity = particleDirection * speed;
            newParticle.force = glm::vec2(0.0f, 0.0f);
            newParticle.lifetime = lifetime;
            newParticle.alive = true;
            
            particles.push_back(newParticle);
        }
        
        // Subtract the time used to emit a particle
        accumulator_ -= timePerParticle;
    }
}

} // namespace particlesystem 