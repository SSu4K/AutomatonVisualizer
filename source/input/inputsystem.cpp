#include "inputsystem.h"

KeyPressAction::KeyPressAction(sf::Keyboard::Key key) : key(key) {
  value = false;
  wasPressed = false;
  wasReleased = false;
}

void KeyPressAction::update_value() {
  if (sf::Keyboard::isKeyPressed(key)) {
    if (value == false) {
      value = true;
      wasPressed = true;
    } else {
      wasPressed = false;
    }
  } else {
    if (value == true) {
      value = false;
      wasReleased = true;
    } else {
      wasReleased = false;
    }
  }
}

bool KeyPressAction::isPressed() {
  return value;
}

bool KeyPressAction::wasPressedThisFrame() {
  return wasPressed;
}

bool KeyPressAction::wasReleasedThisFrame() {
  return wasReleased;
}

void ArrowsAction::update_value() {
  sf::Vector2f result = {0, 0};
  if (sf::Keyboard::isKeyPressed(UP_KEY)) {
    result += {0, -1};
  }
  if (sf::Keyboard::isKeyPressed(DOWN_KEY)) {
    result += {0, 1};
  }
  if (sf::Keyboard::isKeyPressed(RIGHT_KEY)) {
    result += {1, 0};
  }
  if (sf::Keyboard::isKeyPressed(LEFT_KEY)) {
    result += {-1, 0};
  }

  if (result.x == 0 && result.y == 0) {
    value = result;
    return;
  }

  float len = hypot(result.x, result.y);
  value = {result.x / len, result.y / len};
}

InputSystem::InputSystem()
    : zoomInAction(ZOOM_IN_KEY),
      zoomOutAction(ZOOM_OUT_KEY),
      pauseAction(PAUSE_KEY),
      resetAction(RESET_KEY) {}

void InputSystem::update() {
  zoomInAction.update_value();
  zoomOutAction.update_value();
  pauseAction.update_value();
  resetAction.update_value();
  arrowsAction.update_value();
}