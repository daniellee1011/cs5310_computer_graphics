/** @file PPM.hpp
 *  @brief Class for working with PPM images
 *
 *  Class for working with P3 PPM images specifically.
 *
 *  @author your_name_here
 *  @bug No known bugs.
 */
#ifndef PPM_HPP
#define PPM_HPP

#include <cstdint>
#include <glad/glad.h>
#include <string>
#include <vector>

class PPM {
public:
  PPM() = default;
  PPM(std::string fileName);
  ~PPM();
  inline std::vector<uint8_t> pixelData() const { return m_PixelData; }
  inline int getWidth() const { return m_width; }
  inline int getHeight() const { return m_height; }
  inline bool isEmpty() const { return m_PixelData.empty(); }
  bool createTexture();
  inline GLuint getTextureID() const { return m_textureID; }
  void print() const;

private:
  std::vector<uint8_t> m_PixelData;
  int m_width{0};
  int m_height{0};
  GLuint m_textureID{0}; // The OpenGL texture ID
};

#endif
