#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/log/trivial.hpp>
#include <lz4.h>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

namespace gpx {

class AssetLoader {
 public:
  AssetLoader(const char* fileName = "assets.rc") 
    : fReader_(fileName, std::ifstream::binary)
  {
    LoadIndexer();
  }
  ~AssetLoader() {}
  
 private:
  void LoadIndexer() {
    if (fReader_.good()) {
      std::string indexStr("");
      std::getline(fReader_, indexStr);

      std::istringstream iss("asdasd");
      std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::ostream_iterator<std::string>(std::cout, "\n"));
    }
    else{
      throw std::runtime_error("AssetPacker :\
           error loading file");
    }
  }
  std::ifstream fReader_;
  std::vector<std::string> indexBuffer_;
};
}