#ifndef __WINDOW__
#define __WINDOW__ 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "Figure.hpp"

/**
 * @class Window
 * @brief A wrapper class for a GLFW window with OpenGL context.
 *
 * This class encapsulates a GLFWwindow pointer and provides methods for window
 * management, rendering, and input handling. It follows RAII principles to ensure
 * proper cleanup of GLFW window resources upon destruction.
 *
 * The class supports move semantics but not copy semantics to prevent multiple
 * instances from managing the same window resources.
 */
class Window {
public:
  /**
   * @brief Constructor that wraps an existing GLFWwindow pointer.
   *
   * @param glfwWindow Pointer to an already created GLFWwindow.
   */
  Window(GLFWwindow* glfwWindow);

  /**
   * @brief Destructor that cleans up the GLFWwindow if destroy flag is set.
   */
  ~Window();

  /**
   * @brief Copy constructor, deleted to prevent multiple instances managing the same window.
   */
  Window(const Window& other) = delete;

  /**
   * @brief Move constructor, transfers ownership of the window.
   *
   * @param other The Window instance to move from.
   */
  Window(Window&& other) noexcept;

  /**
   * @brief Copy assignment operator, deleted to prevent multiple instances managing the same window.
   */
  Window& operator=(const Window& other) = delete;

  /**
   * @brief Move assignment operator, transfers ownership of the window.
   *
   * @param other The Window instance to move from.
   * @return Reference to this Window instance.
   */
  Window& operator=(Window& other);

  /**
   * @brief Factory method to create a new window with specified dimensions and title.
   *
   * @param window_width Width of the window in pixels.
   * @param window_height Height of the window in pixels.
   * @param window_name Title of the window.
   * @return std::optional<Window> A valid Window instance if creation was successful, std::nullopt otherwise.
   */
  static std::optional<Window> make(int window_width, int window_height, const char* window_name);

  /**
   * @brief Sets this window as the current OpenGL context.
   *
   * Makes this window's OpenGL context current for the calling thread.
   */
  void setCurrentWindowActive();

  /**
   * @brief Renders the current frame.
   *
   * Swaps the front and back buffers and polls for events.
   */
  void render();

  /**
   * @brief Clears the window's framebuffer.
   *
   * Clears both the color buffer and depth buffer.
   */
  void clear();

  /**
   * @brief Renders a Figure object in the window.
   *
   * @param figure The Figure object to render.
   */
  void renderFigure(Figure& figure);

  /**
   * @brief Handles input for a Figure object.
   *
   * @param figure The Figure object whose position will be updated based on input.
   */
  void handleInput(Figure& figure);

  /**
   * @brief Destroys the window.
   *
   * @return Always returns 0 (seems to be unused).
   */
  int destroyWindow();

  /**
   * @brief Checks if the window should be closed.
   *
   * @return true if the window should remain open, false if it should close.
   * @note The return value is inverted from glfwWindowShouldClose.
   */
  bool isOpen();

  /**
   * @brief Pointer to the underlying GLFWwindow object.
   */
  GLFWwindow* window;

private:
  /**
   * @brief Flag indicating whether the window should be destroyed in the destructor.
   *
   * This flag is used to ensure proper resource cleanup and prevent double-free errors
   * when ownership of the window is transferred via move semantics.
   */
  bool destroy;
};
#endif // !__WINDOW__