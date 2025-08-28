#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

#include <imgui.h>
#include <imgui-SFML.h>

int main() {
  srand(time(NULL));
  size_vector size = {200, 100};
  //BSRule carpet_rule({2, 3, 4, 5}, {});
  BSRule bs_gol_rule(6152);
  Simulation simulation(size, make_shared<BSRule>(bs_gol_rule), 0.1);

  WindowSettings settings = DEFAULT_WINDOW_SETTINGS;
  settings.show_grid = true;
  SimulationWindow window({1000, 1000}, make_shared<Simulation>(simulation), settings);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        break;
      }
    }
    window.step();
  }

  return 0;
}

// int main() {
//   srand(time(NULL));
//   size_vector size = {200, 100};
//   BSRule bs_gol_rule(6152);
//   Simulation simulation(size, make_shared<BSRule>(bs_gol_rule), 0.1);

//   WindowSettings settings = DEFAULT_WINDOW_SETTINGS;
//   settings.show_grid = true;

//   // --- Create SFML window (instead of SimulationWindow at first) ---
//   sf::RenderWindow window(sf::VideoMode(1000, 1000), "Simulation with ImGui");

//   // --- Initialize ImGui-SFML ---
//   ImGui::SFML::Init(window);

//   SimulationWindow sim_window({1000, 1000}, make_shared<Simulation>(simulation), settings);

//   sf::Clock deltaClock; // NEW: for ImGui timing

//   while (window.isOpen()) {
//     sf::Event event;
//     while (window.pollEvent(event)) {
//       ImGui::SFML::ProcessEvent(event);   // NEW: pass events to ImGui
//       if (event.type == sf::Event::Closed) {
//         window.close();
//         break;
//       }
//     }

//     ImGui::SFML::Update(window, deltaClock.restart());  // NEW: update ImGui

//     // --- Your simulation logic ---
//     sim_window.step();

//     // --- Draw ImGui UI ---
//     ImGui::Begin("Control Panel");
//     ImGui::Text("Simulation running...");
//     static float speed = 0.1f;
//     if (ImGui::SliderFloat("Speed", &speed, 0.01f, 1.0f)) {
//         // hook into your Simulation class if you want
//         // simulation.setSpeed(speed);
//     }
//     ImGui::End();

//     // --- Render everything ---
//     window.clear();
//     sim_window.draw(window);   // You might need to implement draw(sf::RenderWindow&) if not yet
//     ImGui::SFML::Render(window);
//     window.display();
//   }

//   // --- Cleanup ---
//   ImGui::SFML::Shutdown();

//   return 0;
// }

