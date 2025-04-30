#ifndef __COMPONENT__
#define __COMPONENT__ 1

#include <string>
#include <unordered_map>
#include <functional>
#include "common.hpp"
#include "Input.hpp"
#include "Scripting.hpp"
#include "ModelLoader/Model.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @typedef Entity
 * @brief Type definition for entity identifiers in the ECS system.
 *
 * Entities are represented as unsigned integers.
 */
using Entity = unsigned int;

/**
 * @def M_PI
 * @brief Mathematical constant PI.
 */
#define M_PI 3.141592f

 /**
  * @struct ComponentBase
  * @brief Base structure for all component types.
  *
  * Provides common functionality shared by all components, such as activation state.
  */
struct ComponentBase {
  bool active;  ///< Flag indicating whether the component is active
};

/**
 * @enum LightType
 * @brief Enumeration of supported light types.
 */
enum class LightType {
  Directional,  ///< Directional light - simulates light from far away (e.g., sun)
  Point,        ///< Point light - emits light in all directions from a point
  Spot,         ///< Spot light - emits light in a cone shape
  Ambient       ///< Ambient light - uniform light applied to all objects
};

/**
 * @struct LightComponent
 * @brief Component for light sources in the scene.
 *
 * Stores information about different types of lights, including position, direction,
 * color, intensity, and attenuation properties.
 */
struct LightComponent : ComponentBase
{
  LightType type;        ///< The type of light
  glm::vec3 color;       ///< RGB color of the light
  glm::vec3 position;    ///< Position of the light (for point and spot lights)
  glm::vec3 direction;   ///< Direction the light is pointing (for directional and spot lights)
  float intensity;       ///< Brightness of the light
  float radius;          ///< Radius of effect (for point lights and spotlights)
  float cutoff;          ///< Inner cutoff angle for spotlight (in degrees)
  float outerCutoff;     ///< Outer cutoff angle for spotlight (in degrees)
};

/**
 * @struct TransformComponent
 * @brief Component for spatial transformation.
 *
 * Stores position, rotation, and scale information for entities, as well as
 * the combined transformation matrix.
 */
struct TransformComponent : ComponentBase {
  glm::vec3 position;           ///< Position in 3D space
  glm::vec3 rotation;           ///< Rotation in Euler angles (degrees)
  glm::vec3 scale;              ///< Scale factors for X, Y, and Z axes
  glm::mat4 transform_matrix;   ///< Combined transformation matrix
};

/**
 * @struct RenderComponent
 * @brief Component for renderable models.
 *
 * Links an entity to a 3D model for rendering.
 */
struct RenderComponent : ComponentBase {
  std::shared_ptr<Model> model;  ///< Shared pointer to the 3D model
};

/**
 * @enum ShapeType
 * @brief Enumeration of basic shape types.
 */
enum class ShapeType {
  Square,    ///< 2D square shape
  Triangle,  ///< 2D triangle shape
  Circle,    ///< 2D circle shape
  Cube,      ///< 3D cube shape
  Sphere     ///< 3D sphere shape
};

/**
 * @struct ShapeComponent
 * @brief Component for basic geometric shapes.
 *
 * Defines simple geometric primitives for entities that don't need full 3D models.
 */
struct ShapeComponent : ComponentBase {
  ShapeType type;           ///< The type of shape
  std::vector<Vec3> vertices; ///< The vertices defining the shape
  Vec4 color;                 ///< The color of the shape (RGBA)
};

/**
 * @namespace
 * @brief Anonymous namespace for shape creation helper functions.
 */
namespace {
  /**
   * @brief Creates a square shape component.
   *
   * @param size The size (width/height) of the square.
   * @param color The color of the square (RGBA).
   * @return A ShapeComponent initialized as a square.
   */
  ShapeComponent createSquare(float size, const Vec4& color) {
    return { true, ShapeType::Square, {
        {-size / 2, -size / 2, 0},
        { size / 2, -size / 2, 0},
        { size / 2,  size / 2, 0},
        {-size / 2,  size / 2, 0}
    }, color };
  }

  /**
   * @brief Creates a triangle shape component.
   *
   * @param size The size (base/height) of the triangle.
   * @param color The color of the triangle (RGBA).
   * @return A ShapeComponent initialized as a triangle.
   */
  ShapeComponent createTriangle(float size, const Vec4& color) {
    return { true, ShapeType::Triangle, {
        {0, size / 2, 0},
        {-size / 2, -size / 2, 0},
        { size / 2, -size / 2, 0}
    }, color };
  }

  /**
   * @brief Creates a circle shape component.
   *
   * @param radius The radius of the circle.
   * @param color The color of the circle (RGBA).
   * @param segments The number of line segments to approximate the circle.
   * @return A ShapeComponent initialized as a circle.
   */
  ShapeComponent createCircle(float radius, const Vec4& color, int segments = 32) {
    std::vector<Vec3> vertices;
    for (int i = 0; i < segments; ++i) {
      float angle = 2.0f * M_PI * i / segments;
      vertices.push_back({ radius * cos(angle), radius * sin(angle), 0 });
    }
    return { true, ShapeType::Circle, vertices, color };
  }
}

/**
 * @struct CameraComponent
 * @brief Component for camera functionality.
 *
 * Defines a camera in the scene, including position, orientation, and projection parameters.
 */
struct CameraComponent : ComponentBase {
  glm::vec3 position{ 0.0f, 0.0f, 5.0f };    ///< Position of the camera
  glm::vec3 front{ 0.0f, 0.0f, -1.0f };      ///< Direction the camera is looking
  glm::vec3 up{ 0.0f, 1.0f, 0.0f };          ///< Up vector of the camera
  glm::vec3 right{ 1.0f, 0.0f, 0.0f };       ///< Right vector of the camera

  // Euler angles for camera rotation
  float yaw{ -90.0f };     ///< Horizontal rotation angle (degrees)
  float pitch{ 0.0f };     ///< Vertical rotation angle (degrees)

  // Camera configuration
  float movementSpeed{ 50.0f };       ///< Movement speed (units per second)
  float sprintMultiplier{ 2.0f };     ///< Speed multiplier when sprinting
  float mouseSensitivity{ 0.1f };     ///< Mouse sensitivity for camera rotation

  // Camera matrices
  glm::mat4 view{ 1.0f };             ///< View matrix
  glm::mat4 projection{ 1.0f };       ///< Projection matrix

  // Projection properties
  float fov{ 90.0f };                 ///< Field of view (degrees)
  float aspectRatio{ 4.0f / 3.0f };   ///< Aspect ratio (width/height)
  float nearPlane{ 0.1f };            ///< Near clipping plane distance
  float farPlane{ 1000.0f };          ///< Far clipping plane distance

  /**
   * @brief Updates the camera vectors based on Euler angles.
   *
   * Recalculates the front, right, and up vectors based on the current yaw and pitch.
   */
  void updateCameraVectors() {
    // Calculate the front vector based on Euler angles
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Recalculate the right and up vectors
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
  }

  /**
   * @brief Updates the view matrix based on camera position and orientation.
   */
  void updateViewMatrix() {
    view = glm::lookAt(position, position + front, up);
  }

  /**
   * @brief Updates the projection matrix based on camera properties.
   */
  void updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
  }
};

/**
 * @struct InputComponent
 * @brief Component for entity input processing.
 *
 * Stores input state for entities that respond to user input.
 */
struct InputComponent : ComponentBase {
  bool isMovingForward = false;    ///< Flag for forward movement
  bool isJumping = false;          ///< Flag for jumping
  bool isShooting = false;         ///< Flag for shooting
  int keyReceived;                 ///< Last key code received
  bool followingMouse = false;     ///< Flag for mouse following behavior
  double mouseX = 0.0;             ///< Current mouse X coordinate
  double mouseY = 0.0;             ///< Current mouse Y coordinate
};

/**
 * @struct AnimationComponent
 * @brief Component for simple entity animations.
 *
 * Defines animation parameters for entities, including translation, rotation, and scaling over time.
 */
struct AnimationComponent : ComponentBase {
  glm::vec3 translation;    ///< Translation per frame (x, y, z)
  glm::vec3 rotation;       ///< Rotation per frame (pitch, yaw, roll)
  glm::vec3 scale;          ///< Scaling per frame (x, y, z)
  float duration;           ///< Duration of the animation in seconds
  float elapsedTime;        ///< Time elapsed since animation start
};

/**
 * @struct ScriptComponent
 * @brief Component for entity scripting.
 *
 * Links an entity to a Lua script for custom behavior.
 */
struct ScriptComponent : ComponentBase {
  std::shared_ptr<LuaScript> script;  ///< Shared pointer to the Lua script
};


struct NameComponent : ComponentBase {
    std::string name;
};
#endif // !__COMPONENT__

