// Model.hpp
#ifndef __MODEL__ 
#define __MODEL__ 1

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <vector>
#include <string>
#include <future>
#include <memory>
#include "Mesh.hpp"
#include "../Program.hpp"
#include "JobSystem.hpp"

/**
 * @class Model
 * @brief Represents a 3D model loaded from file.
 *
 * The Model class encapsulates a 3D model composed of multiple meshes and textures.
 * It provides functionality for loading models from files (synchronously or asynchronously),
 * processing mesh data, loading textures, and rendering the model.
 */
class Model {
public:

  // Constructors for synchronous and asynchronous loading
  Model(const std::string& path, std::string name_);

  /**
   * @brief Constructor that loads a model synchronously.
   *
   * Loads and processes the model immediately, making it ready for rendering.
   *
   * @param path Path to the model file to load.


  /**
   * @brief Constructor that loads a model asynchronously using a job system.
   *
   * Starts the model loading process asynchronously using the provided job system.
   * The model will not be ready for rendering until the loading process completes.
   *
   * @param path Path to the model file to load.
   * @param jobSystem Reference to the job system to use for asynchronous loading.
   */

  Model(const std::string& path, JobSystem& jobSystem);

  /**
   * @brief Default destructor.
   */
  ~Model() = default;

  /**
   * @brief Deleted copy constructor to prevent accidental copying.
   */
  Model(const Model& other) = delete;

  /**
   * @brief Deleted copy assignment operator to prevent accidental copying.
   */
  Model& operator=(const Model& other) = delete;

  /**
   * @brief Move constructor that transfers ownership of resources.
   *
   * @param other The model to move from.
   */
  Model(Model&& other) noexcept;

  /**
   * @brief Move assignment operator that transfers ownership of resources.
   *
   * @param other The model to move from.
   * @return Reference to this model after assignment.
   */
  Model& operator=(Model&& other) noexcept;

  /**
   * @brief Finalizes model loading.
   *
   * For asynchronously loaded models, this method creates the OpenGL resources
   * once the data loading is complete.
   */
  void finalizeModel();

  /**
   * @brief Renders the model using the specified shader program.
   *
   * Renders all meshes that compose the model.
   *
   * @param program The shader program to use for rendering.
   */
  void Draw(Program& program) const;

  /**
   * @brief Checks if the model loading process is complete.
   *
   * @return true if the model is fully loaded and ready for rendering, false otherwise.
   */
  bool isLoadComplete() const { return loadComplete; }

  /**
   * @brief Checks if the model data has been loaded.
   *
   * @return true if the model data has been loaded (but possibly not yet processed), false otherwise.
   */
  bool isDataLoaded() const { return dataLoaded; }

  /**
   * @brief Checks if the OpenGL resources have been created.
   *
   * @return true if the OpenGL resources have been created, false otherwise.
   */
  bool isGLResourcesCreated() const { return glResourcesCreated; }

  /**
   * @brief Vector of meshes that compose the model.
   */
  std::vector<Mesh> meshes;

  /**
   * @brief Creates OpenGL resources for the loaded model data.
   *
   * Converts the loaded mesh data into OpenGL mesh objects with buffers and textures.
   */
  void createGLResources();
  std::string get_name();
private:
  /**
   * @struct MaterialInfo
   * @brief Structure for storing material information during loading.
   *
   * Contains information about a material texture before it's loaded into OpenGL.
   */
  struct MaterialInfo {
    std::string path;        ///< File path of the texture
    std::string type;        ///< Texture type (e.g., "texture_diffuse", "texture_specular")
    aiTextureType assimpType; ///< Assimp texture type enumeration
  };

  /**
   * @struct MeshLoadData
   * @brief Structure for storing mesh data during loading.
   *
   * Contains the vertex data, indices, and material information for a mesh
   * before it's converted into an OpenGL mesh object.
   */
  struct MeshLoadData {
    std::vector<Mesh::Vertex> vertices;       ///< Vertex data for the mesh
    std::vector<unsigned int> indices;        ///< Indices defining the mesh triangles
    std::vector<MaterialInfo> materialInfo;   ///< Material information for the mesh
    std::string directory;                    ///< Directory containing the model's textures
  };


  std::string name;
  // Member variables for loading state

  /**
   * @brief Vector of mesh data loaded from file but not yet processed.
   */

  std::vector<MeshLoadData> meshesData;

  /**
   * @brief Vector of textures already loaded to avoid duplicate loading.
   */
  std::vector<Mesh::Texture> textures_loaded;

  /**
   * @brief Directory containing the model's textures.
   */
  std::string directory;

  /**
   * @brief Flag indicating whether the model is being loaded asynchronously.
   */
  bool asyncMode = false;

  /**
   * @brief Flag indicating whether the model loading process is complete.
   */
  bool loadComplete = false;

  /**
   * @brief Flag indicating whether the model data has been loaded from file.
   */
  bool dataLoaded = false;

  /**
   * @brief Flag indicating whether the OpenGL resources have been created.
   */
  bool glResourcesCreated = false;

  /**
   * @brief Loads a model synchronously from file.
   *
   * Processes the model file and loads the mesh data and materials.
   *
   * @param path Path to the model file to load.
   */
  void loadModel(const std::string& path);

  /**
   * @brief Loads a model asynchronously from file using a job system.
   *
   * Starts the model loading process asynchronously using the provided job system.
   *
   * @param path Path to the model file to load.
   * @param jobSystem Reference to the job system to use for asynchronous loading.
   */
  void loadModelDataAsync(const std::string& path, JobSystem& jobSystem);

  /**
   * @brief Processes a node in the model hierarchy.
   *
   * Recursively processes the model's node hierarchy, extracting mesh data.
   *
   * @param node Pointer to the node to process.
   * @param scene Pointer to the Assimp scene containing the model data.
   */
  void processNode(aiNode* node, const aiScene* scene);

  /**
   * @brief Processes a mesh from the Assimp scene.
   *
   * Extracts vertex data, indices, and material information from an Assimp mesh.
   *
   * @param mesh Pointer to the Assimp mesh to process.
   * @param scene Pointer to the Assimp scene containing the model data.
   * @return A MeshLoadData structure containing the processed mesh data.
   */
  MeshLoadData processMeshData(aiMesh* mesh, const aiScene* scene);

  /**
   * @brief Loads material textures.
   *
   * Extracts texture information from an Assimp material.
   *
   * @param mat Pointer to the Assimp material to process.
   * @param type The type of texture to extract (diffuse, specular, etc.).
   * @param typeName String name for the texture type.
   * @return A vector of Texture structures containing the loaded texture information.
   */
  std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

  /**
   * @brief Loads a texture from file into OpenGL.
   *
   * Creates an OpenGL texture object from an image file.
   *
   * @param path Path to the texture file to load.
   * @param directory Directory containing the texture file.
   * @return The OpenGL texture ID of the loaded texture.
   */
  unsigned int TextureFromFile(const char* path, const std::string& directory);
};

#endif