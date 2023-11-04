#include "OBJModel.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

// Default constructor. Initializes an empty model.
OBJModel::OBJModel() {
  //   std::cout << "OBJModel default constructor: Nothing loaded" <<
  //   std::endl;
}

// Loads and sets up the model from the given file path.
OBJModel::OBJModel(const std::string &filepath) { loadModelFromFile(filepath); }

// Sets up the OpenGL buffers and attribute pointers for rendering.
void OBJModel::setupBuffers() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(2);

  // UVs (textCoords)
  glVertexAttribPointer(
      3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void *)offsetof(Vertex, textCoord)); // Rename to texCoord for clarity.
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Renders the loaded model using the previously set up OpenGL buffers.
void OBJModel::render() const {
  glBindVertexArray(vao);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);
}

glm::vec3 generateColor(const glm::vec3 &normal) {
  glm::vec3 color = (normal + 1.0f) * 0.5f;
  return color;
}

// Loads model data from an OBJ file.
// Parses vertices, normals, and face indices, then sets up OpenGL buffers.
void OBJModel::loadModelFromFile(const std::string &filepath) {
  std::ifstream objFile(filepath);
  if (!objFile.is_open()) {
    std::cerr << "Failed to open the OBJ file: " << filepath << std::endl;
    return;
  }

  std::string line;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec3> temp_normals;
  std::vector<glm::vec2> temp_texCoords;
  MTLReader mtlReader; // Instance to handle MTL files
  bool materialsLoaded = false;

  while (std::getline(objFile, line)) {
    std::istringstream ss(line);
    std::string prefix;
    ss >> prefix;

    if (prefix == "mtllib") {
      std::string mtlFileName;
      ss >> mtlFileName;
      materialsLoaded = mtlReader.loadMaterialsFromFile(
          filepath.substr(0, filepath.find_last_of("/\\") + 1) + mtlFileName);
      if (!materialsLoaded) {
        std::cerr << "Failed to load materials from MTL file." << std::endl;
      } else {
        std::cout << "Successfully opened MTL file." << std::endl;
      }
      mtlReader.printAllMaterials();
    } else if (prefix == "usemtl") {
      std::string matName;
      ss >> matName;
      if (materialsLoaded) {
        currentMaterial = mtlReader.getMaterial(matName);
        if (currentMaterial.name.empty()) {
          std::cerr << "Material '" << matName
                    << "' not found in loaded materials." << std::endl;
        }
      }
    } else if (prefix == "v") {
      glm::vec3 vertex;
      ss >> vertex.x >> vertex.y >> vertex.z;
      temp_vertices.push_back(vertex);
    } else if (prefix == "vt") {
      glm::vec2 texCoord;
      ss >> texCoord.x >> texCoord.y;
      temp_texCoords.push_back(texCoord);
    } else if (prefix == "vn") {
      glm::vec3 normal;
      ss >> normal.x >> normal.y >> normal.z;
      temp_normals.push_back(normal);
    } else if (prefix == "f") {
      std::string segment;
      std::array<std::string, 3> vertexStrings;
      std::array<glm::ivec3, 3> vertexData;

      for (int i = 0; i < 3; i++) {
        ss >> vertexStrings[i];
        std::replace(vertexStrings[i].begin(), vertexStrings[i].end(), '/',
                     ' ');
        std::istringstream vss(vertexStrings[i]);
        vss >> vertexData[i].x >> vertexData[i].y >> vertexData[i].z;
      }

      for (int i = 0; i < 3; i++) {
        auto &v = vertexData[i];
        Vertex vertex;
        vertex.position = temp_vertices[v.x - 1];
        if (!temp_texCoords.empty()) {
          vertex.textCoord = temp_texCoords[v.y - 1];
        }
        if (!temp_normals.empty()) {
          vertex.normal = temp_normals[v.z - 1];
        }

        // Apply the current material to the vertex if one is set
        if (!currentMaterial.name.empty()) {
          vertex.color =
              glm::vec3(currentMaterial.diffuse[0], currentMaterial.diffuse[1],
                        currentMaterial.diffuse[2]);
        }

        vertices.push_back(vertex);
      }
    }
  }
  objFile.close();

  for (const auto &vertex : vertices) {
    std::cout << "Vertex Position: " << vertex.position.x << ", "
              << vertex.position.y << ", " << vertex.position.z << std::endl;
    std::cout << "Texture Coord: " << vertex.textCoord.x << ", "
              << vertex.textCoord.y << std::endl;
    std::cout << "Normal: " << vertex.normal.x << ", " << vertex.normal.y
              << ", " << vertex.normal.z << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }

  setupBuffers();
}

// Cleans up OpenGL resources associated with the model.
OBJModel::~OBJModel() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void OBJModel::printUVs() const {
  for (const Vertex &vertex : vertices) {
    std::cout << "UV: (" << vertex.textCoord.x << ", " << vertex.textCoord.y
              << ")" << std::endl;
  }
}
