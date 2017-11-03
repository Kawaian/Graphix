#pragma once

#include <GL/glew.h>
#include "Type.h"
#include "Tags.h"

namespace gpx {
  namespace gl4 {

    template<>
    class GLStateManager<Tags::FaceCull> {
    public:
      static void RequestState(const bool& state) {
        if (state) {
          if (!stateControl_) {
            glEnable(GL_CULL_FACE);
            stateControl_ = !stateControl_;
          }
        }
        else
          if (stateControl_) {
            glDisable(GL_CULL_FACE);
            stateControl_ = !stateControl_;
          }
      }
    private:
      static bool stateControl_;
    };
    bool GLStateManager<Tags::FaceCull>::stateControl_ = false;

    struct FaceCullMode {
      static const unsigned int Front = GL_FRONT;
      static const unsigned int Back = GL_BACK;
      static const unsigned int FrontAndBack = GL_FRONT_AND_BACK; 
    };

    class FaceCullState {
    public:
      static void SwitchState(const bool& state) {
        GLStateManager<Tags::FaceCull>::RequestState(state);
      }
      static void CullMode(const GLenum& cullMode) {
        glCullFace(cullMode);
      }
    };
  }
}