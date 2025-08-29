#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

#include <imgui-SFML.h>
#include <imgui.h>

const float IMGUI_WINDOW_FRAMERATE = 60;

typedef enum {
  Reload,
  Close,
} SimulationResult;

SimulationWindow build_simulation_window() {
  size_vector size = {200, 100};
  BSRule bs_gol_rule(6152);
  Simulation simulation(size, std::make_shared<BSRule>(bs_gol_rule), 0.1);

  WindowSettings settings = DEFAULT_WINDOW_SETTINGS;
  return SimulationWindow(std::make_shared<Simulation>(simulation), settings);
}

SimulationResult run(RenderWindow &window, SimulationWindow &sim_window) {
  sf::Clock deltaClock;

  bool reload = false;
  while (window.isOpen() && !reload) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed) {
        return Close;
      }
    }

    sf::Time dt = deltaClock.restart();
    ImGui::SFML::Update(window, dt);

    static int simulationFramerateSlider =
        sim_window.getSettings().simulation_framerate;

    sim_window.renderSimulation();
    sim_window.step(dt.asSeconds());

    window.clear();

    // Main Menu
    ImGui::BeginMainMenuBar();
    ImGui::Text("Automaton Visualiser");
    if(ImGui::Button("Reload")){
      reload = true;
    }
    ImGui::EndMainMenuBar();

    // View
    ImGui::Begin("View", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Image(sim_window.getTexture());
    ImGui::End();

    // Control panel
    ImGui::Begin("Control Panel");
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Text("Simulation running...");

    if (ImGui::SliderInt("Framerate", &simulationFramerateSlider, 0, 20)) {
      sim_window.setSimulationFramerate(simulationFramerateSlider);
    }

    if (sim_window.getIsPaused()) {
      if (ImGui::Button("Run"))
        sim_window.setPaused(false);
    } else {
      if (ImGui::Button("Pause"))
        sim_window.setPaused(true);
    }

    ImGui::EndGroup();

    ImGui::End();

    // Render ImGui on top of the window
    ImGui::SFML::Render(window);

    sf::sleep(sf::seconds(1.f / IMGUI_WINDOW_FRAMERATE -
                          deltaClock.getElapsedTime().asSeconds()));

    window.display();
  }

  return Reload;
}

int main() {
  srand(time(NULL));
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Simulation with ImGui");
  if (!ImGui::SFML::Init(window)) {
    return 0;
  }
  SimulationResult result = Reload;

  while (result == Reload) {
    SimulationWindow sim_window = build_simulation_window();
    result = run(window, sim_window);
  }

  window.close();
  ImGui::SFML::Shutdown();

  return 0;
}
