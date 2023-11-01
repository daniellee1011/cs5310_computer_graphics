#ifndef OBJMODEL_HPP
#define OBJMODEL_HPP

// Third Party Libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// C++ Standard Template Library (STL)
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// The OBJModel class represents a 3D model loaded from an OBJ file.
class OBJModel {
public:
  // Default constructor. Initializes an empty model.
  OBJModel();
  // Parameterized constructor. Loads a model from the provided OBJ file path.
  OBJModel(const std::string &filepath);
  // Destructor. Cleans up OpenGL resources.
  ~OBJModel();

  // Renders the loaded model to the current OpenGL context.
  void render() const;
  // Loads a model from the provided OBJ file path.
  void loadModelFromFile(const std::string &filepath);

private:
  // Represents a single vertex with position and normal data.
  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
  };

  std::vector<Vertex> vertices;
  unsigned int vao;
  unsigned int vbo;

  // Initializes OpenGL buffers and attribute pointers.
  void setupBuffers();
};

#endif // OBJMODEL_HPP
