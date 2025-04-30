#pragma once

#ifndef __WINDOW_SYSTEM__ 
#define __WINDOW_SYSTEM__ 1

#include <optional>

/**
 * @class WindowSystem
 * @brief A singleton-like system class that manages the GLFW window system lifecycle.
 *
 * This class is responsible for initializing and terminating the GLFW library.
 * It follows the RAII (Resource Acquisition Is Initialization) pattern to ensure
 * proper cleanup of GLFW resources upon destruction.
 *
 * @note This class cannot be copied, only moved, to prevent multiple initializations
 * of the GLFW library. It uses std::optional for the factory method to indicate
 * potential initialization failure.
 */
class WindowSystem {
public:
  /**
   * @brief Factory method for creating a WindowSystem instance.
   *
   * This static method attempts to initialize the GLFW library and creates a
   * WindowSystem instance if successful.
   *
   * @return std::optional<WindowSystem> A valid WindowSystem instance if GLFW
   *         initialization was successful, std::nullopt otherwise.
   */
  static std::optional<WindowSystem> make();

  /**
   * @brief Destructor that terminates the GLFW library if it was initialized.
   *
   * Calls glfwTerminate() if the WindowSystem instance was successfully initialized.
   */
  ~WindowSystem();

  /**
   * @brief Move constructor.
   *
   * Transfers ownership of the GLFW initialization state from the source object to this one.
   *
   * @param other The WindowSystem instance to move from.
   */
  WindowSystem(WindowSystem&& other) noexcept;

  /**
   * @brief Move assignment operator.
   *
   * Transfers ownership of the GLFW initialization state from the source object to this one.
   * If this instance already had an initialized GLFW state, it is properly terminated first.
   *
   * @param other The WindowSystem instance to move from.
   * @return Reference to this WindowSystem instance.
   */
  WindowSystem& operator=(WindowSystem&& other) noexcept;

private:
  /**
   * @brief Default constructor, made private to enforce use of the factory method.
   *
   * This constructor does not initialize GLFW; initialization is handled by the make() method.
   */
  WindowSystem() = default;

  /**
   * @brief Copy constructor, deleted to prevent multiple GLFW instances.
   */
  WindowSystem(const WindowSystem&) = delete;

  /**
   * @brief Copy assignment operator, deleted to prevent multiple GLFW instances.
   */
  WindowSystem& operator=(const WindowSystem&) = delete;

  /**
   * @brief Flag to track whether GLFW has been initialized successfully.
   *
   * Used to determine whether glfwTerminate() should be called in the destructor
   * and move assignment operator.
   */
  bool initialized = false;
};

#endif // !__WINDOW_SYSTEM__ 1