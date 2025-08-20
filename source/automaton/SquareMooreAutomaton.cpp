#include "SquareMooreAutomaton.h"
#include <vector>
#include <assert.h>

size_t wrap_around(const long &value, const long &max) {
  size_t output = value % max;
  if (value < 0) {
    output += max;
  }
  return output;
}

size_t get_index(const vector<long> &pos, const size_vector &size) {
  assert(pos.size() == size.size());
  size_t index = 0;
  size_t mlt = 1;
  for (size_t i = 0; i < pos.size(); i++) {
    index += wrap_around(pos[i], size[i]) * mlt;
    mlt *= size[i];
  }
  return index;
}

size_vector get_pos(size_t index, const size_vector &size) {
  size_vector pos(size.size(), 0);
  size_t cell_count =
      accumulate(size.begin(), size.end(), 1, multiplies<size_t>());
  for (size_t i = size.size() - 1; i > 0; i--) {
    cell_count /= size[i];
    pos[i] = index / cell_count;
    index %= cell_count;
  }
  pos[0] = index;
  return pos;
}

vector<size_t> get_square_grid_nhood(const size_t &index,
                                     const size_vector &size) {
  assert(size.size() == 2);
  vector<size_t> indicies;
  size_vector pos = get_pos(index, size);
  const long x = pos[0];
  const long y = pos[1];
  for (int8_t yo = -1; yo < 2; yo++) {
    for (int8_t xo = -1; xo < 2; xo++) {
      if (yo == 0 && xo == 0) {
        continue;
      }
      size_t index = get_index({x + xo, y + yo}, size);
      indicies.push_back(index);
    }
  }
  return indicies;
}