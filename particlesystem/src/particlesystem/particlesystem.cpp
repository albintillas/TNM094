#include <particlesystem/particlesystem.h>
#include <algorithm>

namespace particlesystem {

ParticleSystem::ParticleSystem() {
    // Initialize with reasonable default capacity
    particles_.reserve(1000);
    emitters_.reserve(10);
    effects_.reserve(10);
}

void ParticleSystem::update(float dt) {
    // Step 1: Emit new particles from all emitters
    for (auto& emitter : emitters_) {
        emitter->emit(particles_, dt);
    }
    
    // Step 2: Reset forces on all particles
    for (auto& particle : particles_) {
        particle.resetForce();
    }
    
    // Step 3: Apply all effects to all particles
    for (auto& effect : effects_) {
        if (effect->isEnabled()) {
            for (auto& particle : particles_) {
                if (particle.alive) {
                    effect->apply(particle);
                }
            }
        }
    }
    
    // Step 4: Update all particles (move and age them)
    for (auto& particle : particles_) {
        particle.update(dt);
    }
    
    // Step 5: Remove dead particles
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(), 
                      [](const Particle& p) { return !p.alive; }),
        particles_.end());
}

void ParticleSystem::addEmitter(std::shared_ptr<Emitter> emitter) {
    if (emitter) {
        emitters_.push_back(emitter);
    }
}

void ParticleSystem::removeEmitter(std::shared_ptr<Emitter> emitter) {
    // Find and remove the emitter from the container
    auto it = std::find(emitters_.begin(), emitters_.end(), emitter);
    if (it != emitters_.end()) {
        emitters_.erase(it);
    }
}

void ParticleSystem::addEffect(std::shared_ptr<Effect> effect) {
    if (effect) {
        effects_.push_back(effect);
    }
}

void ParticleSystem::removeEffect(std::shared_ptr<Effect> effect) {
    // Find and remove the effect from the container
    auto it = std::find(effects_.begin(), effects_.end(), effect);
    if (it != effects_.end()) {
        effects_.erase(it);
    }
}

const std::vector<Particle>& ParticleSystem::getParticles() const {
    return particles_;
}

void ParticleSystem::setParticles(const std::vector<Particle>& particles) {
    // Replace the current particles with the provided ones
    particles_ = particles;
}

void ParticleSystem::getParticleData(std::vector<glm::vec2>& positions, std::vector<glm::vec4>& colors, std::vector<float>& sizes) const {
    // Clear the output vectors
    positions.clear();
    colors.clear();
    sizes.clear();
    
    // Reserve space for efficiency
    positions.reserve(particles_.size());
    colors.reserve(particles_.size());
    sizes.reserve(particles_.size());
    
    // Add data from all alive particles
    for (const auto& particle : particles_) {
        if (particle.alive) {
            positions.push_back(particle.position);
            
            // Calculate color based on lifetime (fade out as they age)
            // This is just an example - could be customized or moved elsewhere
            float lifeFactor = std::min(1.0f, particle.lifetime / 2.0f);
            colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, lifeFactor));
            
            // Size decreases slightly with age
            sizes.push_back(0.02f + 0.02f * lifeFactor);
        }
    }
}

void ParticleSystem::reserve(size_t capacity) {
    particles_.reserve(capacity);
}

void ParticleSystem::clearParticles() {
    particles_.clear();
}

void ParticleSystem::clearEmitters() {
    emitters_.clear();
}

void ParticleSystem::clearEffects() {
    effects_.clear();
}

} // namespace particlesystem
