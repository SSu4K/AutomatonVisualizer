#include "rules.h"

bool GOLRule::get_next_cell_value(const Node<bool>& cell,
                                  std::shared_ptr<Graph<bool>>& graph) const {
  int sum = 0;
  for (size_t neighbour : cell.neighbours) {
    sum += graph->nodes[neighbour].value;
  }
  if (sum < 2 || sum > 3) {
    return false;
  }
  if (sum > 2) {
    return true;
  }
  return cell.value;
}

BSRule::BSRule(std::vector<int> b, std::vector<int> s) {
  map = 0;
  for (auto num : b) {
    if (num >= 0 && num <= 8) {
      map |= (1 << num);
    }
  }
  for (auto num : s) {
    if (num >= 0 && num <= 8) {
      map |= ((1 << 9) << num);
    }
  }
}

BSRule::BSRule(unsigned int number) : map(number) {}

bool BSRule::get_next_cell_value(const Node<bool>& cell,
                                 std::shared_ptr<Graph<bool>>& graph) const {
  int sum = 0;
  for (size_t neighbour : cell.neighbours) {
    sum += graph->nodes[neighbour].value;
  }
  return map & (1 << (9 * cell.value + sum));
}