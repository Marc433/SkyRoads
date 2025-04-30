#ifndef __LUQUI_IMGUI__
#define __LUQUI_IMGUI__ 1


#include "window.hpp"
#include "ECS/Component.hpp"
#include "ECS/ECSManager.hpp"

/**
 * @class LuquiImgui
 * @brief Handles the graphical user interface using Dear ImGui.
 */
class LuquiImgui {
public:
	/**
 * @brief Constructor of LuquiImgui.
 * @param window Pointer to the window where the UI will be rendered.
	 */
	LuquiImgui(Window* window);

	/**
   * @brief Destructor of LuquiImgui.
   */
	~LuquiImgui();

	/**
   * @brief Starts a new ImGui frame.
   */
	void NewFrame();


	/**
   * @brief Creates all entity-related windows.
   * @param ecsManager Reference to the ECS manager.
   */
	void EntityWindows(ECSManager& ecsManager);

	/**
  * @brief Entity selection window.
  * @param ecsManager Reference to the ECS manager.
  */
	void EntitySelector(ECSManager& ecsManager);

	/**
 * @brief Entity properties window.
 * @param ecsManager Reference to the ECS manager.
 */
	void EntityProperties(ECSManager& ecsManager);

	/**
	  * @brief Camera properties window.
	  * @param camera Reference to the camera component.
	  */
	void CameraProperties(CameraComponent& camera);

	/**
  * @brief Gets the currently selected entity.
  * @return The selected entity.
  */
	Entity GetSelectedEntity() const;

	/**
 * @brief Sets the selected entity.
 * @param entity Entity ID to select.
 */
	void SetSelectedEntity(Entity entity);

	/**
   * @brief Renders ImGui windows.
   */
	void Render();

private:
	Entity selectedEntity;
};

#endif // !__LUQUI_IMGUI__