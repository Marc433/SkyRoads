#ifndef __SHADER_H__
#define __SHADER_H__ 1

#include <GL/glew.h>
#include "string"

/**
 * @class Shader
 * @brief Represents an individual OpenGL shader.
 *
 * The Shader class encapsulates an OpenGL shader object, which is a program
 * that runs on the GPU. It provides functionality for loading, compiling, and
 * managing different types of shaders (vertex, fragment, geometry).
 */
class Shader {
public:
  /**
   * @enum ShaderType
   * @brief Enumeration of supported shader types.
   */
  enum ShaderType {
    kShaderType_Invalid,   ///< Invalid or unspecified shader type
    kShaderType_Vertex,    ///< Vertex shader - processes vertices
    kShaderType_Fragment,  ///< Fragment shader - processes fragments/pixels
    kShaderType_Geometry,  ///< Geometry shader - processes primitives
  };

  /**
   * @brief Default constructor.
   *
   * Creates a new Shader object with default values.
   */
  Shader();

  /**
   * @brief Destructor.
   */
  ~Shader();

  /**
   * @brief Loads shader source code from memory.
   *
   * @param shader_type The type of shader to create.
   * @param source Pointer to the shader source code.
   * @param source_size Size of the shader source code in bytes.
   * @return true if the source was loaded successfully, false otherwise.
   */
  bool loadSource(const ShaderType shader_type, const char* source, const unsigned int source_size);

  /**
   * @brief Loads shader source code from a file.
   *
   * @param shader_type The type of shader to create.
   * @param filePath Path to the file containing the shader source code.
   * @return true if the source was loaded successfully, false otherwise.
   */
  bool loadFromFile(const ShaderType shader_type, const std::string& filePath);

  /**
   * @brief Compiles the loaded shader source.
   *
   * @return true if compilation succeeded, false otherwise.
   */
  bool compile();

  /**
   * @brief Checks if the shader has been compiled successfully.
   *
   * @return true if the shader is compiled, false otherwise.
   */
  bool get_isCompiled() const;

  /**
   * @brief Gets the OpenGL shader ID.
   *
   * @return The OpenGL shader ID.
   */
  unsigned int get_id();

private:
  /**
   * @brief The type of this shader.
   */
  ShaderType type;

  /**
   * @brief Flag indicating whether the shader has been compiled.
   */
  bool isCompiled;

  /**
   * @brief The OpenGL shader ID.
   */
  unsigned int id;
};

#endif