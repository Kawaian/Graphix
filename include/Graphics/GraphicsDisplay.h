#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "../include/Graphics/LogManager.h"
#include "../include/Graphics/SDL_Handle.h"
#include "../include/Graphics/GraphicsDisplayConfig.h"

namespace gpx {
class GraphicsDisplay {
 public:
  GraphicsDisplay(unsigned x = 300, unsigned y = 200, unsigned int w = 800,
                  unsigned int h = 700, const char* title = "",
                  bool resizable = false, bool fullScreen = false);
  GraphicsDisplay(const GraphicsDisplayConfig& config);
  virtual ~GraphicsDisplay();
  void Clear(float r, float g, float b, float a);
  void SwapBuffers();
  void Update();
  bool Resized();
  inline unsigned GetWidth() { return w_; }
  inline unsigned GetHeight() { return h_; }
  inline const char* GetTitle() const { return title_; }
  inline bool HasMoved() { return deltaX_ != x_ || deltaY_ != y_; }
  void BindAsRenderTarget();
  void SetWidth(unsigned w) { SDL_SetWindowSize(object_, w, h_); }
  void SetHeight(unsigned h) { SDL_SetWindowSize(object_, w_, h); }
  void SetTitle(const char* title) { SDL_SetWindowTitle(object_, title); }
  void SetMousePosition(unsigned x, unsigned y) {
    SDL_WarpMouseInWindow(object_, x, y);
  }
  void CaptureMouse(SDL_bool b) { SDL_SetRelativeMouseMode(b); }
  void BlockPosition(bool b) {
    bocked_ = b;
    blockX_ = x_;
    blockY_ = y_;
  }
  unsigned GetXPosition() { return x_; }
  unsigned GetYPosition() { return y_; }
  void SetPosition(unsigned x, unsigned y) {
    SDL_SetWindowPosition(object_, x, y);
  }
  void SetMouseVisible(bool b) {
    int toggle = 0;
    if (b) toggle = 1;
    SDL_ShowCursor(toggle);
  };
  GraphicsDisplay& operator=(const GraphicsDisplay& d) = delete;
  GraphicsDisplay(const GraphicsDisplay&) = delete;
 private:
  unsigned x_;
  unsigned y_;
  unsigned deltaX_;
  unsigned deltaY_;
  unsigned w_;
  unsigned h_;
  unsigned deltaW_;
  unsigned deltaH_;
  const char* title_;
  SDL_Window* object_;
  SDL_GLContext glContext_ = 0;
  bool bocked_ = false;
  unsigned blockX_ = x_;
  unsigned blockY_ = y_;
};
}