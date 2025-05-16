#include <particlesystem/wind.h>
#include <particlesystem/transform.hpp>
#include <cmath>

namespace particlesystem {

Wind::Wind(const glm::vec2& direction)
    : direction_(normalize(direction))
    , currentDirection_(direction_)
    , varying_(false)
    , hasPosition_(false) {
}

void Wind::setDirection(const glm::vec2& direction) {
    direction_ = normalize(direction);
    if (!varying_) {
        currentDirection_ = direction_;
    }
}

const glm::vec2& Wind::getDirection() const {
    return direction_;
}

void Wind::setVarying(bool varying) {
    varying_ = varying;
    if (!varying_) {
        currentDirection_ = direction_;
    }
}

bool Wind::isVarying() const {
    return varying_;
}

void Wind::setPosition(const glm::vec2& position) {
    position_ = position;
    hasPosition_ = true;
}

const glm::vec2& Wind::getPosition() const {
    return position_;
}

bool Wind::hasPosition() const {
    return hasPosition_;
}

void Wind::apply(Particle& particle) {
    if (!enabled_ || !particle.alive) {
        return;
    }
    
    // Apply the wind force
    particle.force += currentDirection_ * strength_;
}

void Wind::update(float time) {
    if (varying_) {
        // Create some variation in the wind direction using sine waves with different frequencies
        float angle = 0.2f * std::sin(time * 0.5f) + 0.1f * std::sin(time * 1.1f);
        currentDirection_ = rotate(direction_, angle);
    }
}

} // namespace particlesystem 