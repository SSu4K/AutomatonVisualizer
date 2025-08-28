#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

#include <imgui.h>
#include <imgui-SFML.h>

// int main() {
//   srand(time(NULL));
//   size_vector size = {200, 100};
//   //BSRule carpet_rule({2, 3, 4, 5}, {});
//   BSRule bs_gol_rule(6152);
//   Simulation simulation(size, make_shared<BSRule>(bs_gol_rule), 0.1);

//   WindowSettings settings = DEFAULT_WINDOW_SETTINGS;
//   settings.show_grid = true;
//   SimulationWindow window({1000, 1000}, make_shared<Simulation>(simulation), settings);

//   while (window.isOpen()) {
//     sf::Event event;
//     while (window.pollEvent(event)) {
//       if (event.type == sf::Event::Closed) {
//         window.close();
//         break;
//       }
//     }
//     window.step();
//   }

//   return 0;
// }
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "simulation/simulation.h"
#include "simulation/rules.h"
#include "window.h"

int main() {
    srand(time(NULL));
    size_vector size = {200, 100};
    BSRule bs_gol_rule(6152);
    Simulation simulation(size, std::make_shared<BSRule>(bs_gol_rule), 0.1);

    WindowSettings settings = DEFAULT_WINDOW_SETTINGS;
    settings.show_grid = true;

    // --- Create SFML window ---
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Simulation with ImGui");

    // --- Initialize ImGui-SFML ---
    ImGui::SFML::Init(window);

    // --- Create SimulationWindow (renders to off-screen texture) ---
    SimulationWindow sim_window({1000, 1000}, std::make_shared<Simulation>(simulation), settings);

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);   // pass events to ImGui
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
        }

        // Update ImGui state
        ImGui::SFML::Update(window, deltaClock.restart());

        // --- Step simulation ---
        sim_window.step();

        // --- Render simulation to its off-screen texture ---
        sim_window.renderSimulation();

        // --- Draw everything ---
        window.clear();

        // Draw the simulation texture inside ImGui
        ImGui::Begin("Simulation View");
        ImGui::Image(sim_window.getTexture());  // display the RenderTexture
        ImGui::End();

        // Control panel
        ImGui::Begin("Control Panel");
        ImGui::Text("Simulation running...");
        static float speed = 0.1f;
        if (ImGui::SliderFloat("Speed", &speed, 0.01f, 1.0f)) {
            // Optionally pass speed to simulation
            // sim_window.getSimulation()->setSpeed(speed);
        }
        ImGui::End();

        // Render ImGui on top of the window
        ImGui::SFML::Render(window);

        window.display();
    }

    // Cleanup
    ImGui::SFML::Shutdown();

    return 0;
}

