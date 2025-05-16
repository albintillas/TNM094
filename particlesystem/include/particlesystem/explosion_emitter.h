#pragma once

#include <particlesystem/emitter.h>
#include <random>

namespace particlesystem {

class ExplosionEmitter : public Emitter {
public:
    ExplosionEmitter(const glm::vec2& position);
    ~ExplosionEmitter() override = default;
    
    // Override to ignore rate setting (explosion happens instantly)
    void setRate(float rate) override;
    
    // Set the number of particles to emit in the explosion
    void setParticleCount(int count);
    int getParticleCount() const;
    
    // Set the speed range for emitted particles
    void setSpeedRange(float minSpeed, float maxSpeed);
    
    // Set the lifetime range for emitted particles
    void setLifetimeRange(float minLifetime, float maxLifetime);
    
    // Trigger the explosion
    void trigger();
    
    // Implementation of the emit method
    void emit(std::vector<Particle>& particles, float dt) override;

private:
    int particleCount_;
    float minSpeed_;
    float maxSpeed_;
    float minLifetime_;
    float maxLifetime_;
    bool triggered_;
    
    // Random number generation
    std::mt19937 generator_;
    std::uniform_real_distribution<float> angleDistribution_;
    std::uniform_real_distribution<float> speedDistribution_;
    std::uniform_real_distribution<float> lifetimeDistribution_;
};

} // namespace particlesystem 