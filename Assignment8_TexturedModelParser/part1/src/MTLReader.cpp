#include "MTLReader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

MTLReader::MTLReader() {}

void MTLReader::printAllMaterials() const {
  for (const auto &[name, mat] : materials) {
    std::cout << "Material: " << name << std::endl;
    std::cout << "  Shininess: " << mat.shininess << std::endl;
    std::cout << "  Ambient: " << mat.ambient.r << " " << mat.ambient.g << " "
              << mat.ambient.b << std::endl;
    std::cout << "  Diffuse: " << mat.diffuse.r << " " << mat.diffuse.g << " "
              << mat.diffuse.b << std::endl;
    std::cout << "  Specular: " << mat.specular.r << " " << mat.specular.g
              << " " << mat.specular.b << std::endl;
    std::cout << "  Emissive: " << mat.emissive.r << " " << mat.emissive.g
              << " " << mat.emissive.b << std::endl;
    std::cout << "  Optical Density: " << mat.opticalDensity << std::endl;
    std::cout << "  Transparency: " << mat.transparency << std::endl;
    std::cout << "  Illumination Model: " << mat.illuminationModel << std::endl;
    // We skip the PPM-related attributes to simplify debugging.
    std::cout << std::endl;
  }
}

bool MTLReader::loadMaterialsFromFile(const std::string &filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Failed to open MTL file: " << filepath << std::endl;
    return false;
  }

  std::string line;
  Material currentMaterial;
  bool firstMaterial = true;

  // In the OBJModel::loadModelFromFile, when calling
  // mtlReader.loadMaterialsFromFile
  std::cout << "MTLReader: Current path: " << std::filesystem::current_path()
            << '\n';
  std::cout << "MTLReader: filepath: " << filepath << std::endl;

  std::string directory = filepath.substr(0, filepath.find_last_of("/\\") + 1);
  std::cout << "MTLReader: directory: " << directory << std::endl;

  while (getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "newmtl") {
      if (!firstMaterial) {
        materials[currentMaterial.name] =
            std::move(currentMaterial); // Save the previous material
      } else {
        firstMaterial = false;
      }
      std::cout << "MTLReader: New material: " << currentMaterial.name
                << std::endl;
      // Initialize a new material
      currentMaterial = Material();
      iss >> currentMaterial.name;
    } else if (prefix == "Ns") {
      iss >> currentMaterial.shininess;
    } else if (prefix == "Ka") {
      iss >> currentMaterial.ambient.r >> currentMaterial.ambient.g >>
          currentMaterial.ambient.b;
    } else if (prefix == "Kd") {
      iss >> currentMaterial.diffuse.r >> currentMaterial.diffuse.g >>
          currentMaterial.diffuse.b;
    } else if (prefix == "Ks") {
      iss >> currentMaterial.specular.r >> currentMaterial.specular.g >>
          currentMaterial.specular.b;
    } else if (prefix == "Ke") {
      iss >> currentMaterial.emissive.r >> currentMaterial.emissive.g >>
          currentMaterial.emissive.b;
    } else if (prefix == "Ni") {
      iss >> currentMaterial.opticalDensity;
    } else if (prefix == "d") {
      iss >> currentMaterial.transparency;
    } else if (prefix == "illum") {
      iss >> currentMaterial.illuminationModel;
    } else if (prefix == "map_Kd") {
      std::string textureFilename;
      iss >> textureFilename;
      currentMaterial.diffuseMap =
          PPM(directory +
              textureFilename); // create a PPM object for the diffuse map
    } else if (prefix == "map_Bump") {
      std::string textureFilename;
      iss >> textureFilename;
      currentMaterial.bumpMap = PPM(
          directory + textureFilename); // create a PPM object for the bump map
    } else if (prefix == "map_Ks") {
      std::string textureFilename;
      iss >> textureFilename;
      currentMaterial.specularMap =
          PPM(directory +
              textureFilename); // create a PPM object for the specular map
    }
  }

  // Don't forget to save the last material read
  if (!firstMaterial) {
    materials[currentMaterial.name] =
        std::move(currentMaterial); // use std::move for efficiency
  }

  file.close();
  return true;
}

Material MTLReader::getMaterial(const std::string &name) const {
  auto it = materials.find(name);
  if (it != materials.end()) {
    return it->second;
  } else {
    std::cerr << "Material not found: " << name << std::endl;
    return Material();
  }
}
