#ifndef __SYSTEM__
#define __SYSTEM__ 1

#include <GL/glew.h>
#include "Component.hpp"

/**
 * @class RenderSystem
 * @brief System for rendering entities with render components.
 *
 * The RenderSystem is responsible for rendering entities that have
 * transform and render components. It handles various rendering tasks
 * such as drawing shapes and models, and manages the OpenGL rendering
 * pipeline for the application.
 */
class RenderSystem {
public:
  /**
   * @brief Constructor that initializes the rendering system.
   *
   * Sets up OpenGL rendering states and configures projection matrices.
   * Enables depth testing and sets the background clear color.
   */
  RenderSystem();

  /**
   * @brief Updates the rendering for all entities with transform and render components.
   *
   * Iterates through all entities with transform and render components and
   * renders them according to their properties.
   *
   * @param transforms Vector of transform components for entities.
   * @param renders Vector of render components for entities.
   */
  void update(const std::vector<TransformComponent>& transforms, const std::vector<RenderComponent>& renders);

  /**
   * @brief Renders a geometric shape.
   *
   * Draws a primitive shape using the OpenGL fixed-function pipeline,
   * applying the transform component's position, rotation, and scale.
   *
   * @param transform Pointer to the transform component defining position, rotation, and scale.
   * @param shape Pointer to the shape component defining the geometry and color.
   */
  void drawShape(const TransformComponent* transform, const ShapeComponent* shape);

  /**
   * @brief Renders a 3D model.
   *
   * Draws a 3D model using the provided shader program, applying the
   * transform component's position, rotation, and scale.
   *
   * @param transform Pointer to the transform component defining position, rotation, and scale.
   * @param model Pointer to the render component containing the model to render.
   * @param program The shader program to use for rendering.
   */
  void drawModel(const TransformComponent* transform, const RenderComponent* model, Program& program);

  /**
   * @brief Updates a transform component's matrix based on its position, rotation, and scale.
   *
   * Recalculates the transformation matrix from the component's position, rotation,
   * and scale values, ensuring the matrix is up-to-date before rendering.
   *
   * @param transform Reference to the transform component to update.
   */
  static void UpdateTransformMatrix(TransformComponent& transform);

private:
  /**
   * @brief Renders a specific entity using its transform and render components.
   *
   * Internal method used by update() to handle the rendering of an individual entity.
   *
   * @param entity The ID of the entity to render.
   * @param transform The transform component containing position, rotation, and scale.
   * @param render The render component containing the model to render.
   */
  void renderEntity(Entity entity, const TransformComponent& transform, const RenderComponent& render);
};

/**
 * @class AnimationSystem
 * @brief System for updating entity animations.
 *
 * The AnimationSystem handles updating entity transform components based on
 * animation parameters defined in animation components.
 */
class AnimationSystem {
public:
  /**
   * @brief Updates an entity's transform based on its animation component.
   *
   * Applies translation, rotation, and scaling animation defined in the animation
   * component to the entity's transform component, accounting for elapsed time.
   *
   * @param transform Pointer to the transform component to update.
   * @param anim Pointer to the animation component containing animation parameters.
   * @param deltaTime Time elapsed since the last update, in seconds.
   */
  void update(TransformComponent* transform, AnimationComponent* anim, float deltaTime);
};

/**
 * @class InputSystem
 * @brief System for handling entity input.
 *
 * The InputSystem processes user input and applies it to entities with
 * input components, updating their state accordingly.
 */
class InputSystem {
public:
  /**
   * @brief Updates a camera component based on user input.
   *
   * Processes keyboard and mouse input to control camera position and orientation.
   * This method handles camera movement (WASD), rotation (mouse), and other camera controls.
   *
   * @param input Pointer to the input component containing input state.
   * @param camera Pointer to the camera component to update.
   * @param inputManager Reference to the input manager for querying input state.
   * @param deltaTime Time elapsed since the last update, in seconds.
   */
  void update(InputComponent* input, CameraComponent* camera, Input& inputManager, float deltaTime);
};

#endif // !__SYSTEMS__