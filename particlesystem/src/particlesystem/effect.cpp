#include <particlesystem/effect.h>

namespace particlesystem {

Effect::Effect()
    : strength_(1.0f)
    , enabled_(true) {
}

void Effect::setStrength(float strength) {
    strength_ = strength;
}

float Effect::getStrength() const {
    return strength_;
}

void Effect::setEnabled(bool enabled) {
    enabled_ = enabled;
}

bool Effect::isEnabled() const {
    return enabled_;
}

} // namespace particlesystem 