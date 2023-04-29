#include "OSEngine.h"
OSEngine* OSEngine::osEngine = NULL;
android_app* OSEngine::app = NULL;
std::vector<int32_t> OSEngine::pointerList;
glm::vec2 OSEngine::pointerPosList[MAX_TOUCH_EVENTS];
glm::vec2 OSEngine::pointerPos;  //shared
float OSEngine::new_cameraWidth = 0.f;
pthread_mutex_t OSEngine::mutex_;

/*******************************************************************************************************************
init Android system: called in main function
*******************************************************************************************************************/
void OSEngine::InitAndroid(android_app* app)
{
    this->app = app;
    initialized_resources_ = false;
    has_focus_ = false;
    gl_context_ = ndk_helper::GLContext::GetInstance();
}

/*******************************************************************************************************************
create display: called by Android system
*******************************************************************************************************************/
void OSEngine::createDisplay()
{
    //Display------------------------------------------------------------------//
    if (!initialized_resources_)
    {
        gl_context_->Init(app->window);
        windowsWidth = gl_context_->GetScreenWidth();
        windowsHeight = gl_context_->GetScreenHeight();
        X_to_Y_ratio = windowsWidth / windowsHeight;

        // assume width always more than height
        // we want width to match the scren ratio
        if(windowsWidth > windowsHeight) {
            // new_cameraWidth = Screen::CAMERA_HEIGHT * X_to_Y_ratio;
        }

        Init();

        initialized_resources_ = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS != gl_context_->Resume(app->window))
        {
            Init();
        }
    }

    // Note that screen size might have been changed RATIO: 4:3
    /* float game_ScreenWidth = Screen::SCREEN_HEIGHT * Screen::CAM_X_TO_Y_RATIO; //4:3
    float excessWidth = Screen::SCREEN_WIDTH - game_ScreenWidth;

    glViewport(excessWidth * 0.5f,
               0.f, game_ScreenWidth, gl_context_->GetScreenHeight());
    CU::view.ClearScreen();*/

    glViewport(0, 0, windowsWidth, windowsHeight);

    // reset timer to start now
    mFrameClock.Reset();
}

/*******************************************************************************************************************
init functions
*******************************************************************************************************************/
void OSEngine::initOpenGLSettings()
{
    // opengl rendering configurations
    // depth ordering on OpenGL
    glEnable(GL_DEPTH_TEST);
    // alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // cull face (discard unseen faces, save 50% on fragment shader runs)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

void OSEngine::initInput()
{

}

/*******************************************************************************************************************
constructors
*******************************************************************************************************************/
OSEngine::OSEngine()
{
    deltaTime = 0.f;
    mFrameClock.SetMaxDelta(1.0f / 60.f);  //60fps (input is milliseconds)
    pthread_mutex_init(&mutex_, NULL);

    for (int i = 0; i < INPUT_TOTAL; ++i)
    {
        inputList[i] = false;
    }
}

OSEngine::~OSEngine()
{
    // delete all instances
    delete MaterialBuilder::instance();
    delete TextureBuilder::instance();
    delete MeshBuilder::instance();
    delete ShaderManager::instance();
    delete Engine::instance();
    terminateOpenGL();
    pthread_mutex_destroy(&mutex_);
}

OSEngine* OSEngine::instance()
{
    if (!osEngine)
    {
        osEngine = new OSEngine();
    }
    return osEngine;
}

/*******************************************************************************************************************
init: called after display created
*******************************************************************************************************************/
void OSEngine::Init()
{
    initOpenGLSettings();
    initInput();
    Engine::instance()->Init();
}

/*******************************************************************************************************************
render loop: NOTE called every frame by run()
*******************************************************************************************************************/
void OSEngine::renderLoop()
{
    // input (check input per frame not applicable in Android)
    // processInput(window);

    // engine update
    Engine::instance()->Update(inputList, deltaTime);
    Engine::instance()->Draw();

    // read: double buffers to prevent flickering issues due to physical constraints of drawing a buffer to screen
    // resulting in flickering
    swapBuffers();
}

void OSEngine::swapBuffers()
{
    gl_context_->Swap();
}

/*******************************************************************************************************************
render loop: called by system
*******************************************************************************************************************/
void OSEngine::Run()
{
    // loop waiting for stuff to do.
    while (1) {

        // Read all pending events.
        int id;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(IsReady() ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(app, source);
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0) {

                TermDisplay();
                return;
            }
        }

        if (IsReady()) {

            //calculate dt----------------------------------------//
            deltaTime = mFrameClock.ReadDelta();
            // CU::fps = (float)(1.f / CU::dt);

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            renderLoop();
        }
    }
}

/*******************************************************************************************************************
system callbacks
*******************************************************************************************************************/
void OSEngine::engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (OSEngine::instance()->app->window != NULL) {
                OSEngine::instance()->createDisplay();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            OSEngine::instance()->TermDisplay();
            OSEngine::instance()->has_focus_ = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            OSEngine::instance()->has_focus_ = true;
            break;
        case APP_CMD_LOST_FOCUS:
            // Also stop animating.
            OSEngine::instance()->has_focus_ = false;
            break;
        case APP_CMD_LOW_MEMORY:
            //Free up GL resources
            OSEngine::instance()->TrimMemory();
            break;
    }
}

int32_t OSEngine::engine_handle_input(struct android_app* app, AInputEvent* event)
{
    return 0;
}

/*******************************************************************************************************************
exit
*******************************************************************************************************************/
void OSEngine::terminateOpenGL()
{
    TermDisplay();
}

/*******************************************************************************************************************
system functions
*******************************************************************************************************************/
bool OSEngine::IsReady()
{
    if (has_focus_)
        return true;
    return false;
}

void OSEngine::TermDisplay()
{
    gl_context_->Suspend();
}

void OSEngine::TrimMemory()
{
    gl_context_->Invalidate();
}

const android_app* OSEngine::GetApp()
{
    return app;
}