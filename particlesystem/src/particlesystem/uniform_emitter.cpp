#include <particlesystem/uniform_emitter.h>
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace particlesystem {

UniformEmitter::UniformEmitter(const glm::vec2& position)
    : Emitter(position)
    , minSpeed_(1.0f)
    , maxSpeed_(2.0f)
    , minLifetime_(1.0f)
    , maxLifetime_(3.0f)
    , angleDistribution_(0.0f, 2.0f * glm::pi<float>())
    , speedDistribution_(minSpeed_, maxSpeed_)
    , lifetimeDistribution_(minLifetime_, maxLifetime_) {
    
    // Initialize random number generator with a random seed
    std::random_device rd;
    generator_ = std::mt19937(rd());
}

void UniformEmitter::setSpeedRange(float minSpeed, float maxSpeed) {
    minSpeed_ = minSpeed;
    maxSpeed_ = maxSpeed;
    speedDistribution_ = std::uniform_real_distribution<float>(minSpeed_, maxSpeed_);
}

void UniformEmitter::setLifetimeRange(float minLifetime, float maxLifetime) {
    minLifetime_ = minLifetime;
    maxLifetime_ = maxLifetime;
    lifetimeDistribution_ = std::uniform_real_distribution<float>(minLifetime_, maxLifetime_);
}

void UniformEmitter::emit(std::vector<Particle>& particles, float dt) {
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
                // Generate random angle, speed, and lifetime
                float angle = angleDistribution_(generator_);
                float speed = speedDistribution_(generator_);
                float lifetime = lifetimeDistribution_(generator_);
                
                // Reset the particle
                particle.position = position_;
                particle.velocity = glm::vec2(std::cos(angle), std::sin(angle)) * speed;
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
            
            // Generate random angle, speed, and lifetime
            float angle = angleDistribution_(generator_);
            float speed = speedDistribution_(generator_);
            float lifetime = lifetimeDistribution_(generator_);
            
            // Initialize the new particle
            newParticle.position = position_;
            newParticle.velocity = glm::vec2(std::cos(angle), std::sin(angle)) * speed;
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