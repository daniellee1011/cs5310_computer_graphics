#include "PPM.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
  std::ifstream inFile(fileName);
  if (!inFile.is_open()) {
    throw std::invalid_argument("File open failed!");
  }

  std::string line;
  std::getline(inFile, line);
  if (line != "P3") {
    throw std::invalid_argument("Unsupported PPM format or missing P3 header!");
  }

  // Skip comments
  while (std::getline(inFile, line)) {
    if (line[0] == '#')
      continue;
    std::stringstream dimensionsStream(line);
    dimensionsStream >> m_width >> m_height;
    break;
  }

  int m_maxColorValue;
  inFile >> m_maxColorValue;
  if (m_maxColorValue != 255) {
    throw std::invalid_argument("Unsupported max color value!");
  }

  // Read pixel data
  int r, g, b;
  while (inFile >> r >> g >> b) {
    m_PixelData.push_back(static_cast<uint8_t>(r));
    m_PixelData.push_back(static_cast<uint8_t>(g));
    m_PixelData.push_back(static_cast<uint8_t>(b));
  }
}

bool PPM::createTexture() {
  if (m_textureID == 0 && !m_PixelData.empty()) {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, m_PixelData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      std::cerr << "OpenGL error during texture creation: " << err << std::endl;
      return false; // or handle the error as appropriate
    }
    return true;
  }
  return false;
}

void PPM::print() const {
  std::cout << "PPM Image:" << std::endl;
  std::cout << "Width: " << m_width << std::endl;
  std::cout << "Height: " << m_height << std::endl;
  std::cout << "Texture ID: " << m_textureID << std::endl;

  // Optionally, print out some pixel data
  std::cout << "Pixel Data (first 10 pixels): ";
  for (size_t i = 0; i < 30 && i < m_PixelData.size();
       i += 3) { // Print RGB for first 10 pixels
    std::cout << "(" << static_cast<int>(m_PixelData[i]) << ", "
              << static_cast<int>(m_PixelData[i + 1]) << ", "
              << static_cast<int>(m_PixelData[i + 2]) << ") ";
  }
  std::cout << std::endl;
}

// Destructor deletes(delete or delete[]) any memory that has been allocated
// or otherwise calls any 'shutdown' or 'destroy' routines for this deletion
// to occur.
PPM::~PPM() {
  if (m_textureID != 0) {
    glDeleteTextures(1, &m_textureID);
  }
}
