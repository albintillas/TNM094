#include <particlesystem/emitter.h>

namespace particlesystem {

Emitter::Emitter(const glm::vec2& position)
    : position_(position)
    , rate_(1.0f)
    , accumulator_(0.0f) {
}

const glm::vec2& Emitter::getPosition() const {
    return position_;
}

void Emitter::setPosition(const glm::vec2& position) {
    position_ = position;
}

void Emitter::setRate(float rate) {
    rate_ = rate;
}

float Emitter::getRate() const {
    return rate_;
}

} // namespace particlesystem 