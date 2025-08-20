#ifndef __INPUT_SYSTEM_H__
#define __INPUT_SYSTEM_H__

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include "inputaction.hpp"

#define ZOOM_IN_KEY sf::Keyboard::Equal
#define ZOOM_OUT_KEY sf::Keyboard::Hyphen
#define PAUSE_KEY sf::Keyboard::Space
#define RESET_KEY sf::Keyboard::R

#define UP_KEY sf::Keyboard::Up
#define DOWN_KEY sf::Keyboard::Down
#define RIGHT_KEY sf::Keyboard::Right
#define LEFT_KEY sf::Keyboard::Left

class KeyPressAction : public InputAction<bool> {
 protected:
  const sf::Keyboard::Key key;
  bool wasPressed;
  bool wasReleased;

 public:
  KeyPressAction(sf::Keyboard::Key key);
  void update_value() override;

  bool wasPressedThisFrame();
  bool wasReleasedThisFrame();
  bool isPressed();
};

class ArrowsAction : public InputAction<sf::Vector2f> {
 public:
  void update_value() override;
};

class InputSystem {
 public:
  KeyPressAction zoomInAction;
  KeyPressAction zoomOutAction;
  KeyPressAction pauseAction;
  KeyPressAction resetAction;
  ArrowsAction arrowsAction;

  InputSystem();
  void update();
};

#endif