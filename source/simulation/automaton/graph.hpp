#include <cassert>
#include <iostream>
#include <vector>
#define MOORE_NEIGHBOURS_2D_COUNT 8

using namespace std;

static int const MOORE_NEIGHBOURS_2D_OFFSETS[MOORE_NEIGHBOURS_2D_COUNT][2] = {
    {-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}};

typedef vector<size_t> size_vector;

template <class T>
class Node {
 public:
  T value;
  vector<size_t> neighbours;
  size_t generation;

  Node() = default;
  ~Node() = default;
};

template <class T>
class Graph {
 protected:
  size_vector size;

 public:
  vector<Node<T>> nodes;
  size_vector get_size();
  void test_neighbours();
};

template <class T>
class MooreLattice2D : public Graph<T> {
 private:
  void create_neighbours(int x, int y);
  bool clamped;
  int normalize(const int value, const int max) const;

 public:
  MooreLattice2D(size_vector size, bool clamped = false);
  Node<T>& at(int x, int y);
  void fill(T value);
};

static inline int wrap(const int value, const int max) {
  return (value % max + max) % max;
}

static inline int clamp(const int value, const int max) {
  return (value < 0) ? 0 : (value >= max) ? max-1 : value;
}

template <class T>
int MooreLattice2D<T>::normalize(const int value, const int max) const {
  if (clamped) {
    return (value < 0) ? 0 : (value > max) ? max : value;
  } else {
    return (value % max + max) % max;
  }
}

template <class T>
size_vector Graph<T>::get_size() {
  return this->size;
}

template <class T>
void Graph<T>::test_neighbours() {
  Node<T>* base = &(nodes[0]);
  std::cout << "Base: " << base << std::endl;
  for (int i = 0; i < nodes.size(); i++) {
    Node<T>* node = &(nodes[i]);
    std::cout << "Neighbours of node #" << i << " (" << node << "):\n";
    for (int j = 0; j < nodes[i].neighbours.size(); j++) {
      std::cout << "\tnode #" << (nodes[i].neighbours[j]) << ":\n";
    }
  }
}

template <class T>
Node<T>& MooreLattice2D<T>::at(const int x, const int y) {
  int width = this->size[0];
  int height = this->size[1];

  return this->nodes[normalize(x, width) + width * normalize(y, height)];
}

template <class T>
void MooreLattice2D<T>::create_neighbours(int x, int y) {
  Node<T>& node = this->at(x, y);
  auto it = MOORE_NEIGHBOURS_2D_OFFSETS;
  auto end = it + MOORE_NEIGHBOURS_2D_COUNT;
  const size_t width = this->size[0];
  const size_t height = this->size[1];
  while (it < end) {
    int dx = (*it)[0];
    int dy = (*it)[1];
    if(clamped){
      if((x+dx) >= 0 && (x+dx) < width && (y+dy) >= 0 && (y+dy) < height){
        node.neighbours.push_back(x+dx + width*(y+dy));  
      }
    }
    else{
      size_t index = normalize(x + dx, width) + width * normalize(y + dy, height);
      node.neighbours.push_back(index);
    }
    
    it++;
  }
}

template <class T>
MooreLattice2D<T>::MooreLattice2D(size_vector size, bool clamped)
    : clamped(clamped) {
  this->size = size;
  assert(size.size() == 2);
  size_t count = size[0] * size[1];
  for (size_t i = 0; i < count; i++) {
    this->nodes.push_back(Node<T>());
  }

  for (size_t y = 0; y < size[1]; y++) {
    for (size_t x = 0; x < size[0]; x++) {
      create_neighbours(x, y);
    }
  }
}

template <class T>
void MooreLattice2D<T>::fill(T value) {
  for (auto node : this->nodes) {
    node.value = value;
  }
}