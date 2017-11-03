#include "../include/Core/ResourceManager.h"

namespace gpx
{
	AssetLoader::ShaderRepo AssetLoader::shaderObjects = ShaderRepo();
	AssetLoader::MeshRepo AssetLoader::meshObjects = MeshRepo();
	AssetLoader::TextureRepo AssetLoader::textureObjects = TextureRepo();
	std::string AssetLoader::shaderRootDir = std::string("assets/shaders/");
	std::string AssetLoader::meshRootDir = std::string("assets/models/");
	std::string AssetLoader::textureRootDir = std::string("assets/images/");
}   