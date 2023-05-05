#pragma once
// common headers
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <time.h>       /* time */
#include <iterator>
#include <bitset>
#include <iomanip>
#include <random>
#include <map>
// Core C++, OpenGL, texture and any other platform specific libs
#ifdef PLATFORM_WINDOWS
// memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// OpenGL specific
#include <glad/glad.h>  // !include before GLFW!
#include <GLFW/glfw3.h>
#elif PLATFORM_ANDROID
// OpenGL ES and GL context
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <JNIHelper.h>
#include <GLContext.h>
#elif __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GL/gl.h>	// replaces GLAD
#include <GLFW/glfw3.h>
#endif
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// multi-threading
// #include "../thread/ThreadPoolBase.h"
// defines
#define PI 3.141592653589793
static int windowsWidth = 1280;
static int windowsHeight = 720;
#ifdef PLATFORM_WINDOWS
const static std::string assetsPath = "../../../shared/assets";
const static std::string shaderspath = "../../../shared/shaders";
#elif PLATFORM_ANDROID
const static std::string assetsPath = "assets";
const static std::string shaderspath = "shaders";
#elif __EMSCRIPTEN__
const static std::string assetsPath = "assets/assets";
const static std::string shaderspath = "assets/shaders";
#endif