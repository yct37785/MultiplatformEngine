#ifndef OSENGINE_H
#define OSENGINE_H

#include <NDKHelper.h>
#include <engine/Engine.h>
#include "AndroidUtils.h"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

/**********************************************************************************************
Android version

Author: Tan Yie Cher
Date: 25/12/2016 (volatile)
MAJOR REVISION: 17/2/2017
USING NEW CLASS: 7/4/2021
/**********************************************************************************************/
class OSEngine
{
    // core
    bool inputList[INPUT_TOTAL];
    static pthread_mutex_t mutex_;
    static OSEngine* osEngine;
    static android_app* app;
	float deltaTime;

    // touchscreen input
    static const int MAX_TOUCH_EVENTS = 15;
    static std::vector<int32_t> pointerList; //store the unique pointer lists
    static glm::vec2 pointerPosList[MAX_TOUCH_EVENTS];
    static glm::vec2 pointerPos;  //shared

    // EGL/OpenGL ES
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    bool initialized_resources_;
    bool has_focus_;

    //display vars
    float X_to_Y_ratio; //display X to Y ratio (NOT GAME SCREEN!!)
    static float new_cameraWidth;  //store new cam. width instead of screen

    // frame clock -- it computes the deltas between successive frames
    DeltaClock mFrameClock;

    // GLFW window equivalent
    ndk_helper::GLContext* gl_context_;

private:
    OSEngine();

    // init OpenGL/glfw
	static void initOpenGLSettings();
    void initInput();

    // render loop
    void renderLoop();
    void swapBuffers();

    // exit
    void terminateOpenGL();

    //system functions
    void TermDisplay(); //terminate display
    void TrimMemory();
    bool IsReady();

public:
    ~OSEngine();
    static OSEngine* instance();

    // core
    void InitAndroid(android_app* app);
    void createDisplay();
    void Init();
    void Run();

    //callback
    static int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
    static void engine_handle_cmd(struct android_app* app, int32_t cmd);

    //get app Android instance
    static const android_app* GetApp();
};

#endif