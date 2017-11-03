#pragma once

#include <GL/glew.h>
#include "Type.h"
#include "Tags.h"

namespace gpx {
namespace gl4 {

template<>
class GLStateManager<Tags::Blending>{
 public:
  static void RequestState(const bool& state) {
    if (state) {
      if (!stateControl_) {
        glEnable(GL_BLEND);
        stateControl_ = !stateControl_;
      }
    }
    else
      if (stateControl_){
        glDisable(GL_BLEND);
          stateControl_ = !stateControl_;
      }
  }
 private:
   static bool stateControl_;
};
bool GLStateManager<Tags::Blending>::stateControl_ = false;

struct BlendFunc {
  static const unsigned int Zero = GL_ZERO;
  static const unsigned int One = GL_ONE;
  static const unsigned int SrcColor = GL_SRC_COLOR;
  static const unsigned int OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR;
  static const unsigned int DstColor = GL_DST_COLOR;
  static const unsigned int OneMinusDstColor = GL_ONE_MINUS_DST_COLOR;
  static const unsigned int SrcAlpha = GL_SRC_ALPHA;
  static const unsigned int OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA;
  static const unsigned int DstAlpha = GL_DST_ALPHA;
  static const unsigned int OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA;
  static const unsigned int ConstantColor = GL_CONSTANT_COLOR;
  static const unsigned int OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR;
  static const unsigned int ConstantAlpha = GL_CONSTANT_ALPHA;
  static const unsigned int OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA;
  static const unsigned int SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE;
  static const unsigned int Src1Color = GL_SRC1_COLOR;
  static const unsigned int OneMinusSrc1Color = GL_ONE_MINUS_SRC1_COLOR;
  static const unsigned int Src1Alpha = GL_SRC1_ALPHA;
  static const unsigned int OneMinusSrc1Alpha = GL_ONE_MINUS_SRC1_ALPHA;
};

struct BlendEquation {
  static const unsigned int FuncAdd = GL_FUNC_ADD;
  static const unsigned int FuncSubstract = GL_FUNC_SUBTRACT;
  static const unsigned int FuncReverseSubstract = GL_FUNC_REVERSE_SUBTRACT;
};

class BlendState {
 public:
  static void SwitchState(const bool& state) {
    GLStateManager<Tags::Blending>::RequestState(state);
  }
  static void BlendFunc(const GLenum& sfactor, const GLenum& dfactor) {
    glBlendFunc(sfactor, dfactor);
  }
  static void BlendEquation(const GLenum& equation) {
    glBlendEquation(equation);
  }
 private:
  GLenum blendFunc_;
};
}
}