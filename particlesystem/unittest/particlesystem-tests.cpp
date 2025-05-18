#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <particlesystem/all.h>
#include <memory>
#include <glm/geometric.hpp>

using namespace Catch::Matchers;

TEST_CASE("Particle System Initialization", "[particlesystem]") {
    ps::ParticleSystem system;
    REQUIRE(system.getParticles().empty());
}

TEST_CASE("Particle Basic Update", "[particle]") {
    ps::Particle particle;
    particle.position = glm::vec2(0.0f, 0.0f);
    particle.velocity = glm::vec2(1.0f, 2.0f);
    particle.force = glm::vec2(0.0f, -9.8f);
    particle.lifetime = 1.0f;
    particle.alive = true;
    
    float dt = 0.1f;
    particle.update(dt);
    
    // Position should update based on velocity
    REQUIRE_THAT(particle.position.x, WithinAbs(0.1f, 0.0001f));
    REQUIRE_THAT(particle.position.y, WithinAbs(0.2f, 0.0001f));
    
    // Velocity should update based on force
    REQUIRE_THAT(particle.velocity.x, WithinAbs(1.0f, 0.0001f));
    REQUIRE_THAT(particle.velocity.y, WithinAbs(1.02f, 0.0001f));  // 2.0 - 9.8 * 0.1
    
    // Lifetime should decrease
    REQUIRE_THAT(particle.lifetime, WithinAbs(0.9f, 0.0001f));
    
    // Particle should still be alive
    REQUIRE(particle.alive);
    
    // Update again to see if lifetime works
    particle.update(dt);
    REQUIRE_THAT(particle.lifetime, WithinAbs(0.8f, 0.0001f));
    REQUIRE(particle.alive);
    
    // Update until lifetime is over
    for (int i = 0; i < 8; ++i) {
        particle.update(dt);
    }
    
    // Particle should now be dead
    REQUIRE(particle.lifetime <= 0.0f);
    REQUIRE_FALSE(particle.alive);
}

TEST_CASE("Particle System with Emitter", "[particlesystem]") {
    ps::ParticleSystem system;
    
    // Add a uniform emitter
    auto emitter = std::make_shared<ps::UniformEmitter>(glm::vec2(0.0f, 0.0f));
    emitter->setRate(100.0f);  // Fast emission rate for testing
    system.addEmitter(emitter);
    
    // Update system to emit particles
    system.update(0.1f);
    
    // There should be particles now
    REQUIRE_FALSE(system.getParticles().empty());
    
    // All particles should be alive
    for (const auto& particle : system.getParticles()) {
        REQUIRE(particle.alive);
    }
}

TEST_CASE("Particle System with Effect", "[particlesystem]") {
    ps::ParticleSystem system;
    
    // Add a uniform emitter
    auto emitter = std::make_shared<ps::UniformEmitter>(glm::vec2(0.0f, 0.0f));
    emitter->setRate(10.0f);
    system.addEmitter(emitter);
    
    // Add a gravity well effect
    auto gravity = std::make_shared<ps::GravityWell>(glm::vec2(1.0f, 1.0f));
    gravity->setStrength(5.0f);
    system.addEffect(gravity);
    
    // Update system to emit particles and apply effect
    system.update(0.1f);
    system.update(0.1f);
    
    // Check that particles have non-zero forces
    bool hasForce = false;
    for (const auto& particle : system.getParticles()) {
        if (glm::length(particle.force) > 0.0f) {
            hasForce = true;
            break;
        }
    }
    
    // Reset forces are called at beginning of update, so we need to check after forces are applied
    // but before update is called again
    REQUIRE(hasForce);
}

/* Unit tests using the catch2 framework
 * Homepage: https://github.com/catchorg/Catch2
 * Tutorial: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
 * Docs: https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md
 */

