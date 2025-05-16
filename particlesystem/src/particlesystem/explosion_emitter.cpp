#include <particlesystem/explosion_emitter.h>
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace particlesystem {

ExplosionEmitter::ExplosionEmitter(const glm::vec2& position)
    : Emitter(position)
    , particleCount_(20)
    , minSpeed_(1.0f)
    , maxSpeed_(5.0f)
    , minLifetime_(0.5f)
    , maxLifetime_(2.0f)
    , triggered_(false)
    , angleDistribution_(0.0f, 2.0f * glm::pi<float>())
    , speedDistribution_(minSpeed_, maxSpeed_)
    , lifetimeDistribution_(minLifetime_, maxLifetime_) {
    
    // Initialize random number generator with a random seed
    std::random_device rd;
    generator_ = std::mt19937(rd());
}

void ExplosionEmitter::setRate(float rate) {
    // Ignore rate setting since explosion happens instantly
    // But still update the base class value for consistency
    Emitter::setRate(rate);
}

void ExplosionEmitter::setParticleCount(int count) {
    particleCount_ = count;
}

int ExplosionEmitter::getParticleCount() const {
    return particleCount_;
}

void ExplosionEmitter::setSpeedRange(float minSpeed, float maxSpeed) {
    minSpeed_ = minSpeed;
    maxSpeed_ = maxSpeed;
    speedDistribution_ = std::uniform_real_distribution<float>(minSpeed_, maxSpeed_);
}

void ExplosionEmitter::setLifetimeRange(float minLifetime, float maxLifetime) {
    minLifetime_ = minLifetime;
    maxLifetime_ = maxLifetime;
    lifetimeDistribution_ = std::uniform_real_distribution<float>(minLifetime_, maxLifetime_);
}

void ExplosionEmitter::trigger() {
    triggered_ = true;
}

void ExplosionEmitter::emit(std::vector<Particle>& particles, float dt) {
    // Only emit particles if the explosion has been triggered
    if (!triggered_) {
        return;
    }
    
    // Create the explosion particles
    for (int i = 0; i < particleCount_; ++i) {
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
        if (!particleEmitted) {
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
    }
    
    // Reset triggered state after emitting all particles
    triggered_ = false;
}

} // namespace particlesystem 