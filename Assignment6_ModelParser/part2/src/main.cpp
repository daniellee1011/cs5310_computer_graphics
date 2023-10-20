/* Compilation on Linux:
 g++ -std=c++17 ./src/*.cpp -o prog -I ./include/ -I./../common/thirdparty/
 -lSDL2 -ldl
*/

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

// Our libraries
#include "Camera.hpp"
#include "OBJModel.hpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.

// Screen Dimensions
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

// Main loop flag
bool gQuit = false; // If this is quit = 'true' then the program terminates.

// shader
// The following stores the a unique id for the graphics pipeline
// program object that will be used for our OpenGL draw calls.
GLuint gGraphicsPipelineShaderProgram = 0;

// Camera
Camera gCamera;

// Draw wireframe mode
GLenum gPolygonMode = GL_LINE;

// Obj file
OBJModel model;
std::string filepath;

// ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^

// vvvvvvvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvv
static void GLClearAllErrors() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

// Returns true if we have an error
static bool GLCheckErrorStatus(const char *function, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL Error:" << error << "\tLine: " << line
              << "\tfunction: " << function << std::endl;
    return true;
  }
  return false;
}

#define GLCheck(x)                                                             \
  GLClearAllErrors();                                                          \
  x;                                                                           \
  GLCheckErrorStatus(#x, __LINE__);
// ^^^^^^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^

/**
 * LoadShaderAsString takes a filepath as an argument and will read line by line
 * a file and return a string that is meant to be compiled at runtime for a
 * vertex, fragment, geometry, tesselation, or compute shader. e.g.
 *       LoadShaderAsString("./shaders/filepath");
 *
 * @param filename Path to the shader file
 * @return Entire file stored as a single string
 */
std::string LoadShaderAsString(const std::string &filename) {
  // Resulting shader program loaded as a single string
  std::string result = "";

  std::string line = "";
  std::ifstream myFile(filename.c_str());

  if (myFile.is_open()) {
    while (std::getline(myFile, line)) {
      result += line + '\n';
    }
    myFile.close();
  }

  return result;
}

/**
 * CompileShader will compile any valid vertex, fragment, geometry, tesselation,
 *or compute shader. e.g. Compile a vertex shader:
 *CompileShader(GL_VERTEX_SHADER, vertexShaderSource); Compile a fragment
 *shader: 	CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
 *
 * @param type We use the 'type' field to determine which shader we are going to
 *compile.
 * @param source : The shader source code.
 * @return id of the shaderObject
 */
GLuint CompileShader(GLuint type, const std::string &source) {
  // Compile our shaders
  GLuint shaderObject;

  // Based on the type passed in, we create a shader object specifically for
  // that type.
  if (type == GL_VERTEX_SHADER) {
    shaderObject = glCreateShader(GL_VERTEX_SHADER);
  } else if (type == GL_FRAGMENT_SHADER) {
    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  }

  const char *src = source.c_str();
  // The source of our shader
  glShaderSource(shaderObject, 1, &src, nullptr);
  // Now compile our shader
  glCompileShader(shaderObject);

  // Retrieve the result of our compilation
  int result;
  // Our goal with glGetShaderiv is to retrieve the compilation status
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
    char *errorMessages = new char[length]; // Could also use alloca here.
    glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

    if (type == GL_VERTEX_SHADER) {
      std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n"
                << errorMessages << "\n";
    } else if (type == GL_FRAGMENT_SHADER) {
      std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n"
                << errorMessages << "\n";
    }
    // Reclaim our memory
    delete[] errorMessages;

    // Delete our broken shader
    glDeleteShader(shaderObject);

    return 0;
  }

  return shaderObject;
}

/**
 * Creates a graphics program object (i.e. graphics pipeline) with a Vertex
 * Shader and a Fragment Shader
 *
 * @param vertexShaderSource Vertex source code as a string
 * @param fragmentShaderSource Fragment shader source code as a string
 * @return id of the program Object
 */
GLuint CreateShaderProgram(const std::string &vertexShaderSource,
                           const std::string &fragmentShaderSource) {

  // Create a new program object
  GLuint programObject = glCreateProgram();

  // Compile our shaders
  GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint myFragmentShader =
      CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  // Link our two shader programs together.
  // Consider this the equivalent of taking two .cpp files, and linking them
  // into one executable file.
  glAttachShader(programObject, myVertexShader);
  glAttachShader(programObject, myFragmentShader);
  glLinkProgram(programObject);

  // Validate our program
  glValidateProgram(programObject);

  // Once our final program Object has been created, we can
  // detach and then delete our individual shaders.
  glDetachShader(programObject, myVertexShader);
  glDetachShader(programObject, myFragmentShader);
  // Delete the individual shaders once we are done
  glDeleteShader(myVertexShader);
  glDeleteShader(myFragmentShader);

  return programObject;
}

/**
 * Create the graphics pipeline
 *
 * @return void
 */
void CreateGraphicsPipeline() {

  std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
  std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

  gGraphicsPipelineShaderProgram =
      CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

/**
 * Initialization of the graphics application. Typically this will involve
 * setting up a window and the OpenGL Context (with the appropriate version)
 *
 * @return void
 */
void InitializeProgram() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError()
              << "\n";
    exit(1);
  }

  // Setup the OpenGL Context
  // Use OpenGL 4.1 core or greater
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // We want to request a double buffer for smooth updating.
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // Create an application window using OpenGL that supports SDL
  gGraphicsApplicationWindow = SDL_CreateWindow(
      "OpenGL First Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  // Check if Window did not create.
  if (gGraphicsApplicationWindow == nullptr) {
    std::cout << "Window could not be created! SDL Error: " << SDL_GetError()
              << "\n";
    exit(1);
  }

  // Create an OpenGL Graphics Context
  gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
  if (gOpenGLContext == nullptr) {
    std::cout << "OpenGL context could not be created! SDL Error: "
              << SDL_GetError() << "\n";
    exit(1);
  }

  // Initialize GLAD Library
  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "glad did not initialize" << std::endl;
    exit(1);
  }
}

/**
 * Setup your geometry during the vertex specification step
 *
 * @return void
 */
void VertexSpecification() { model.loadModelFromFile(filepath); }

/**
 * PreDraw
 * Typically we will use this for setting some sort of 'state'
 * Note: some of the calls may take place at different stages (post-processing)
 * of the pipeline.
 * @return void
 */
void PreDraw() {
  // Disable depth test and face culling.
  glEnable(GL_DEPTH_TEST); // NOTE: Need to enable DEPTH Test
  glDisable(GL_CULL_FACE);

  // Set the polygon fill mode
  glPolygonMode(GL_FRONT_AND_BACK, gPolygonMode);

  // Initialize clear color
  // This is the background of the screen.
  glViewport(0, 0, gScreenWidth, gScreenHeight);
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);

  // Clear color buffer and Depth Buffer
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  // Use our shader
  glUseProgram(gGraphicsPipelineShaderProgram);

  // Model transformation by translating our object into world space
  glm::mat4 model =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
  glm::vec3 scaleFactor(4.0f, 4.0f, 4.0f);
  model = glm::scale(model, scaleFactor);

  // Send data to GPU
  GLint u_ModelMatrixLocation =
      glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
  if (u_ModelMatrixLocation >= 0) {
    glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  } else {
    std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
    exit(EXIT_FAILURE);
  }

  // Update the View Matrix
  GLint u_ViewMatrixLocation =
      glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
  if (u_ViewMatrixLocation >= 0) {
    glm::mat4 viewMatrix = gCamera.GetViewMatrix();
    glUniformMatrix4fv(u_ViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
  } else {
    std::cout << "Could not find u_ViewMatrix, maybe a mispelling?\n";
    exit(EXIT_FAILURE);
  }

  // Projection matrix (in perspective)
  glm::mat4 perspective =
      glm::perspective(glm::radians(45.0f),
                       (float)gScreenWidth / (float)gScreenHeight, 0.1f, 20.0f);
  // Send data to GPU
  GLint u_ProjectionLocation =
      glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");
  if (u_ProjectionLocation >= 0) {
    glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
  } else {
    std::cout << "Could not find u_Projection, maybe a mispelling?\n";
    exit(EXIT_FAILURE);
  }
}

/**
 * Draw
 * The render function gets called once per loop.
 * Typically this includes 'glDraw' related calls, and the relevant setup of
 * buffers for those calls.
 *
 * @return void
 */
void Draw() { model.render(); }

/**
 * Helper Function to get OpenGL Version Information
 *
 * @return void
 */
void getOpenGLVersionInfo() {
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << "\n";
}

/**
 * Function called in the Main application loop to handle user input
 *
 * @return void
 */
void Input() {
  // Two static variables to hold the mouse position
  static int mouseX = gScreenWidth / 2;
  static int mouseY = gScreenHeight / 2;

  // Event handler that handles various events in SDL
  // that are related to input and output
  SDL_Event e;
  // Handle events on queue
  while (SDL_PollEvent(&e) != 0) {
    // If users posts an event to quit
    // An example is hitting the "x" in the corner of the window.
    if (e.type == SDL_QUIT) {
      std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
      gQuit = true;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
      std::cout << "'q': Goodbye! (Leaving MainApplicationLoop())" << std::endl;
      gQuit = true;
    }
  }

  // Retrieve keyboard state
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_W]) {
    SDL_Delay(250); // This is hacky in the name of simplicity,
                    // but we just delay the
                    // system by a few milli-seconds to process the
                    // keyboard input once at a time.
    if (gPolygonMode == GL_FILL) {
      gPolygonMode = GL_LINE;
    } else {
      gPolygonMode = GL_FILL;
    }
  }
}

/**
 * Main Application Loop
 * This is an infinite loop in our graphics application
 *
 * @return void
 */
void MainLoop() {

  // Little trick to map mouse to center of screen always.
  // Useful for handling 'mouselook'
  // This works because we effectively 're-center' our mouse at the start
  // of every frame prior to detecting any mouse motion.
  SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth / 2,
                        gScreenHeight / 2);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  // While application is running
  while (!gQuit) {
    // Handle Input
    Input();
    // Setup anything (i.e. OpenGL State) that needs to take
    // place before draw calls
    PreDraw();
    // Draw Calls in OpenGL
    // When we 'draw' in OpenGL, this activates the graphics pipeline.
    // i.e. when we use glDrawElements or glDrawArrays,
    //      The pipeline that is utilized is whatever 'glUseProgram' is
    //      currently binded.
    Draw();
    // Update screen of our specified window
    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
  }
}

/**
 * The last function called in the program
 * This functions responsibility is to destroy any global
 * objects in which we have create dmemory.
 *
 * @return void
 */
void CleanUp() {
  // Destroy our SDL2 Window
  SDL_DestroyWindow(gGraphicsApplicationWindow);
  gGraphicsApplicationWindow = nullptr;

  // Delete our Graphics pipeline
  glDeleteProgram(gGraphicsPipelineShaderProgram);

  // Quit SDL subsystems
  SDL_Quit();
}

/**
 * The entry point into our C++ programs.
 *
 * @return program status
 */
int main(int argc, char *args[]) {
  std::cout << "Press 'q' to quit\n";

  // Check if the second argument is provided
  if (argc < 2) {
    std::cerr << "Error: No file path provided. Usage: " << args[0]
              << " <path_to_obj_file>\n";
    return 1; // Return an error code
  }
  filepath = args[1];

  // 1. Setup the graphics program
  InitializeProgram();

  // 2. Setup our geometry
  VertexSpecification();

  // 3. Create our graphics pipeline
  // 	- At a minimum, this means the vertex and fragment shader
  CreateGraphicsPipeline();

  // 4. Call the main application loop
  MainLoop();

  // 5. Call the cleanup function when our program terminates
  CleanUp();

  return 0;
}
