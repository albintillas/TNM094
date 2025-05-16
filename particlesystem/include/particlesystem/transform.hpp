#pragma once

#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <algorithm>

/**
 * @file transform.hpp
 * @brief Custom math utilities for particle system operations.
 * 
 * This file contains custom implementations of common vector operations
 * to avoid dependencies on experimental GLM features. These functions
 * provide the core mathematical operations needed for particle physics.
 */

namespace particlesystem {

/**
 * @brief Rotates a 2D vector by an angle.
 * 
 * @param v The vector to rotate
 * @param angle The rotation angle in radians
 * @return The rotated vector
 */
inline glm::vec2 rotate(const glm::vec2& v, float angle) {
    float s = std::sin(angle);
    float c = std::cos(angle);
    return glm::vec2(
        v.x * c - v.y * s,
        v.x * s + v.y * c
    );
}

/**
 * @brief Calculates the length (magnitude) of a 2D vector.
 * 
 * @param v The input vector
 * @return The length of the vector
 */
inline float length(const glm::vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

/**
 * @brief Normalizes a 2D vector (makes it unit length).
 * 
 * @param v The vector to normalize
 * @return A unit vector in the same direction as the input
 */
inline glm::vec2 normalize(const glm::vec2& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len < 1e-8f) {
        return glm::vec2(0.0f, 0.0f);
    }
    return glm::vec2(v.x / len, v.y / len);
}

/**
 * @brief Calculates the dot product of two 2D vectors.
 * 
 * @param a The first vector
 * @param b The second vector
 * @return The dot product of the two vectors
 */
inline float dot(const glm::vec2& a, const glm::vec2& b) {
    return a.x * b.x + a.y * b.y;
}

/**
 * @brief Clamps a value between a minimum and maximum.
 * 
 * @tparam T The type of the value to clamp
 * @param value The value to clamp
 * @param min The minimum allowed value
 * @param max The maximum allowed value
 * @return The clamped value
 */
template<typename T>
inline T clamp(T value, T min, T max) {
    return std::max(min, std::min(value, max));
}

} // namespace particlesystem 