#include <rendering/window.h>
#include <example/randomsystem.h>
#include <example/particle_demo.h>

#include <fmt/format.h>
#include <imgui.h>

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

/*
 * This application represents the "Client" the uses your particle system
 */
int main() try {
    rendering::Window window("Particle System v0.0.2 pre-release beta", 850, 850);

    // Create our new particle demo system
    example::ParticleDemo particleDemo;
    
    // Keep the old system for comparison
    const size_t numParticles = 1000;
    example::RandomSystem randomSystem{numParticles};
    
    float speed = 1.0f;
    bool running = true;
    bool useNewSystem = true;  // Toggle between old and new system
    bool prevMouseDown = false;  // Track previous mouse state
    
    // Mouse click handling variables
    bool mouseButtonClicked = false;

    while (running) {
        window.beginFrame();

        // Get mouse position and convert to normalized coordinates (-1 to 1)
        auto mousePos = window.mousePosition();
        glm::vec2 normalizedMousePos = glm::vec2(
            (mousePos.x / window.width()) * 2.0f - 1.0f,
            -((mousePos.y / window.height()) * 2.0f - 1.0f)  // Flip Y since window coordinates are top-down
        );
        
        // Manual mouse click detection using ImGui
        bool mouseDown = ImGui::GetIO().MouseDown[0];  // 0 = left mouse button
        
        // Handle mouse click (on press, not hold)
        if (mouseDown && !prevMouseDown && useNewSystem && !ImGui::IsAnyItemHovered()) {
            particleDemo.handleMouseClick(normalizedMousePos);
        }
        prevMouseDown = mouseDown;
        
        if (useNewSystem) {
            // Update our new particle demo
            particleDemo.update(window.time(), window.deltaTime() * speed, normalizedMousePos);
        } else {
            // Update the example system
            randomSystem.update(window.time(), speed);
        }

        // Clear screen with color
        window.clear({0.05f, 0.05f, 0.1f, 1.0f});

        // Draw particles
        if (useNewSystem) {
            // Draw the particles
            window.drawPoints(particleDemo.getPositions(), particleDemo.getSizes(), particleDemo.getColors());
            
            // Draw markers for emitters and effects
            window.drawPoints(particleDemo.getMarkerPositions(), particleDemo.getMarkerSizes(), particleDemo.getMarkerColors());
        } else {
            window.drawPoints(randomSystem.getPosition(), randomSystem.getSize(), randomSystem.getColor());
        }

        // User interface
        {
            window.beginGuiWindow("Particle System Controls");
            
            if (useNewSystem) {
                // UI for the new particle system
                particleDemo.renderUI();
                
                window.separator();
            }
            
            window.text("Simulation Controls");
            window.sliderFloat("Simulation Speed", speed, 0.001f, 10.0f);
            
            if (window.button(useNewSystem ? "Switch to Original System" : "Switch to New Particle System")) {
                useNewSystem = !useNewSystem;
            }
            
            window.separator();
            window.text(fmt::format("FPS: {:.1f}", window.fps()));
            window.text(fmt::format("Mouse: ({:.2f}, {:.2f})", normalizedMousePos.x, normalizedMousePos.y));
            
            if (window.button("Close Application")) {
                running = false;
            }
            window.endGuiWindow();
        }

        window.endFrame();
        running = running && !window.shouldClose();
    }

    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    fmt::print("{}", e.what());
    return EXIT_FAILURE;
}
