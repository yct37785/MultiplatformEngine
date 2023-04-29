/*
 * Created on 7/4/2021
 * Originally created on 29/1/2017
 */
#include "OSEngine.h"
#include <Android_dep/Android_Dependencies.h>

#define HELPER_CLASS_NAME \
  "com/example/multiplatform/helper/NDKHelper"  // Class name of helper function

// logs
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

void android_main( android_app* state )
{
        // Make sure glue isn't stripped.
        app_dummy();


        state->userData = OSEngine::instance();
        ndk_helper::JNIHelper::Init( state->activity, HELPER_CLASS_NAME );
        android_fopen_set_asset_manager(state->activity->assetManager);

        state->onAppCmd = OSEngine::engine_handle_cmd;
        state->onInputEvent = OSEngine::engine_handle_input;
        OSEngine::instance()->InitAndroid(state);


        OSEngine::instance()->Run();
}