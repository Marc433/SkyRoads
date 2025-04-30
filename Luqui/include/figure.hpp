#ifndef __FIGURE__
#define __FIGURE__ 1

#include <GLFW/glfw3.h>
#include <vector>
#include "Input.hpp"

/**
 * @class Figure
 * @brief A class for representing and manipulating 2D geometric figures.
 *
 * The Figure class provides functionality for creating, rendering, and manipulating
 * simple 2D shapes within an OpenGL context. It supports custom vertex definitions
 * and interactive movement through the Input system.
 */
class Figure {
public:
  /**
   * @struct Vec2
   * @brief A 2D vector structure used for positions and vertices.
   */
  struct Vec2 {
    float x, y;  ///< X and Y components of the vector
  }position, offset;  ///< Base position and movement offset of the figure

  /**
   * @brief Default constructor.
   *
   * Creates a simple triangle figure with vertices at (-0.5, -0.5), (0.5, -0.5), and (0, 0.5).
   * The offset is initialized to (0, 0).
   */
  Figure();

  /**
   * @brief Constructor with custom vertices.
   *
   * Creates a figure with the specified set of vertices.
   *
   * @param customVertices A vector of Vec2 points defining the figure's shape.
   */
  Figure(std::vector<Vec2>& customVertices);

  /**
   * @brief Destructor.
   *
   * Cleans up the vertices vector.
   */
  ~Figure();

  /**
   * @brief Sets the base position of the figure.
   *
   * @param position The absolute position to set for the figure.
   */
  void setPosition(Vec2 position);

  /**
   * @brief Sets the offset for the figure.
   *
   * The offset is added to the base position when rendering.
   *
   * @param offset The offset vector to apply.
   */
  void setOffset(Vec2 offset);

  /**
   * @brief Gets the current offset of the figure.
   *
   * @return The current offset vector.
   */
  Vec2 getOffset();

  /**
   * @brief Renders the figure using OpenGL.
   *
   * Draws the figure as a set of triangles using the current position and offset.
   */
  void drawFigure();

  /**
   * @brief Sets the vertices of the figure.
   *
   * Replaces the current set of vertices with a new set.
   *
   * @param customVertices A vector of Vec2 points defining the new shape.
   */
  void setVertices(std::vector<Vec2>& customVertices);

  /**
   * @brief Updates the figure's position based on input.
   *
   * Handles keyboard input to move the figure around.
   *
   * @param input The Input object to check for keyboard state.
   */
  void moveFigure(Input& input);

private:
  /**
   * @brief The set of vertices defining the figure's shape.
   */
  std::vector<Vec2> vertices;

  /**
   * @brief The number of vertices in the figure.
   */
  int num_vertex;
};

#endif