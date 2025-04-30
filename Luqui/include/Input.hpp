#ifndef __INPUT__
#define __INPUT__ 1

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <vector>
//#include "Window.hpp"    // Incluimos la clase Window

/**
 * @class Input
 * @brief Manages and abstracts user input through GLFW.
 *
 * The Input class serves as a layer between the application and GLFW's input handling,
 * providing a consistent interface for keyboard, mouse, and gamepad input. It also
 * implements an action mapping system that allows binding high-level game actions
 * to specific input keys.
 */
class Input {
public:
  /**
   * @enum Key
   * @brief Enumeration of supported keyboard keys.
   *
   * These constants represent keyboard keys in an engine-specific format,
   * which are mapped to GLFW key codes internally.
   */
  enum Key
  {
    KEY_UNKNOWN = -1,
    // letters
    KEY_A = 1,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    // numbers
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    // Fs
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    // movement
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    // others
    KEY_SPACE,
    KEY_TAB,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_ESCAPE,
  };

  /**
   * @enum Mouse_Key
   * @brief Enumeration of supported mouse buttons.
   */
  enum Mouse_Key {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_WHEEL,
  };

  /**
   * @enum Modifier_Key
   * @brief Enumeration of supported keyboard modifier keys.
   */
  enum Modifier_Key {
    MOD_KEY_SHIFT,
    MOD_KEY_CONTROL,
    MOD_KEY_ALT,
  };

  /**
   * @enum Action
   * @brief Enumeration of high-level game actions that can be mapped to inputs.
   */
  enum Action {
    MOVE_FORWARD,
    JUMP,
    SHOOT,
  };

  /**
   * @brief Constructor that initializes input handling for a GLFW window.
   *
   * @param window Pointer to the GLFW window to monitor for input events.
   */
  Input(GLFWwindow* window);

  /**
   * @brief Checks if a specific key is currently pressed.
   *
   * @param key The key to check, using the Input::Key enumeration.
   * @return true if the key is pressed, false otherwise.
   */
  bool isKeyPressed(int key);

  /**
   * @brief Checks if a specific key is currently released.
   *
   * @param key The key to check, using the Input::Key enumeration.
   * @return true if the key is released, false otherwise.
   */
  bool isKeyReleased(int key);

  /**
   * @brief Checks if a specific mouse button is currently pressed.
   *
   * @param button The mouse button to check, using the Input::Mouse_Key enumeration.
   * @return true if the button is pressed, false otherwise.
   */
  bool isMouseButtonPressed(int button);

  /**
   * @brief Gets the current mouse cursor position.
   *
   * @param xPos Reference to variable that will receive the X position.
   * @param yPos Reference to variable that will receive the Y position.
   */
  void getMousePosition(double& xPos, double& yPos);

  /**
   * @brief Gets the current mouse scroll wheel offset.
   *
   * @return The vertical scroll offset.
   */
  double getMouseScroll();

  /**
   * @brief Checks if a specific keyboard modifier is currently pressed.
   *
   * @param modifier The modifier to check, using the Input::Modifier_Key enumeration.
   * @return true if the modifier is pressed, false otherwise.
   */
  bool isModifierPressed(int modifier);

  /**
   * @brief Checks if a joystick/gamepad is connected.
   *
   * @param joystickID The ID of the joystick to check.
   * @return true if the joystick is connected, false otherwise.
   */
  bool isJoystickConnected(int joystickID);

  /**
   * @brief Gets the state of all axes for a specific joystick.
   *
   * @param joystickID The ID of the joystick to query.
   * @param count Reference to variable that will receive the number of axes.
   * @return Pointer to an array of axis values.
   */
  const float* getJoystickAxes(int joystickID, int& count);

  /**
   * @brief Maps a high-level action to a specific key.
   *
   * @param action The action to map, using the Input::Action enumeration.
   * @param key The key to map to, using the Input::Key enumeration.
   */
  void mapAction(Action action, int key);

  /**
   * @brief Checks if the key mapped to a specific action is currently pressed.
   *
   * @param action The action to check, using the Input::Action enumeration.
   * @return true if the action's key is pressed, false otherwise.
   */
  bool isActionPressed(Action action);

  /**
   * @brief Checks if a key was just pressed this frame (debouncing).
   *
   * This can be used to prevent repeated triggering while a key is held down.
   *
   * @param key The key to check, using the Input::Key enumeration.
   * @return true if the key was just pressed, false otherwise.
   */
  bool wasKeyJustPressed(int key);

  /**
   * @brief Gets the currently pressed key, if any.
   *
   * @return The key code of the currently pressed key, or KEY_UNKNOWN if none.
   */
  int getCurrentlyPressedKey();

  /**
   * @brief Initializes GLFW callbacks for input events.
   *
   * Sets up the necessary GLFW callback functions for keyboard and mouse events.
   */
  void initializeCallbacks();

private:
  /**
   * @brief Pointer to the GLFW window being monitored.
   */
  GLFWwindow* m_window;

  /**
   * @brief Maps engine-specific key codes to GLFW key codes.
   */
  std::unordered_map<int, int> keyMapping;

  /**
   * @brief Maps engine-specific mouse button codes to GLFW mouse button codes.
   */
  std::unordered_map<int, int> mouseButtonMapping;

  /**
   * @brief Maps engine-specific modifier key codes to GLFW modifier key codes.
   */
  std::unordered_map<int, int> modifierMapping;

  /**
   * @brief Maps high-level actions to specific keys.
   */
  std::unordered_map<Action, int> actionMapping;

  /**
   * @brief Current mouse scroll wheel offset.
   */
  double m_scrollOffset;

  /**
   * @brief Tracks whether each key was previously pressed.
   *
   * Used for implementing wasKeyJustPressed().
   */
  std::unordered_map<int, bool> keyWasPressed;

  /**
   * @brief Sets up the initial key mapping from engine keys to GLFW keys.
   */
  void setupKeyMapping();

  /**
   * @brief Sets up the initial action mapping.
   */
  void setupActionMapping();

  /**
   * @brief GLFW callback function for keyboard events.
   *
   * @param window The GLFW window that received the event.
   * @param key The key code.
   * @param scancode The system-specific scancode.
   * @param action The action (press, release, or repeat).
   * @param mods The modifier keys held down.
   */
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

  /**
   * @brief GLFW callback function for mouse scroll events.
   *
   * @param window The GLFW window that received the event.
   * @param xoffset The horizontal scroll offset.
   * @param yoffset The vertical scroll offset.
   */
  static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif