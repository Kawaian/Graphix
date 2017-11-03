 /* 
 Graphix (outstar engine ) main test sample;
 by Matyas Constans 2016
  
  example of a deferred rendered scene
  
 */

// main include files of the engine;
#include "../include/Graphics/render.h"
#include "../include/Physics/physics.h"
#include "../include/Core/Core.h"
#include "../include/GL/Common.h"

// 1- we need this to disable some SDL subroutine (due to CMake)
// 2- gain some horizontal space; you probably should not do this,
// this is just for the sake of keeping things simple
#undef main
using namespace gpx;

int main(int argc, char** args) {
  // 1- Init Framework ( + Subsystems ) + Check for Errors
  // 2- Log an error msg with Boost-log, and exit program ! 
  if (InitCore() < 0) {
    BOOST_LOG_TRIVIAL(error) << "Failed to init Graphix framework !";
    return -1; 
  }
  // Create a context; this is our main handle to the Framework
  ApplicationContext context; 

  // Create a GraphicsDisplay (a.k.a Window) by sending a request to our context
  // note that we are loading our window settings from an XML file
  // using the ConfigLoader utility function
  // ( note ) Use auto& when creating any resource objects; 
  // the system is actually giving you unique_ptr refs
  // but you should not mess with that !
  auto& display = context.AttachDisplay(
    ConfigLoader<Tags::DisplayConfig>::LoadConfig("display.config"));

  // Create an InputHandler to get Keyboard input (we use the same class 
  // for mouse clicks, controllers, etc...
  auto& input = context.AttachInputHandler();

  // Now we can create a Camera: note that Cams are perspective by default
  // since this is a 3d game engine, but you can render sprites with
  // the SpriteBatch class.
  auto& camera = context.AttachCamera(glm::vec3(0.0f, 12.0f, 24.0f), 70.0f,
    (float)(display->GetXPosition() + display->GetWidth()) /
    (float)(display->GetXPosition() + display->GetHeight()),
    0.01f, 500.0f);

  // Now, we're loading shaders from the AssetLoader class
  // The default paths (can be changed) are asset/shader, assets/models,
  // and assets/images. Note that we refer to resource object with IDs (identifiers)
  // and they can be accessed with .Shader, .Texture, etc...
//  AssetLoader::LoadShader("Render", "basicShaderUBO.vs", "phongShader.fs");
  AssetLoader::LoadShader("Geometry", "geometryPass.vs", "geometryPass.fs");
  AssetLoader::LoadShader("PointLight", "lightPass.vs", "lightPass.fs");
  AssetLoader::LoadShader("AmbientLight", "lightPass.vs", "ambientLightPass.fs");
  AssetLoader::LoadShader("Hdr", "ReinhardToneMap.vs", "ReinhardToneMap.fs");
  AssetLoader::LoadMesh("Plane", "primitives/Plane.obj");
  AssetLoader::LoadMesh("Sphere", "primitives/UVSphere.obj");
  AssetLoader::LoadMesh("LightVolume", "primitives/ICOSphere.obj");
  AssetLoader::LoadTexture("Debug", "Debug.png");
  AssetLoader::LoadTexture("Bricks", "White.png", TextureType::Texture2D, GL_LINEAR_MIPMAP_LINEAR);
  AssetLoader::LoadTexture("BricksNRM", "NormalMap.jpg", TextureType::Texture2D, GL_LINEAR_MIPMAP_LINEAR);
  
  // Get some resource handles to avoid table lookup time (you should do this)
  auto& hdrShader =                 AssetLoader::Shader("Hdr");
  auto& ambientLightPassShader =    AssetLoader::Shader("AmbientLight");
  auto& geometryPassShader =        AssetLoader::Shader("Geometry");
  auto& pointLightPassShader =      AssetLoader::Shader("PointLight");
  auto& terrain =                     AssetLoader::Mesh("Plane");
  auto& mini_terrain =                    AssetLoader::Mesh("Sphere");
  auto& lightVolume =               AssetLoader::Mesh("LightVolume");
  auto& debugTexture =              AssetLoader::Texture("Debug");
  auto& bricksTexture =             AssetLoader::Texture("Bricks");
  auto& bricksNRMTexture =          AssetLoader::Texture("BricksNRM"); 

  // Let's define a quad so we can render it to the screen
  // ( for deferred rendering and tone mapping )
  gl4::GLTypeCT<float> quadVertices[] = {
    -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f, 1.0f,  -1.0f, 0.0f,
  };
  
  // Define VertexArray + VertexBuffer + VertexArrayAttribs
  gl4::VertexArray vertexArray;
  gl4::VertexBuffer vertexBuffer;
  vertexArray.Bind();
  vertexBuffer.Bind();
  gl4::VertexBuffer::BufferData(12, quadVertices);
  gl4::VertexArrayAttribute vertexAttrib(pointLightPassShader.get(), "position");
  vertexAttrib.Enable(0);
  vertexAttrib.Define<float>(0, 3);
  vertexAttrib.Disable(0);
  vertexBuffer.UnBind();

  // Convenience Lambda
  auto RenderQuad = [&] {
    vertexArray.Bind();
    vertexAttrib.Enable(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    vertexAttrib.Disable(0);
    vertexBuffer.UnBind();
  };

  BoundingSphere camViewBoundSphere(camera->GetPosition(), camera->GetNearPlane());

  // Create a GBuffer; this is for deferred rendering ! 
  GBuffer gBuffer(display->GetWidth(), display->GetHeight());

  // Create a Material object, storing the object's data about shininess, ...
  Material matbricks =
    Material(TextureMap(), 20.0f, glm::vec3(1, 0.727811, 0.633));

  // Create a 3D object; the engine refers to those objects as Spatials;
  // Essentially, they store texture, vertex, and transform data.
  // all Spatials inherit from the Entity base class (almost everything
  // can be seen as an entity in the engine).
  Spatial spatial(terrain.get());
  spatial.SetMaterial(matbricks);
  spatial.SetScale(glm::vec3(40.0, 40.0, 40.0));
  Spatial lightInd(mini_terrain.get());
  lightInd.SetMaterial(matbricks);
  lightInd.SetPosition(glm::vec3(0.0f, 20.0f, 0.0f));
  lightInd.SetScale(glm::vec3(5.0f, 5.0f, 5.0f));

  // Create a PointLight !
  PointLight pointlight;
  pointlight.SetPosition(glm::vec3(0.0, 6.0, 0.0));
  pointlight.SetIntensity(glm::vec3(10.0f, 10.0f, 10.0f));
  pointlight.SetAmbientCoefficient(0.000f);

  // Specify the attenuation factors of that pointlight !
  Attenuation attenuation;
  attenuation.linear = 0.032f;
  attenuation.constant = 5.0f;
  attenuation.exponential = 0.09f;
  pointlight.SetAttenuation(attenuation);

  // Let's put our light in an array (for multiple lights)
  static const unsigned NUM_LIGHTS = 1;
  PointLight pointLights[NUM_LIGHTS];
  pointLights[0] = pointlight;

  // Enable Cullface-ing (for performance)
  gl4::FaceCullState::SwitchState(true);
  gl4::FaceCullState::CullMode(gl4::FaceCullMode::Back);

  // Main Render/Update cycle;
  // we can use the RenderCycle utility function !
  bool isrunning = true;
  unsigned fps = 0;
  util::RenderCycle(
    2000, isrunning,
    [&](float delta) {
    // make sure to update window, inputhandler, ... : all the things
    // we attached to our context
    context.UpdateEntities();
    
    // Utility function to give our camera a free, FPS style look.
    util::ApplyFreeFPSLook(*camera, *input, *display, delta, 0.05f, 0.5f,
        glm::ivec2(display->GetXPosition() + display->GetWidth() / 2.0,
          display->GetYPosition() + display->GetWidth() / 2.0f));
    
    // Update the position of our camera bounding sphere
    camViewBoundSphere = BoundingSphere(camera->GetPosition(), camera->GetNearPlane());
    },
    [&]() {
    // Render entities attached to our context (window, etc..)
    context.RenderEntities();
    // Bind viewport uniforms
    context.BindViewportUniforms();

    // Start 1st frame of our GBuffer
    gBuffer.StartFrame();
    
    // -1st pass: GeometryPass
    gBuffer.BindForGeomPass();
    
    // -1 Enable DepthMask 
    // -2 clear geometry buffer
    // (these gl functions are under the process
    // of being wrapped; but that is almost done, as you can see with
    // blending functions and faceculling).
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    gl4::BlendState::SwitchState(false);
   
    // bind the geometry shader
    geometryPassShader->Bind();

    // Update model and normal uniforms
    // note that you need to that manually for now
    // you will not have to when the scene graph system is stable
    geometryPassShader->UpdateUniformMatrix4f(
      "model", spatial.GetTransform().GetModel());
    geometryPassShader->UpdateUniformMatrix3f(
      "normalMatrix", glm::inverseTranspose(
        glm::mat3(spatial.GetTransform().GetModel())));

    // set material uniforms (again this will be the job of the scene graph)
    matbricks.SetUniforms(geometryPassShader.get());

    // bind diffuse texture + get sampler location (this gl function 
    // is under the process of being wrapped too)
    bricksTexture->Bind(0);
    glUniform1i(geometryPassShader->GetUniformLocation("diffuseMap"), 0);
    
    // bind normal texture + get sampler location 
    bricksNRMTexture->Bind(1);
    glUniform1i(geometryPassShader->GetUniformLocation("normalMap"), 1);

    //render our terrain mesh
    terrain->Render();

    //do the same thing with the other spatial
    geometryPassShader->UpdateUniformMatrix4f(
      "model", lightInd.GetTransform().GetModel());
    geometryPassShader->UpdateUniformMatrix3f(
      "normalMatrix", glm::inverseTranspose(
        glm::mat3(lightInd.GetTransform().GetModel())));

    matbricks.SetUniforms(geometryPassShader.get());
    debugTexture->Bind(0);
    glUniform1i(geometryPassShader->GetUniformLocation("diffuseMap"), 0);
    mini_terrain->Render();

    // Light pass preparation:
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    
    gl4::BlendState::SwitchState(true);
    gl4::BlendState::BlendEquation(gl4::BlendEquation::FuncAdd);
    gBuffer.BindForLightPass();
    gl4::BlendState::BlendFunc(gl4::BlendFunc::One, gl4::BlendFunc::One);
    glClear(GL_COLOR_BUFFER_BIT);

    // The actuall light pass here:
    static const glm::mat4 MVP_IDENTITY = glm::mat4(1.0f);

    ambientLightPassShader->Bind();
    pointLightPassShader->UpdateUniformMatrix4f("MVP", MVP_IDENTITY);
    RenderQuad();

    // Render all PointLights
    pointLightPassShader->Bind();
     for (unsigned i = 0; i < NUM_LIGHTS; i++) {
       Attenuation attenuation = pointLights[i].GetAttenuation();

      float pointLightRadius = pointLights[i].GetInfluenceRadius();

      Transform sphereTrsf(
        pointLights[i].GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(pointLightRadius, pointLightRadius, pointLightRadius));

      // is our camera in the light volume ?
      // Let's compute sphere radius and check for 
      // an intersection with the camera bounding sphere !
      BoundingSphere lightSphere(pointLights[i].GetPosition(),
        pointLightRadius);

      pointLightPassShader->UpdateUniformMatrix4f("model",
        sphereTrsf.GetModel());
      pointLightPassShader->UpdateUniform3fv(
        "cameraPosition", glm::value_ptr(camera->GetPosition()));

      pointLights[i].SetUniforms(pointLightPassShader.get());
      spatial.GetMaterial().SetUniforms(pointLightPassShader.get());

      // Now let's render out the ambient light
      if (camViewBoundSphere.Intersect(lightSphere).Intersects()) {
        pointLightPassShader->UpdateUniformMatrix4f("MVP", MVP_IDENTITY);
        RenderQuad();
      }
      else {
        pointLightPassShader->UpdateUniformMatrix4f(
          "MVP", camera->Combined() * sphereTrsf.GetModel());
        lightVolume->Render();
      }
    }

    // Final touch here: the tone mapping; we use
    // an exposure of 0.5 here.
    gBuffer.BindFinalTexture();
    hdrShader->Bind();
    hdrShader->UpdateUniform1f("exposure", 0.5f);
    RenderQuad();

    // UnBind our viewport uniforms (not using them anymore)
    context.UnBindViewportUniforms();
  },
    fps, true);

  //Render cycle end, Program finishes
  return EXIT_SUCCESS;
}