#ifndef __AUTOMATON_HPP__
#define __AUTOMATON_HPP__

#include <functional>
#include <iterator>
#include <numeric>
#include <set>
#include <vector>
#include <memory>
#include "graph.hpp"
#include <iostream>

#define UNLOAD_AGE 2

using namespace std;

template <class T>
class Rule{
public:
    virtual T get_next_cell_value(const Node<T> &cell, std::shared_ptr<Graph<T>> &graph) const;
};

template <class T> class Automaton {
  typedef Node<T> Cell;
  typedef std::shared_ptr<Rule<T>> RuleSp;
  typedef std::shared_ptr<Graph<T>> GraphSp;

protected:
  size_vector size;
  size_t cell_count;
  set<size_t> loaded_cells;
  size_t generation;
  
  std::shared_ptr<Rule<T>> rule_sptr;
  std::shared_ptr<Graph<T>> graph;

  void load_cell(const size_t);
  bool cell_unload_condition(const size_t) const;

  T get_cell_value(const Cell *) const;
  bool is_loaded(Cell *) const;

public:
  Automaton() = default;
  Automaton(std::shared_ptr<Graph<T>> graph, std::shared_ptr<Rule<T>> rule);

  size_vector get_size() const;
  size_t get_cell_count() const;
  size_t get_loaded_cell_count() const;
  size_t get_generation() const;

  T get_cell_value(const size_t) const;
  void set_cell_value(const size_t, const T, const bool supress_load = false);
  bool is_loaded(const size_t) const;
  size_t get_cell_age(const size_t) const;
  void reset_cell(const size_t);
  
  void step();
};

template <class T>
T Rule<T>::get_next_cell_value(const Node<T> &cell, std::shared_ptr<Graph<T>> &graph) const{
  return cell.value;
};

template <class T>
Automaton<T>::Automaton(GraphSp graph,
                        RuleSp rule)
    : size(graph->get_size()),
      loaded_cells({}), generation(0)
      {
  this->graph = graph;
  this->rule_sptr = rule;
  cell_count = graph->nodes.size();
}

template <class T>
void Automaton<T>::load_cell(size_t index) {
  if (!is_loaded(index)) {
    loaded_cells.insert(index);
  }
}

template <class T>
bool Automaton<T>::cell_unload_condition(const size_t index) const {
  if ((generation - graph->nodes[index].generation) < UNLOAD_AGE) {
    return false;
  }
  for (size_t neighbour : graph->nodes[index].neighbours) {
    if ((generation - graph->nodes[neighbour].generation) < UNLOAD_AGE) {
      return false;
    }
  }
  return true;
}

template <class T> T Automaton<T>::get_cell_value(const Cell *cell_ptr) const {
  return cell_ptr->value;
}

template <class T> size_vector Automaton<T>::get_size() const { 
  return size;
}
template <class T> size_t Automaton<T>::get_cell_count() const {
  return graph->nodes.size();
}
template <class T> size_t Automaton<T>::get_loaded_cell_count() const {
  return loaded_cells.size();
}
template <class T> size_t Automaton<T>::get_generation() const {
  return generation;
}

template <class T> bool Automaton<T>::is_loaded(const size_t index) const {

  return loaded_cells.find(index) != loaded_cells.end();
}

template <class T>
void Automaton<T>::set_cell_value(const size_t index, const T value, const bool supress_load) {
  if(index >= graph->nodes.size()){
    throw out_of_range("Setting value for cell out of range!");
  }
  if(graph->nodes[index].value == value){
    return;
  }
  graph->nodes[index].value = value;
  graph->nodes[index].generation = generation;
  if(!supress_load){
    load_cell(index);
    for(auto i : graph->nodes[index].neighbours){
      load_cell(i);
    }
  }
}

template <class T> T Automaton<T>::get_cell_value(const size_t index) const {
  if(index >= graph->nodes.size()){
    throw out_of_range("Accesing value for cell out of range!");
  }
  return graph->nodes[index].value;
}

template <class T> size_t Automaton<T>::get_cell_age(const size_t index) const{
  if(index >= graph->nodes.size()){
    throw out_of_range("Accesing age for cell out of range!");
  }
  return generation - graph->nodes[index].generation;
}

template <class T> void Automaton<T>::reset_cell(const size_t index){
  if(index >= graph->nodes.size()){
    throw out_of_range("Accesing age for cell out of range!");
  }
  graph->nodes[index].value = T{};
  graph->nodes[index].generation = generation;
}

template <class T> 
void Automaton<T>::step() {
  vector<size_t> update_cells;
  vector<T> update_values;
  auto it = loaded_cells.begin();
  while (it != loaded_cells.end()) {
    auto current = it++;
    if (cell_unload_condition(*current)) {
      loaded_cells.erase(current);
      continue;
    }
    T new_value = this->rule_sptr->get_next_cell_value(graph->nodes[*current], graph);
    update_cells.push_back(*current);
    update_values.push_back(new_value);
  }

  for(size_t i=0; i<update_cells.size(); i++){
    set_cell_value(update_cells[i], update_values[i]);
  }
  generation++;
}

#endif