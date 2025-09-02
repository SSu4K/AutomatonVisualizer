#ifndef __SIMULATION_H__
#define __SIMULATION_H__
#include "automaton/automaton.hpp"
#include "utils.hpp"

class Simulation {
  Automaton<bool> automaton;
  size_t cell_count;
  void update_buffers();
  void clear_buffers();

 public:
  bool* value_buffer;
  bool* loaded_buffer;
  int* age_buffer;

  Simulation(size_vector size, shared_ptr<Rule<bool>> rule);

  void step();
  void step(size_t time);
  void random_fill(double fill);
  void set_cell_value(size_t x, size_t y, bool value);

  size_vector get_size() const;
  size_t get_cell_count() const;
};
#endif