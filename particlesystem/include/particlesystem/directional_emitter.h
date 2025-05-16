#pragma once

#include <particlesystem/emitter.h>
#include <random>

namespace particlesystem {

class DirectionalEmitter : public Emitter {
public:
    DirectionalEmitter(const glm::vec2& position, const glm::vec2& direction);
    ~DirectionalEmitter() override = default;

    // Set the direction of emission
    void setDirection(const glm::vec2& direction);
    const glm::vec2& getDirection() const;

    // Set the angle spread (in radians) from the direction
    void setSpread(float spread);
    float getSpread() const;
    
    // Set the speed range for emitted particles
    void setSpeedRange(float minSpeed, float maxSpeed);
    
    // Set the lifetime range for emitted particles
    void setLifetimeRange(float minLifetime, float maxLifetime);

    // Implementation of the emit method
    void emit(std::vector<Particle>& particles, float dt) override;

private:
    glm::vec2 direction_;
    float spread_;
    float minSpeed_;
    float maxSpeed_;
    float minLifetime_;
    float maxLifetime_;
    
    // Random number generation
    std::mt19937 generator_;
    std::uniform_real_distribution<float> spreadDistribution_;
    std::uniform_real_distribution<float> speedDistribution_;
    std::uniform_real_distribution<float> lifetimeDistribution_;
};

} // namespace particlesystem 