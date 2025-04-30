#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>

#include "WindowSystem.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "ECS/Component.hpp"
#include "ECS/System.hpp"
#include "ECS/ECSManager.hpp"
#include "Imgui.hpp"
#include "../deps/imgui/imgui.h"

void ResetGame(ECSManager& ecsmanager, Entity naveEntity, float& currentTargetX, float& speed, float spacing) {
    currentTargetX = 0.0f;
    speed = 0.0f;

    // Resetear la nave
    if (auto naveTransform = ecsmanager.getComponent<TransformComponent>(naveEntity)) {
        naveTransform.value()->position = { 0.0f, 120.0f, 0.0f };
    }

    // Resetear carriles
    int i = 0;
    for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity) {
        if (!ecsmanager.isEntityAlive(entity)) continue;

        auto modelComp = ecsmanager.getComponent<RenderComponent>(entity);
        auto transform = ecsmanager.getComponent<TransformComponent>(entity);
        if (!modelComp || !transform) continue;

        if (modelComp.value()->model->get_name() == "carril") {
            int lane = i / 2;
            int platformIndex = i % 2;
            float baseZ = -1500.0f;
            float gap = 150.0f;

            float newZ = baseZ + platformIndex * (1000.0f + gap);
            float newX = (float)(lane - 1) * 230.0f;
            float newY = 100.0f;

            transform.value()->position = { newX, newY, newZ };

            ++i;
        }
    }
}

int main() {
  auto WS = WindowSystem::make();
  auto window = Window::make(LUQUI_Window_Width, LUQUI_Window_Height, "LUQUI");
  if (nullptr == window->window) {
    return -1;
  }
  srand((unsigned int)time(NULL));
  window->setCurrentWindowActive();

  // Declaramos un gestor de input asociado a la ventana en activo.
  Input input(window->window);
  float speed = 0.0f;
  float maxSpeed = 600.0f;
  float acceleration = 300.0f;
  float deceleration = 400.0f;
  float currentTargetX = 0.0f;
  float lateralSpeed = 200.0f;
  float spacing = 0.0f;

  ///////START OF PROGRAM & SHADERS/////
  Shader vertex = Shader();
  if (!vertex.loadFromFile(Shader::ShaderType::kShaderType_Vertex, "../data/Shaders/vertex.vs")) {
    std::cerr << "Error al cargar el vertex shader desde archivo." << std::endl;
    return -2;
  }
  if (!vertex.compile()) {
    std::cerr << "Error al compilar el vertex shader." << std::endl;
    return -3;
  }

  Shader fragment = Shader();
  if (!fragment.loadFromFile(Shader::ShaderType::kShaderType_Fragment, "../data/Shaders/fragment.fs")) {
    std::cerr << "Error al cargar el fragment shader desde archivo." << std::endl;
    return -4;
  }
  if (!fragment.compile()) {
    std::cerr << "Error al compilar el fragment shader." << std::endl;
    return -5;
  }
  /** Creating Program */
  Program program = Program();
  program.attach(&vertex);
  program.attach(&fragment);
  if (!program.link()) {
    std::cout << "Error al linkar el programa" << std::endl;
    return -4;
  }
  ///////END OF PROGRAM & SHADERS/////


  // Creamos un gestor de entidades
  ECSManager ecsmanager;

  // Declaramos los sistemas que se utilizar�n en el programa.
  RenderSystem renderSystem;
  InputSystem inputSystem;

  // Crear listas de componentes
  ecsmanager.addComponentType<TransformComponent>();
  ecsmanager.addComponentType<InputComponent>();
  ecsmanager.addComponentType<RenderComponent>();
  ecsmanager.addComponentType<LightComponent>();
  ecsmanager.addComponentType<CameraComponent>();
  ecsmanager.addComponentType<NameComponent>();

  //Camera Entity
  Entity CameraEntity = ecsmanager.createEntity();
  ecsmanager.addComponent<CameraComponent>(CameraEntity);
  ecsmanager.editComponent<CameraComponent>(CameraEntity, [](CameraComponent& camera) {
    camera.position = glm::vec3(0.0f, 300.0f, 300.0f);
    camera.updateViewMatrix();
    });

  ecsmanager.addComponent<InputComponent>(CameraEntity);
  ecsmanager.editComponent<InputComponent>(CameraEntity, [](InputComponent& input) {
    input.active = false;
    input.followingMouse = false;
    });

  ecsmanager.addComponent<TransformComponent>(CameraEntity);
  ecsmanager.editComponent<TransformComponent>(CameraEntity, [](TransformComponent& tr) {
    tr.position = { -200.0f, 130.0f, 40.0f };
    });

  ecsmanager.addComponent<NameComponent>(CameraEntity);
  ecsmanager.editComponent<NameComponent>(CameraEntity, [](NameComponent& cameraName) {
    cameraName.name = "Camera";
    });
  // Crear una entidad para la luz
  Entity lightEntity = ecsmanager.createEntity();

  ecsmanager.editComponent<LightComponent>(lightEntity, [](LightComponent& light) {
    light.type = LightType::Directional; 
    light.color = glm::vec3(1.0f, 1.0f, 1.0f); 
    light.position = glm::vec3(20.0f, 200.0f, 0.0f);
    light.direction = glm::vec3(0.0f, -1.0f, -0.3f);
    light.intensity = 2.0f; 
    light.radius = 250.0f; 
    });

  ecsmanager.addComponent<NameComponent>(lightEntity);
  ecsmanager.editComponent<NameComponent>(lightEntity, [](NameComponent& cameraName) {
    cameraName.name = "Directional Light";
    });

  auto nave_mesh = std::make_shared<Model>("../data/Models/SpaceShip.obj", "player");
  ecsmanager.resources.push_back(nave_mesh);

  Entity naveEntity = ecsmanager.createEntity();

  ecsmanager.editComponent<TransformComponent>(naveEntity, [](TransformComponent& transform) {
      transform.position = { 0.0f, 120.0f, 0.0f };
      transform.rotation = { 0.0f,180.0f,0.0f };
      transform.scale = { 10.0f, 10.0f, 10.0f }; 
      });

  ecsmanager.editComponent<RenderComponent>(naveEntity, [&](RenderComponent& modelComp) {
      modelComp.model = nave_mesh;
      });

  auto cube_mesh = std::make_shared<Model>("../data/Models/cube/cube.obj", "carril");
ecsmanager.resources.push_back(cube_mesh);

for (int lane = 0; lane < 3; ++lane) {
    for (int i = 0; i < 2; ++i) {
        Entity laneEntity = ecsmanager.createEntity();

        ecsmanager.editComponent<TransformComponent>(laneEntity, [lane, i, &spacing](TransformComponent& transform) {
            float baseZ = -1500.0f; 
            float gap = 150.0f;      
            spacing = 1000.0f + gap; 

            transform.position = {
                (float)(lane - 1) * 230.0f,
                100.0f,
                baseZ + i * spacing
            };

            if (i == 0) {
                switch (lane) {
                case 0:
                    transform.rotation = { 0.0f, 0.0f, 0.0f };
                    transform.scale = { 60.0f, 5.0f, 500.0f };
                    break;
                case 1:
                    transform.rotation = { 90.0f, 0.0f, 0.0f };
                    transform.scale = { 60.0f, 500.0f, 5.0f };
                    break;
                case 2:
                    transform.rotation = { 0.0f, 0.0f, 180.0f };
                    transform.scale = { 60.0f, 5.0f, 500.0f };
                    break;
                }
            }
            else {
                switch (lane) {
                case 0:
                    transform.rotation = { 90.0f, 0.0f, 0.0f };
                    transform.scale = { 60.0f, 500.0f, 5.0f };
                    break;
                case 1:
                    transform.rotation = { 270.0f, 0.0f, 0.0f };
                    transform.scale = { 60.0f, 500.0f, 5.0f };
                    break;
                case 2:
                    transform.rotation = { 0.0f, 180.0f, 90.0f };
                    transform.scale = { 5.0f, 60.0f, 500.0f };
                    break;
                }
            }
            });

        ecsmanager.editComponent<RenderComponent>(laneEntity, [&](RenderComponent& renderComp) {
            renderComp.model = cube_mesh;
            });
    }
}


  //////////////////////////////////

  LuquiImgui LuquiImgui(&window.value());
  bool isJumping = false;
  bool isFalling = false;
  float jumpVelocity = 0.0f;
  const float jumpStrength = 400.0f;
  const float gravity = 800.0f;
  const float groundY = 120.0f;
  bool timerStarted = false;
  float gameTime = 0.0f;

  // Ciclo del juego
  while (!window->isOpen()) {
    LuquiImgui.NewFrame();
    if (input.isKeyPressed(Input::Key::KEY_W)) {
        speed += acceleration * 0.016f;

        if (!timerStarted) {
            timerStarted = true;
            gameTime = 0.0f;
        }
    }
    if (timerStarted) {
        gameTime += 0.016f; 
    }
    if (input.isKeyPressed(Input::Key::KEY_S)) {
        speed -= deceleration * 0.016f;
    }

    if (speed > maxSpeed) speed = maxSpeed;
    if (speed < 0.0f) speed = 0.0f;

    if (input.isKeyPressed(Input::Key::KEY_A)) {
        currentTargetX -= lateralSpeed * 0.016f;
    }
    if (input.isKeyPressed(Input::Key::KEY_D)) {
        currentTargetX += lateralSpeed * 0.016f;
    }

    if (!isJumping && input.isKeyPressed(Input::Key::KEY_SPACE)) {
        isJumping = true;
        jumpVelocity = jumpStrength;
    }

    if (auto naveTransform = ecsmanager.getComponent<TransformComponent>(naveEntity)) {
        glm::vec3& pos = naveTransform.value()->position;

        if (pos.y == groundY && (
            pos.x < -310.0f ||
            pos.x > 320.0f ||
            (pos.x >= -130.0f && pos.x <= -100.0f) ||
            (pos.x >= 100.0f && pos.x <= 130.0f))) {
            isFalling = true;
        }

        if (auto naveTransform = ecsmanager.getComponent<TransformComponent>(naveEntity)) {
            glm::vec3& pos = naveTransform.value()->position;

            for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity) {
                if (!ecsmanager.isEntityAlive(entity)) continue;

                auto modelComp = ecsmanager.getComponent<RenderComponent>(entity);
                auto transform = ecsmanager.getComponent<TransformComponent>(entity);
                if (!modelComp || !transform) continue;

                if (modelComp.value()->model->get_name() == "carril") {
                    glm::vec3& PlatformPos = transform.value()->position;

                    if (pos.y == groundY && (PlatformPos.z >= 480.0f && PlatformPos.z <= 570.0f)) {
                        isFalling = true;
                    }
                }
            }

        }

    }

    if (auto naveTransform = ecsmanager.getComponent<TransformComponent>(naveEntity)) {
        if (isFalling) {
            naveTransform.value()->position.y -= gravity * 0.016f;

            if (naveTransform.value()->position.y < -200.0f) {
                ResetGame(ecsmanager, naveEntity, currentTargetX, speed, spacing);
                isFalling = false;
                timerStarted = false;
                gameTime = 0.0f;
            }
        }
    }
    if (auto naveTransform = ecsmanager.getComponent<TransformComponent>(naveEntity)) {
        naveTransform.value()->position.x = currentTargetX;
    }

    if (auto naveTransform = ecsmanager.getComponent<TransformComponent>(naveEntity)) {
        if (isJumping) {
            naveTransform.value()->position.y += jumpVelocity * 0.016f;
            jumpVelocity -= gravity * 0.016f;

            if (naveTransform.value()->position.y <= groundY) {
                naveTransform.value()->position.y = groundY;
                jumpVelocity = 0.0f;
                isJumping = false;
            }
        }

    }

    

    for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity) {
        if (!ecsmanager.isEntityAlive(entity)) continue;

        auto modelComp = ecsmanager.getComponent<RenderComponent>(entity);
        auto transform = ecsmanager.getComponent<TransformComponent>(entity);
        if (!modelComp || !transform) continue;

        if (modelComp.value()->model->get_name() == "carril") {
            transform.value()->position.z += speed * 0.016f;

            if (transform.value()->position.z > 600.0f) {
                transform.value()->position.z -= spacing * 2.0f;
            }
        }
    }


    for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity)
    {
      if (auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity))
      {
        RenderSystem::UpdateTransformMatrix(*transformOpt.value());
      }
    }
    glClearColor(0.4, 0.4, 0.4, 1.0);
    glFrontFace(GL_CCW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);



    program.use();
    glm::mat4x4 model, view, projection;
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    // Gestion de camara
    auto cameraComponent = ecsmanager.getComponent<CameraComponent>(CameraEntity);
    auto inputComp = ecsmanager.getComponent<InputComponent>(CameraEntity);
    if (cameraComponent) {
      if (inputComp)
        inputSystem.update(inputComp.value(), cameraComponent.value(), input, 0.016f);

      // Usa las matrices de la cámara del componente
      cameraComponent.value()->updateViewMatrix();
      cameraComponent.value()->updateProjectionMatrix();
      view = cameraComponent.value()->view;
      projection = cameraComponent.value()->projection;
    }

    for (Entity Light_entity = 1; Light_entity < ecsmanager.get_nextEntity(); ++Light_entity) {
      // Configurar la luz en el shader
      if (!ecsmanager.isEntityAlive(Light_entity)) continue;
      auto lightOpt = ecsmanager.getComponent<LightComponent>(Light_entity);
      if (!lightOpt.has_value()) continue;
      if (lightOpt && lightOpt.value()->type == LightType::Point) {
        //GLuint pointLightIntensityLoc = glGetUniformLocation(program.get_id(), "pointLightIntensity");
        program.setVec3("pointLightColor", lightOpt.value()->color);
        program.setVec3("pointLightPosition", lightOpt.value()->position);
        program.setFloat("pointLightRadius", lightOpt.value()->radius);
        program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
        //glUniform1f(pointLightIntensityLoc, lightOpt.value()->intensity);

      }
      if (lightOpt && lightOpt.value()->type == LightType::Spot) {
        program.setVec3("spotlightColor", lightOpt.value()->color);
        program.setVec3("spotlightPosition", lightOpt.value()->position);
        program.setVec3("spotlightDirection", lightOpt.value()->direction);
        program.setFloat("spotlightIntensity", lightOpt.value()->intensity);
        program.setFloat("spotlightCutoff", lightOpt.value()->cutoff);
        program.setFloat("spotlightOuterCutoff", lightOpt.value()->outerCutoff);
        program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
      }
      if (lightOpt && lightOpt.value()->type == LightType::Directional) {
        program.setVec3("directionalLightColor", lightOpt.value()->color);
        program.setVec3("directionalLightDirection", lightOpt.value()->direction);
        program.setFloat("directionalLightIntensity", lightOpt.value()->intensity);
        program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
      }
      // Renderizar todas las entidades
      for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity) {
        if (!ecsmanager.isEntityAlive(entity)) continue;
        if (ecsmanager.getComponent<LightComponent>(entity).has_value()) continue;
        // Obtener los componentes de la entidad
        auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity);
        auto modelOpt = ecsmanager.getComponent<RenderComponent>(entity);
        auto inputComponentOpt = ecsmanager.getComponent<InputComponent>(entity);

        if (transformOpt && modelOpt) {


          // Pasar la matriz de modelo al shader
          GLuint modelLoc = glGetUniformLocation(program.get_id(), "model");
          glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformOpt.value()->transform_matrix));

          // Dibujar el modelo
          renderSystem.drawModel(transformOpt.value(), modelOpt.value(), program);
        }




      }
      glBlendFunc(GL_ONE, GL_ONE);
      glDepthMask(false);
    }
    auto camera = ecsmanager.getComponent<CameraComponent>(CameraEntity).value();
    // Pasar las matrices de vista y proyección al shader
    GLuint viewLoc = glGetUniformLocation(program.get_id(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(program.get_id(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDepthMask(true);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    program.unuse();
    // Intercambiar buffers
    ImGui::Begin("Contador");
    ImGui::Text("Tiempo: %.2f segundos", gameTime);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1000, 50), ImGuiCond_Always);
    ImGui::Begin("Velocidad");
    ImGui::Text("Velocidad actual: %.2f", speed);
    ImGui::End();
    LuquiImgui.Render();
    window->render();
  }

  window->~Window();
  WS->~WindowSystem();
}
