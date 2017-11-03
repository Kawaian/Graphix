#pragma once

#include <type_traits>
#include <GL/glew.h>
#include "Type.h"

namespace gpx {
namespace gl4 {
namespace Tags {

#define GL_DEFINE_OBJECT_TYPES(ID, TYPE) \
struct TYPE : std::integral_constant<int, ID> \
{ \
  typedef GLType::UInteger internal; \
};

//objects
GL_DEFINE_OBJECT_TYPES(1, Texture)
GL_DEFINE_OBJECT_TYPES(2, Shader)
GL_DEFINE_OBJECT_TYPES(3, FrameBuffer)
GL_DEFINE_OBJECT_TYPES(4, RenderBuffer)
GL_DEFINE_OBJECT_TYPES(5, Program)
GL_DEFINE_OBJECT_TYPES(6, VertexArray)
GL_DEFINE_OBJECT_TYPES(7, VertexBuffer)
GL_DEFINE_OBJECT_TYPES(8, UniformBuffer)

//states
GL_DEFINE_OBJECT_TYPES(-1,  Blending)
GL_DEFINE_OBJECT_TYPES(-2,  DepthTest)
GL_DEFINE_OBJECT_TYPES(-3,  FaceCull)

#undef GL_DEFINE_OBJECT_TYPES
} //namespace Tags

template<typename T>
struct GLObjDelAlloc {
};
template<typename T>
struct GLObjBind {
};
template<typename T>
struct GLStateManager {
};
} //namespace GLLayer
} //namespace Spiky
