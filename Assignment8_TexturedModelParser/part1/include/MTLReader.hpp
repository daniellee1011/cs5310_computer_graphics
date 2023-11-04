#ifndef MTLREADER_HPP
#define MTLREADER_HPP

#include "PPM.hpp"

#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>

struct Material {
  std::string name;
  float shininess;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 emissive;
  float opticalDensity;
  float transparency;
  int illuminationModel;
  PPM diffuseMap;  // PPM object for the diffuse map
  PPM bumpMap;     // PPM object for the bump map
  PPM specularMap; // PPM object for the specular map
};

class MTLReader {
public:
  MTLReader();
  bool loadMaterialsFromFile(const std::string &filepath);
  Material getMaterial(const std::string &name) const;
  void printAllMaterials() const;

private:
  std::unordered_map<std::string, Material> materials;
};

#endif // MTLREADER_HPP
