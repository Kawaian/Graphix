#include <GL/glew.h>
#include "../include/Graphics/LogManager.h"
#include "../include/Graphics/SDL_Handle.h"
#include "../include/Graphics/GraphicsDisplay.h"
#include <iostream>
#include <boost/log/trivial.hpp>
#include "../include/Core/Exceptions.h"

using namespace gpx;

GraphicsDisplay::GraphicsDisplay(unsigned x, unsigned y, unsigned int w, unsigned int h, const char* title, bool resizable, bool fullScreen)
  :
  x_(x), y_(y), deltaX_(x_), deltaY_(y_), w_(w), h_(h),
  deltaW_(w_), deltaH_(h_), title_(title),
  object_(nullptr)
{
  try
  {
    if (fullScreen)
      object_ = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    else if (resizable)
      object_ = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    else
      object_ = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (object_ == nullptr)
      throw ContextInitException(std::string("Failed to create SDL context : ", SDL_GetError()));

    glContext_ = SDL_GL_CreateContext(object_);

    if (glContext_ == nullptr)
      throw ContextInitException(std::string("Failed to create opengl context : error #", glGetError()));

    GLenum init = glewInit();

    if (init != GLEW_OK)
      throw ContextInitException(std::string("Failed to create glew context : error #", *glewGetErrorString(init)));

    BOOST_LOG_TRIVIAL(info) << "Opengl version " << glGetString(GL_VERSION);
    BOOST_LOG_TRIVIAL(info) << "Opengl vendor " << glGetString(GL_VENDOR);
    BOOST_LOG_TRIVIAL(info) << "GLEW version " << glewGetString(GLEW_VERSION);

  }
  catch (const std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

GraphicsDisplay::GraphicsDisplay(const GraphicsDisplayConfig & config) 
  :
  GraphicsDisplay::GraphicsDisplay(config.x, config.y,
                                    config.w, config.h,
                                    config.title.c_str(), config.resizable,
                                    config.fullScreen)
{
  if (config.glConfig.blending)
    glEnable(GL_BLEND);
  if (config.glConfig.depthTest)
    glEnable(GL_DEPTH_TEST);
  if (config.glConfig.cullFace)
    glEnable(GL_CULL_FACE);
}

GraphicsDisplay::~GraphicsDisplay() {
  SDL_GL_DeleteContext(glContext_);
  SDL_DestroyWindow(object_);
}

void GraphicsDisplay::BindAsRenderTarget() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glViewport(0, 0, w_, h_);
}

void GraphicsDisplay::Clear(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsDisplay::SwapBuffers() {
  SDL_GL_SwapWindow(object_);
}


bool GraphicsDisplay::Resized() {
  return deltaW_ != w_ || deltaH_ != h_;
}

void GraphicsDisplay::Update() {
  deltaW_ = w_;
  deltaH_ = h_;
  deltaX_ = x_;
  deltaY_ = y_;
  int nw = w_;
  int nh = h_;
  SDL_GetWindowSize(object_, &nw, &nh);
  w_ = nw;
  h_ = nh;
  int nx = x_;
  int ny = y_;
  SDL_GetWindowPosition(object_, &nx, &ny);
  x_ = nx;
  y_ = ny;
  title_ = SDL_GetWindowTitle(object_);

  if (bocked_)
    SDL_SetWindowPosition(object_, blockX_, blockY_);
}