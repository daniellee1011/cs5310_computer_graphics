#include "OBJModel.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Default constructor. Initializes an empty model.
OBJModel::OBJModel() {
  std::cout << "OBJModel default constructor: Nothing loaded yet" << std::endl;
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
                        (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Renders the loaded model using the previously set up OpenGL buffers.
void OBJModel::render() const {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);

  // Stop using our current graphics pipeline
  // Note: This is not necessary if we only have one graphics pipeline.
  glUseProgram(0);
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

  while (std::getline(objFile, line)) {
    std::istringstream ss(line);
    std::string prefix;
    ss >> prefix;

    if (prefix == "v") { // Parse vertices
      glm::vec3 vertex;
      ss >> vertex.x >> vertex.y >> vertex.z;
      temp_vertices.push_back(vertex);
    } else if (prefix == "vn") { // Parse normals
      glm::vec3 normal;
      ss >> normal.x >> normal.y >> normal.z;
      temp_normals.push_back(normal);
    } else if (prefix == "f") { // Parse faces. OBJ files use 1-based indexing,
                                // hence the '-1'.
      int v1, v2, v3, n1, n2, n3;
      char slash;
      ss >> v1 >> slash >> slash >> n1 >> v2 >> slash >> slash >> n2 >> v3 >>
          slash >> slash >> n3;

      vertices.push_back({temp_vertices[v1 - 1], temp_normals[n1 - 1]});
      vertices.push_back({temp_vertices[v2 - 1], temp_normals[n2 - 1]});
      vertices.push_back({temp_vertices[v3 - 1], temp_normals[n3 - 1]});
    }
  }
  objFile.close();

  setupBuffers();
}

// Cleans up OpenGL resources associated with the model.
OBJModel::~OBJModel() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}
