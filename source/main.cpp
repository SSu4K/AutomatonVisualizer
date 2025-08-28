#include <SFML/Graphics.hpp>
#include <iostream>

#include "simulation/rules.h"
#include "simulation/simulation.h"
#include "window.h"

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
