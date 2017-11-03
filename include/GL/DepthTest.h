#pragma once

#include <GL/glew.h>
#include "Type.h"
#include "Tags.h"

namespace gpx {
  namespace gl4 {

    template<>
    class GLStateManager<Tags::DepthTest> {
    public:
      static void RequestState(const bool& state) {
        if (state) {
          if (!stateControl_) {
            glEnable(GL_DEPTH_TEST);
            stateControl_ = !stateControl_;
          }
        }
        else
          if (stateControl_) {
            glDisable(GL_DEPTH_TEST);
            stateControl_ = !stateControl_;
          }
      }
    private:
      static bool stateControl_;
    };
    bool GLStateManager<Tags::DepthTest>::stateControl_ = false;

    class DepthTest {
    public:
      static void SwitchState(const bool& state) {
        GLStateManager<Tags::DepthTest>::RequestState(state);
      }
    private:
    };
  }
}