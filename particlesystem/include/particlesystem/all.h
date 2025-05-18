#pragma once

/**
 Include all the components of the particle system
 */

// Core components
#include <particlesystem/particle.h>
#include <particlesystem/particlesystem.h>
#include <particlesystem/transform.hpp>

// Emitters - objects that create particles
#include <particlesystem/emitter.h>
#include <particlesystem/uniform_emitter.h>
#include <particlesystem/directional_emitter.h>
#include <particlesystem/explosion_emitter.h>

// Effects - objects that modify particle behavior
#include <particlesystem/effect.h>
#include <particlesystem/gravity_well.h>
#include <particlesystem/wind.h>

// Convenience namespace
namespace ps = particlesystem; 