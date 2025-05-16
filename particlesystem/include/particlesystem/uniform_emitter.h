#pragma once

#include <particlesystem/emitter.h>
#include <random>

namespace particlesystem {

class UniformEmitter : public Emitter {
public:
    UniformEmitter(const glm::vec2& position);
    ~UniformEmitter() override = default;

    // Set the speed range for emitted particles
    void setSpeedRange(float minSpeed, float maxSpeed);

    // Set the lifetime range for emitted particles
    void setLifetimeRange(float minLifetime, float maxLifetime);

    // Implementation of the emit method
    void emit(std::vector<Particle>& particles, float dt) override;

private:
    float minSpeed_;
    float maxSpeed_;
    float minLifetime_;
    float maxLifetime_;
    
    // Random number generation
    std::mt19937 generator_;
    std::uniform_real_distribution<float> angleDistribution_;
    std::uniform_real_distribution<float> speedDistribution_;
    std::uniform_real_distribution<float> lifetimeDistribution_;
};

} // namespace particlesystem 