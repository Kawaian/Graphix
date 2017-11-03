#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/log/trivial.hpp>
#include <exception>

#include "ConfigLoader.h"

namespace gpx {

struct GLConfiguration {
  bool cullFace = false;
  bool depthTest = false;
  bool blending = false;
};

struct GraphicsDisplayConfig {
  unsigned x = 0;
  unsigned y = 0;
  unsigned w = 0;
  unsigned h = 0;
  std::string title = "";
  bool resizable = false;
  bool fullScreen = false;
  GLConfiguration glConfig;
};

template<>
class ConfigLoader<Tags::DisplayConfig> {
public:
  static GraphicsDisplayConfig LoadConfig(const char* path) {
    GraphicsDisplayConfig config;
    try {
      boost::property_tree::ptree pt;
      boost::property_tree::read_xml(path, pt);

      config.title = pt.get<std::string>("display.title");
      config.x = pt.get<unsigned>("display.position.<xmlattr>.x");
      config.y = pt.get<unsigned>("display.position.<xmlattr>.y");
      config.w = pt.get<unsigned>("display.resolution.<xmlattr>.w");
      config.h = pt.get<unsigned>("display.resolution.<xmlattr>.h");
      config.glConfig.cullFace = pt.get<bool>("display.GL.cullFace");
      config.glConfig.depthTest = pt.get<bool>("display.GL.depthTest");
      config.glConfig.blending = pt.get<bool>("display.GL.blending");
    
      std::cout << config.glConfig.cullFace << std::endl;
      std::cout << config.glConfig.depthTest << std::endl;
      std::cout << config.glConfig.blending << std::endl;
    }
    catch (std::exception& e) {
      BOOST_LOG_TRIVIAL(error) << e.what();
    }
    return config;
  }
};
} //namespace Spiky