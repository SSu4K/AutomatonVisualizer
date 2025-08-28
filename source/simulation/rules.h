#ifndef __RULES_H__
#define __RULES_H__
#include "automaton/automaton.hpp"

class GOLRule : public Rule<bool> {
 public:
  bool get_next_cell_value(const Node<bool>& cell,
                           std::shared_ptr<Graph<bool>>& graph) const override;
};

class BSRule : public Rule<bool> {
  unsigned int map;

 public:
  BSRule(std::vector<int> b, std::vector<int> s);
  BSRule(unsigned int number);

  bool get_next_cell_value(const Node<bool>& cell,
                           std::shared_ptr<Graph<bool>>& graph) const override;
};

#endif