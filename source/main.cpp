#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

#include <imgui.h>
#include <imgui-SFML.h>

const float WINDOW_FRAMERATE = 60;
//const float SIMULATION_FRAMERATE = 5;

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
    if(!ImGui::SFML::Init(window)){
        return 0;
    }

    const Vector2i sim_window_size(800, 800); 

    // --- Create SimulationWindow (renders to off-screen texture) ---
    SimulationWindow sim_window(sim_window_size, std::make_shared<Simulation>(simulation), settings);

    sf::Clock deltaClock;
    sf::Clock simulationDeltaClock;

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
        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        static int simulationFramerate = 10;
        // --- Step simulation ---
        if(simulationDeltaClock.getElapsedTime().asSeconds() >= 1.0 / simulationFramerate){
            sim_window.stepSimulation();
            simulationDeltaClock.restart();
        }
        sim_window.stepWindow(dt.asSeconds());
        sim_window.interpolate(simulationDeltaClock.getElapsedTime().asSeconds() * simulationFramerate);

        // --- Render simulation to its off-screen texture ---
        sim_window.renderSimulation();

        // --- Draw everything ---
        window.clear();

        // Draw the simulation texture inside ImGui
        ImGui::Begin("Simulation View", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Image(sim_window.getTexture(), Vector2f(sim_window_size));  // display the RenderTexture

        // Control panel
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Text("Simulation running...");
    
        if (ImGui::SliderInt("Framerate", &simulationFramerate, 0, 20)) {
            sim_window.setSimulationFramerate(simulationFramerate);
        }

        if(sim_window.isPaused()){
            if(ImGui::Button("Run"))
                sim_window.setPaused(false);
        }
        else{
            if(ImGui::Button("Pause"))
                sim_window.setPaused(true);
        }

        ImGui::EndGroup();

        ImGui::End();

        // Render ImGui on top of the window
        ImGui::SFML::Render(window);

        sf::sleep(sf::seconds(1.f / WINDOW_FRAMERATE - deltaClock.getElapsedTime().asSeconds()));

        window.display();
    }

    // Cleanup
    ImGui::SFML::Shutdown();

    return 0;
}

