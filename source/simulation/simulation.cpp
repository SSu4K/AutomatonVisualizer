#include "simulation.h"
#include "automaton/automaton.hpp"

void Simulation::random_fill(double fill) {
  for (size_t i = 0; i < automaton.get_cell_count(); i++) {
    bool value = double(rand()) / RAND_MAX < fill;
    automaton.reset_cell(i);
    automaton.set_cell_value(i, value);
  }
}

void Simulation::update_buffers() {
  for (size_t i = 0; i < cell_count; i++) {
    value_buffer[i] = automaton.get_cell_value(i);
    loaded_buffer[i] = automaton.is_loaded(i);
    age_buffer[i] = automaton.get_cell_age(i);
  }
}

void Simulation::clear_buffers() {
  for (size_t i = 0; i < cell_count; i++) {
    value_buffer[i] = false;
    loaded_buffer[i] = false;
    age_buffer[i] = 0;
  }
}

Simulation::Simulation(size_vector size,
                       shared_ptr<Rule<bool>> rule){
  MooreLattice2D<bool> lattice(size, true);
  automaton = Automaton<bool>(make_shared<MooreLattice2D<bool>>(lattice), rule);
  cell_count = size[0] * size[1];
  value_buffer = new bool[cell_count];
  loaded_buffer = new bool[cell_count];
  age_buffer = new int[cell_count];
}

void Simulation::step() {
  this->automaton.step();
  update_buffers();
}

void Simulation::step(size_t time) {
  for (size_t i = 0; i < time; i++) {
    this->automaton.step();
  }
  update_buffers();
}

void Simulation::set_cell_value(size_t x, size_t y, bool value){
  int width = automaton.get_size()[0];
  int height = automaton.get_size()[1];
  int i = utils::clampi(x, 0, width-1) + width*utils::clampi(y, 0, height-1);

  automaton.set_cell_value(i, value);
}

size_vector Simulation::get_size() const{
  return automaton.get_size();
}

size_t Simulation::get_cell_count() const{
  return cell_count;
}