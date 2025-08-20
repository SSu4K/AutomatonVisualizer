#ifndef __SIMULATION_H__
#define __SIMULATION_H__
#include "automaton/automaton.hpp"

class Simulation {
  Automaton<bool> automaton;
  size_t cell_count;
  double fill;
  void random_fill();
  void update_buffers();
  void clear_buffers();

 public:
  bool* value_buffer;
  bool* loaded_buffer;
  int* age_buffer;

  Simulation(size_vector size, shared_ptr<Rule<bool>> rule, double fill);

  void step();
  void step(size_t time);
  size_vector get_size();
  size_t get_cell_count();

  void reset();
};
#endif