#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Terrain.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h) {
  // Initialization flag
  bool success = true;
  // String to hold any errors that occur.
  std::stringstream errorStream;
  // The window we'll be rendering to
  m_window = NULL;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError()
                << "\n";
    success = false;
  } else {
    // Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    m_window = SDL_CreateWindow("Lab", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, w, h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Check if Window did not create.
    if (m_window == NULL) {
      errorStream << "Window could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
      success = false;
    }

    // Create an OpenGL Graphics Context
    m_openGLContext = SDL_GL_CreateContext(m_window);
    if (m_openGLContext == NULL) {
      errorStream << "OpenGL context could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
      success = false;
    }

    // Initialize GLAD Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
      errorStream << "Failed to iniitalize GLAD\n";
      success = false;
    }

    // Initialize OpenGL
    if (!InitGL()) {
      errorStream << "Unable to initialize OpenGL!\n";
      success = false;
    }
  }

  // If initialization did not work, then print out a list of errors in the
  // constructor.
  if (!success) {
    errorStream
        << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
    std::string errors = errorStream.str();
    SDL_Log("%s\n", errors.c_str());
  } else {
    SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, "
            "errors detected during initialization\n\n");
  }

  // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra
  // debug support!
  GetOpenGLVersionInfo();

  // Setup our Renderer
  m_renderer = new Renderer(w, h);
}

// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram() {
  if (m_renderer != nullptr) {
    delete m_renderer;
  }

  // Destroy window
  SDL_DestroyWindow(m_window);
  // Point m_window to NULL to ensure it points to nothing.
  m_window = nullptr;
  // Quit SDL subsystems
  SDL_Quit();
}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL() {
  // Success flag
  bool success = true;

  return success;
}

// ====================== Create the planets =============
// NOTE: I will admit it is a little lazy to have these as globals,
//       we could build on our ObjectManager structure to manage the
//       each object that is inserted and automatically create a 'SceneNode'
//       for it for example. Or otherwise, build a 'SceneTree' class that
//       manages creating nodes and setting their type.
//       (e.g. A good use of the Factory Design Pattern if you'd like to
//             invetigate further--but it is beyond the scope of this
//             assignment).

// Create the Moon
Object *sphere3;
SceneNode *Moon;

Object *sphere4;
SceneNode *Moon2;

Object *sphere6;
SceneNode *Moon3;

Object *sphere7;
SceneNode *Moon4;

Object *sphere9;
SceneNode *Moon5;

Object *sphere10;
SceneNode *Moon6;

// Create the Earth
Object *sphere2;
SceneNode *Earth;

Object *sphere5;
SceneNode *Earth2;

Object *sphere8;
SceneNode *Earth3;

// Create the Sun
Object *sphere;
SceneNode *Sun;

// Angle and speed of rotation for the Moon's orbit
float moonOrbitAngle = 0.0f;
const float moonOrbitSpeed = 0.05f;

// Angle and speed of rotation for the Earth's orbit
float earthOrbitAngle = 0.0f;
float earthOrbitSpeed = 0.02f;

float earth3OrbitAngle = 0.0f;
float earth3OrbitSpeed = 0.05f;

// ====================== Create the planets =============

// Loops forever!
void SDLGraphicsProgram::Loop() {

  // ================== Initialize the planets ===============
  static float rotate = 0.0f;

  // Create new geometry for Earth's Moon
  sphere3 = new Sphere();
  sphere3->LoadTexture("rock.ppm");
  // Create a new node using sphere3 as the geometry
  Moon = new SceneNode(sphere3);

  sphere4 = new Sphere();
  sphere4->LoadTexture("rock.ppm");
  Moon2 = new SceneNode(sphere4);

  sphere6 = new Sphere();
  sphere6->LoadTexture("rock.ppm");
  Moon3 = new SceneNode(sphere4);

  sphere7 = new Sphere();
  sphere7->LoadTexture("rock.ppm");
  Moon4 = new SceneNode(sphere4);

  sphere9 = new Sphere();
  sphere9->LoadTexture("rock.ppm");
  Moon5 = new SceneNode(sphere9);

  sphere10 = new Sphere();
  sphere10->LoadTexture("rock.ppm");
  Moon6 = new SceneNode(sphere10);

  // Create the Earth
  sphere2 = new Sphere();
  sphere2->LoadTexture("earth.ppm");
  Earth = new SceneNode(sphere2);

  sphere5 = new Sphere();
  sphere5->LoadTexture("earth.ppm");
  Earth2 = new SceneNode(sphere5);

  sphere8 = new Sphere();
  sphere8->LoadTexture("earth.ppm");
  Earth3 = new SceneNode(sphere8);

  // Create the Sun
  sphere = new Sphere();
  sphere->LoadTexture("sun.ppm");
  Sun = new SceneNode(sphere);

  // Render our scene starting from the sun.
  m_renderer->setRoot(Sun);
  // Make the Earth a child of the Sun
  Sun->AddChild(Earth);
  // Make the Moon a child of the Earth
  Earth->AddChild(Moon);
  Earth->AddChild(Moon2);

  Sun->AddChild(Earth2);
  Earth2->AddChild(Moon3);
  Earth2->AddChild(Moon4);

  Sun->AddChild(Earth3);
  Earth3->AddChild(Moon5);
  Earth3->AddChild(Moon6);

  // Set a default position for our camera
  m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f, 0.0f, 20.0f);

  // Main loop flag
  // If this is quit = 'true' then the program terminates.
  bool quit = false;
  // Event handler that handles various events in SDL
  // that are related to input and output
  SDL_Event e;
  // Enable text input
  SDL_StartTextInput();

  // Set the camera speed for how fast we move.
  float cameraSpeed = 5.0f;

  // While application is running
  while (!quit) {

    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      // User posts an event to quit
      // An example is hitting the "x" in the corner of the window.
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      // Handle keyboard input for the camera class
      if (e.type == SDL_MOUSEMOTION) {
        // Handle mouse movements
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        //              m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
      }
      switch (e.type) {
      // Handle keyboard presses
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          m_renderer->GetCamera(0)->MoveLeft(cameraSpeed);
          break;
        case SDLK_RIGHT:
          m_renderer->GetCamera(0)->MoveRight(cameraSpeed);
          break;
        case SDLK_UP:
          m_renderer->GetCamera(0)->MoveForward(cameraSpeed);
          break;
        case SDLK_DOWN:
          m_renderer->GetCamera(0)->MoveBackward(cameraSpeed);
          break;
        case SDLK_RSHIFT:
          m_renderer->GetCamera(0)->MoveUp(cameraSpeed);
          break;
        case SDLK_RCTRL:
          m_renderer->GetCamera(0)->MoveDown(cameraSpeed);
          break;
        }
        break;
      }
    } // End SDL_PollEvent loop.
      // ================== Use the planets ===============
      // TODO:
      //      After the planets have been created, and the hierarchy
      //      has been made, you can transform them.
      //      Note, you'll need to setup the scene graph world/local
      //      transformation correctly first before the planets are shown.
      //
      //      The 'Sun' for example will be the only object shown initially
      //      since the rest of the planets are children (or grandchildren)
      //      of the Sun.

    float sunRotationAngle = 0.01f;
    float earthRotationAngle = 0.02f;
    float moonRotationAngle = 0.03f;
    float moonOrbitRadius = 10.0f;

    // Earth's orbit radius
    float earthOrbitRadius = 10.0f;
    float earthX = earthOrbitRadius * cos(earthOrbitAngle);
    float earthZ = earthOrbitRadius * sin(earthOrbitAngle);

    // Rotating around the Y-axis
    Sun->GetLocalTransform().Rotate(sunRotationAngle, 0.0f, 1.0f, 0.0f);

    Earth->GetLocalTransform().LoadIdentity();
    Earth->GetLocalTransform().Scale(0.5f, 0.5f, 0.5f);
    Earth->GetLocalTransform().Translate(earthX, 0.0f, earthZ);
    Earth->GetLocalTransform().Rotate(earthRotationAngle, 0.0f, 1.0f, 0.0f);

    // Increment the Moon's orbit angle
    moonOrbitAngle += moonOrbitSpeed;

    // Calculate the Moon's position in its orbit
    float moonX = moonOrbitRadius * cos(moonOrbitAngle);
    float moonZ = moonOrbitRadius * sin(moonOrbitAngle);

    Moon->GetLocalTransform().LoadIdentity();
    Moon->GetLocalTransform().Scale(0.2f, 0.2f, 0.2f);
    Moon->GetLocalTransform().Translate(moonX, 0.0f, moonZ);
    Moon->GetLocalTransform().Rotate(moonRotationAngle, 0.0f, 1.0f, 0.0f);

    float moon2Y = moonOrbitRadius * cos(moonOrbitAngle);
    float moon2Z = moonOrbitRadius * sin(moonOrbitAngle);

    Moon2->GetLocalTransform().LoadIdentity();
    Moon2->GetLocalTransform().Scale(0.2f, 0.2f, 0.2f);
    Moon2->GetLocalTransform().Translate(0.0f, moonX, moonZ);
    Moon2->GetLocalTransform().Rotate(moonRotationAngle, 0.0f, 0.0f, 1.0f);

    // 2nd Set
    Earth2->GetLocalTransform().LoadIdentity();
    Earth2->GetLocalTransform().Scale(0.7f, 0.7f, 0.7f);
    Earth2->GetLocalTransform().Translate(-8.0f, 0.0f, -8.0f);
    Earth2->GetLocalTransform().Rotate(-earthRotationAngle, 0.0f, 1.0f, 0.0f);

    float moon3X = moonOrbitRadius * cos(moonOrbitAngle);
    float moon3Z = moonOrbitRadius * sin(moonOrbitAngle);

    Moon3->GetLocalTransform().LoadIdentity();
    Moon3->GetLocalTransform().Scale(0.2f, 0.2f, 0.2f);
    Moon3->GetLocalTransform().Translate(moon3X, 0.0f, moon3Z);
    Moon3->GetLocalTransform().Rotate(moonRotationAngle, 0.0f, 1.0f, 0.0f);

    float moon4Y = moonOrbitRadius * cos(moonOrbitAngle);
    float moon4Z = moonOrbitRadius * sin(moonOrbitAngle);

    Moon4->GetLocalTransform().LoadIdentity();
    Moon4->GetLocalTransform().Scale(0.2f, 0.2f, 0.2f);
    Moon4->GetLocalTransform().Translate(0.0f, moon4Y, moon4Z);
    Moon4->GetLocalTransform().Rotate(moonRotationAngle, 0.0f, 0.0f, 1.0f);

    // 3rd Set
    earth3OrbitAngle -= earth3OrbitSpeed;
    earthOrbitRadius = 15.0f;
    float earth3X = earthOrbitRadius * cos(earth3OrbitAngle);
    float earth3Z = earthOrbitRadius * sin(earth3OrbitAngle);

    Earth3->GetLocalTransform().LoadIdentity();
    Earth3->GetLocalTransform().Scale(0.9f, 0.9f, 0.9f);
    Earth3->GetLocalTransform().Translate(earth3X, 0.0f, earth3Z);
    Earth3->GetLocalTransform().Rotate(-earthRotationAngle, 0.0f, 1.0f, 0.0f);

    float moon5X = moonOrbitRadius * cos(moonOrbitAngle);
    float moon5Z = moonOrbitRadius * sin(moonOrbitAngle);

    Moon5->GetLocalTransform().LoadIdentity();
    Moon5->GetLocalTransform().Scale(0.2f, 0.2f, 0.2f);
    Moon5->GetLocalTransform().Translate(moon5X, 0.0f, moon5Z);
    Moon5->GetLocalTransform().Rotate(moonRotationAngle, 0.0f, 1.0f, 0.0f);

    float moon6Y = moonOrbitRadius * cos(moonOrbitAngle);
    float moon6Z = moonOrbitRadius * sin(moonOrbitAngle);

    Moon6->GetLocalTransform().LoadIdentity();
    Moon6->GetLocalTransform().Scale(0.2f, 0.2f, 0.2f);
    Moon6->GetLocalTransform().Translate(0.0f, moon6Y, moon6Z);
    Moon6->GetLocalTransform().Rotate(moonRotationAngle, 0.0f, 0.0f, 1.0f);

    // Update our scene through our renderer
    m_renderer->Update();
    // Render our scene using our selected renderer
    m_renderer->Render();
    // Delay to slow things down just a bit!
    SDL_Delay(35); // TODO: You can change this or implement a frame
                   // independent movement method if you like.
    // Update screen of our specified window
    SDL_GL_SwapWindow(GetSDLWindow());
  }
  // Disable text input
  SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window *SDLGraphicsProgram::GetSDLWindow() { return m_window; }

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo() {
  SDL_Log(
      "(Note: If you have two GPU's, make sure the correct one is selected)");
  SDL_Log("Vendor: %s", (const char *)glGetString(GL_VENDOR));
  SDL_Log("Renderer: %s", (const char *)glGetString(GL_RENDERER));
  SDL_Log("Version: %s", (const char *)glGetString(GL_VERSION));
  SDL_Log("Shading language: %s",
          (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
