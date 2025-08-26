#include <SFML/Graphics.hpp>
#include <iostream>

#include "rules.h"
#include "simulation.h"
#include "window.h"

int main() {
  srand(time(NULL));
  size_vector size = {100, 100};
  // BSRule carpet_rule({2, 3, 4, 5}, {});
  BSRule bs_gol_rule(6152);
  // BSRule bs_gol_rule(6162);
  Simulation simulation(size, make_shared<BSRule>(bs_gol_rule), 0.1);
  SimulationWindow window({1000, 1000}, make_shared<Simulation>(simulation));

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
