#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Graphics/Camera.h"
#include "../Graphics/GraphicsDisplay.h"
#include "../Graphics/GraphicsDisplayConfig.h"
#include "../Graphics/InputHandler.h"
#include "../GL/UniformBuffer.h"
#include "../Core/ResourceManager.h"

namespace gpx {

class ApplicationContext {
public:
  explicit ApplicationContext() : viewport_trigger_(false) {
  }
  std::unique_ptr<GraphicsDisplay>& 
          AttachDisplay(const GraphicsDisplayConfig & config) {
    display_ = std::make_unique< 
      GraphicsDisplay>(config);
    return display_; 
  }
  std::unique_ptr<GraphicsDisplay>&
    AttachDisplay(unsigned x = 300, unsigned y = 200, unsigned int w = 800,
      unsigned int h = 700, const char* title = "",
      bool resizable = false, bool fullScreen = false) {
    display_ = std::make_unique<
      GraphicsDisplay>(x, y, w, h, title,
        resizable, fullScreen);
    return display_;
  }
  std::unique_ptr<Camera>&
    AttachCamera(glm::vec3 pos, float fieldOfView, 
      float aspect, float nearPlane = .00001f,
      float farPlane = 1000.0f, float horizontleAngle = .0f,
      float verticalAngle = .0f) {
    camera_ = std::make_unique<
      Camera>(pos, fieldOfView, aspect, nearPlane,
        farPlane, horizontleAngle, verticalAngle);
    if (!viewport_trigger_) {
      AssetLoader::LoadShader("Render", "basicShaderUBO.vs", 
        "phongShader.fs");
      _R__InitViewPortUBO();
      viewport_trigger_ = true;
    }
    return camera_;
  }
  std::unique_ptr<Camera>&
    AttachCamera(void) {
    camera_ = std::make_unique<
      Camera>();
    if (!viewport_trigger_) {
      AssetLoader::LoadShader("Render", "basicShaderUBO.vs",
        "phongShader.fs");
      _R__InitViewPortUBO();
      viewport_trigger_ = true;
    }
    return camera_;
  }
 private:
   void _R__InitViewPortUBO(void) {
     viewportUBO_ = std::make_unique<gl4::UniformBuffer>();
     viewportUBO_->Bind();
     gl4::UniformBuffer::EmptyBufferData(3 * sizeof(glm::mat4) + sizeof(glm::vec2));
     viewportUBO_->BindUniformBlock(AssetLoader::Shader("Render"), "ViewPort");
     viewportUBO_->UnBind();
     viewportUBO_->BufferRange(0, 0, sizeof(glm::mat4));
   }
   void _R__UpdateViewportUBO(void) {
     viewportUBO_->Bind();
     glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
       glm::value_ptr(camera_->Projection()));
     glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
       glm::value_ptr(camera_->View()));
     glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4),
       sizeof(glm::mat4), glm::value_ptr(camera_->Combined()));
     glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4),
       sizeof(glm::vec2),
       glm::value_ptr(glm::vec2(display_->GetWidth(), display_->GetHeight())));
     viewportUBO_->UnBind();
   }
   void _R__UpdateDisplay(void) {
     display_->SwapBuffers();
     display_->BindAsRenderTarget();
     display_->Clear(0.0f, 0.0f, 0.0f, 1.0f);
   }
   void _U__UpdateDisplay(void) {
     display_->Update();
   }
   void _U__UpdateInput(void) {
     input_->Update();
     if (input_->KeyDown(SDL_SCANCODE_ESCAPE) ||
            input_->CloseRequested() )
       exit(1);
   }
 public:
   void BindViewportUniforms() {
     viewportUBO_->Bind();
   }
   void UnBindViewportUniforms() {
     viewportUBO_->UnBind();
   }
   void RenderEntities() {
     _R__UpdateDisplay();
     _R__UpdateViewportUBO();
   }
   void UpdateEntities() {
     _U__UpdateDisplay();
     _U__UpdateInput();
   }

  std::unique_ptr<InputHandler>&
    AttachInputHandler() {
    input_ = std::make_unique<
      InputHandler>();
    return input_;
  }
private:
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<GraphicsDisplay> display_;
  std::unique_ptr<InputHandler> input_;
  std::unique_ptr<gl4::UniformBuffer> viewportUBO_;
  bool viewport_trigger_;
};
}