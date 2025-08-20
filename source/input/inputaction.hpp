#ifndef __INPUT_ACTION_HPP__
#define __INPUT_ACTION_HPP__

template <class T>
class InputAction {
 protected:
  T value;

 public:
  InputAction() = default;
  T get_value() const;
  virtual void update_value() = 0;
};

template <class T>
T InputAction<T>::get_value() const {
  return value;
}

#endif