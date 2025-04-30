#ifndef __COMMON__
#define __COMMON__ 

#include <GL/glew.h>
#include <iostream>

/**
 * @def OPENGL_CHECK
 * @brief Macro for OpenGL error checking.
 *
 * This macro prints the current OpenGL error state to stdout, including the file and line
 * where the check was performed. It's a debugging tool to help identify OpenGL errors.
 */
#define OPENGL_CHECK do{std::cout << "OpenGL " << __FILE__ << __LINE__ << " "<< glErrorString(glGetError()) << std::endl;} while(false)

 /**
  * @brief Converts an OpenGL error code to a human-readable string.
  *
  * @param err The OpenGL error code returned by glGetError().
  * @return A constant string representing the error code.
  */
static const char* glErrorString(GLenum err) {
  switch (err) {
  case GL_NO_ERROR:
    return "GL_NO_ERROR";
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_STACK_OVERFLOW:
    return "GL_STACK_OVERFLOW";
  case GL_STACK_UNDERFLOW:
    return "GL_STACK_UNDERFLOW";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  default:
    return "Unknown error";
  }
}

/**
 * @brief Width of the main application window.
 */
const int LUQUI_Window_Width = 1280;

/**
 * @brief Height of the main application window.
 */
const int LUQUI_Window_Height = 1040;

/**
 * @struct Vec2
 * @brief A 2D vector structure with basic vector operations.
 */
struct Vec2 {
  float x, y;  ///< X and Y components of the vector

  /**
   * @brief Vector addition operator.
   *
   * @param other The vector to add to this one.
   * @return A new Vec2 containing the sum.
   */
  Vec2 operator+(const Vec2& other) const {
    return { x + other.x, y + other.y };
  }

  /**
   * @brief Scalar multiplication operator.
   *
   * @param scalar The scalar value to multiply the vector by.
   * @return A new Vec2 containing the scaled vector.
   */
  Vec2 operator*(float scalar) const {
    return { x * scalar, y * scalar };
  }

  /**
   * @brief Compound addition operator.
   *
   * @param other The vector to add to this one.
   * @return Reference to this vector after addition.
   */
  Vec2& operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Compound subtraction operator.
   *
   * @param other The vector to subtract from this one.
   * @return Reference to this vector after subtraction.
   */
  Vec2& operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
};

/**
 * @struct Vec3
 * @brief A 3D vector structure with basic vector operations.
 */
struct Vec3 {
  float x, y, z;  ///< X, Y, and Z components of the vector

  /**
   * @brief Vector addition operator.
   *
   * @param other The vector to add to this one.
   * @return A new Vec3 containing the sum.
   */
  Vec3 operator+(const Vec3& other) const {
    return { x + other.x, y + other.y, z + other.z };
  }

  /**
   * @brief Scalar multiplication operator.
   *
   * @param scalar The scalar value to multiply the vector by.
   * @return A new Vec3 containing the scaled vector.
   */
  Vec3 operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
  }

  /**
   * @brief Compound addition operator.
   *
   * @param other The vector to add to this one.
   * @return Reference to this vector after addition.
   */
  Vec3& operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  /**
   * @brief Compound subtraction operator.
   *
   * @param other The vector to subtract from this one.
   * @return Reference to this vector after subtraction.
   */
  Vec3& operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
};

/**
 * @struct Vec4
 * @brief A 4D vector structure with basic vector operations.
 */
struct Vec4 {
  float x, y, z, w;  ///< X, Y, Z, and W components of the vector

  /**
   * @brief Vector addition operator.
   *
   * @param other The vector to add to this one.
   * @return A new Vec4 containing the sum.
   */
  Vec4 operator+(const Vec4& other) const {
    return { x + other.x, y + other.y, z + other.z, w + other.w };
  }

  /**
   * @brief Scalar multiplication operator.
   *
   * @param scalar The scalar value to multiply the vector by.
   * @return A new Vec4 containing the scaled vector.
   */
  Vec4 operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar , w * scalar };
  }

  /**
   * @brief Compound addition operator.
   *
   * @param other The vector to add to this one.
   * @return Reference to this vector after addition.
   */
  Vec4& operator+=(const Vec4& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }
};
#endif // !__COMMON__