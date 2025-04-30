#ifndef __MESH__
#define __MESH__ 1

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <optional>
#include <vector>
#include <string>
#include <assimp/material.h>
#include "../Program.hpp"

/**
 * @class Mesh
 * @brief Represents a 3D mesh with vertices, indices, and material properties.
 *
 * The Mesh class encapsulates a 3D geometric object with its vertex data,
 * indices, and textures. It handles the creation and management of OpenGL
 * buffer objects (VBO, VAO, EBO) and provides functionality for rendering
 * the mesh with a shader program.
 */
class Mesh {
public:
  /**
   * @struct Texture
   * @brief Structure representing a texture loaded from file.
   *
   * Contains information about a texture, including its OpenGL ID,
   * type (diffuse, specular, etc.), file path, and Assimp texture type.
   */
  struct Texture {
    unsigned int id;         ///< OpenGL texture ID
    std::string type;        ///< Texture type (e.g., "texture_diffuse", "texture_specular")
    std::string path;        ///< File path of the texture
    aiTextureType assimpType; ///< Assimp texture type enumeration
  };

  /**
   * @struct Vertex
   * @brief Structure representing a vertex with position, normal, and texture coordinates.
   *
   * Contains the essential data for a single vertex in a 3D mesh.
   */
  struct Vertex {
    glm::vec3 position;   ///< 3D position of the vertex
    glm::vec3 normal;     ///< Normal vector of the vertex
    glm::vec2 texCoords;  ///< Texture coordinates of the vertex
  };

  /**
   * @brief Constructor that initializes a mesh with vertex data, indices, and textures.
   *
   * Creates OpenGL buffer objects (VAO, VBO, EBO) and sets up the vertex attributes
   * based on the provided data.
   *
   * @param vertices Vector of vertex data for the mesh.
   * @param indices Vector of indices defining the mesh triangles.
   * @param textures Vector of texture information for the mesh materials.
   */
  Mesh(std::vector<Vertex>vertices, std::vector<unsigned int>indices, std::vector<Texture> textures);

  /**
   * @brief Destructor that cleans up OpenGL resources.
   *
   * Deletes the VAO, VBO, and EBO if the destroy flag is set.
   */
  ~Mesh();

  /**
   * @brief Deleted copy constructor to prevent accidental copying.
   */
  Mesh(const Mesh&) = delete;

  /**
   * @brief Move constructor that transfers ownership of OpenGL resources.
   *
   * @param other The mesh to move from.
   */
  Mesh(Mesh&& other) noexcept;

  /**
   * @brief Move assignment operator that transfers ownership of OpenGL resources.
   *
   * @param other The mesh to move from.
   * @return Reference to this mesh after assignment.
   */
  Mesh& operator=(Mesh&& other) noexcept;

  /**
   * @brief Renders the mesh using the specified shader program.
   *
   * Binds the appropriate textures, sets uniform variables in the shader,
   * and issues the draw call to render the mesh.
   *
   * @param program The shader program to use for rendering.
   */
  void Draw(Program& program) const;

  /**
   * @brief Vector of texture information for the mesh materials.
   */
  std::vector<Texture> textures_;

  /**
   * @brief OpenGL Vertex Array Object ID.
   */
  GLuint VAO;

  /**
   * @brief OpenGL Vertex Buffer Object ID.
   */
  GLuint VBO;

  /**
   * @brief OpenGL Element Buffer Object ID.
   */
  GLuint EBO;

  /**
   * @brief Gets the OpenGL Vertex Array Object ID.
   *
   * @return The VAO ID.
   */
  GLuint getVAO();

  /**
   * @brief Gets the OpenGL Vertex Buffer Object ID.
   *
   * @return The VBO ID.
   */
  GLuint getVBO();

  /**
   * @brief Gets the OpenGL Element Buffer Object ID.
   *
   * @return The EBO ID.
   */
  GLuint getEBO();

  /**
   * @brief Vector of vertex data for the mesh.
   */
  std::vector<Vertex> vertices_;

  /**
   * @brief Vector of indices defining the mesh triangles.
   */
  std::vector<unsigned int> indices_;

  /**
   * @brief Gets the vector of vertex data for the mesh.
   *
   * @return A copy of the vertices vector.
   */
  std::vector<Vertex> getVertices();

  /**
   * @brief Size of the indices vector.
   *
   * Cached for performance during drawing.
   */
  unsigned int indices_size_;

private:
  /**
   * @brief Flag indicating whether the OpenGL resources should be destroyed.
   *
   * This flag is used to prevent double deletion when ownership is transferred
   * via move operations.
   */
  bool destroy;
};

#endif