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
  // Constructor loads a filename with the .ppm extension
  PPM(std::string fileName);
  // Destructor clears any memory that has been allocated
  ~PPM();
  // Saves a PPM Image to a new file.
  void savePPM(std::string outputFileName) const;
  void setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B);
  // Returns the raw pixel data in an array.
  inline std::vector<uint8_t> pixelData() const { return m_PixelData; }
  // Returns image width
  inline int getWidth() const { return m_width; }
  // Returns image height
  inline int getHeight() const { return m_height; }
  inline bool isEmpty() const { return m_PixelData.empty(); }
  bool createTexture();
  inline GLuint getTextureID() const { return m_textureID; }

private:
  // Store the raw pixel data here
  // Data is R,G,B format
  std::vector<uint8_t> m_PixelData;
  // Store width and height of image.
  int m_width{0};
  int m_height{0};
  GLuint m_textureID{0}; // The OpenGL texture ID
};

#endif
