# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Declares and names the project.
project("Multiplatform")

# build native_app_glue as a static lib (Android includes it as source code only, must built into a .so lib)
add_library(app-glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(cpufeatures STATIC
        ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)

# Build dependency lib to overwrite definitions for subsequent libs/cpp----------------------------//
add_library( # Sets the name of the library.
        dependency-lib

        # Sets the library as a shared library.
        STATIC

        includes/Android_dep/Android_Dependencies.cpp
        )

target_include_directories(dependency-lib PRIVATE
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries( # Specifies the target library.
        dependency-lib
        app-glue
        cpufeatures
        android)

#add RUNTIME dependency files here for subsequent libs/cpp
SET(CMAKE_CXX_FLAGS "-include ../../../../includes/Android_dep/Android_Dependencies.h")
SET(CMAKE_C_FLAGS "-include ../../../../includes/Android_dep/Android_Dependencies.h")

# build the ndk-helper library--------------------------------------------------------------------//
set(ndk_helper_dir includes/ndk_helper)	#start location is location of CMakeLists.txt
add_subdirectory(${ndk_helper_dir} ndk_helper)

# build the native library--------------------------------------------------------------------//
ADD_DEFINITIONS (-DANDROID_VERSION)
ADD_DEFINITIONS (-DPLATFORM_ANDROID=1)
# output for diff. mobile CPUs
ADD_DEFINITIONS (-DANDROID_ABI=arm64-v8a)
# ADD_DEFINITIONS (-DANDROID_ABI=x86_64)
# ADD_DEFINITIONS (-DANDROID_ABI=armeabi-v7a)
# ADD_DEFINITIONS (-DANDROID_ABI=x86)

# Create and name a library, set it as either STATIC
# or SHARED, and provide the relative paths to it's source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(shared_src_dir ../../Shared/source)
set(shared_includes_dir ../../Shared/includes)

add_library( # Sets the name of the library.
        multiplatform-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        # shared by all platforms (with defines to differentiate btw platforms)
        ${shared_src_dir}/top/Utility.cpp

        # platform agnostic
        ${shared_src_dir}/camera/Camera.cpp
        ${shared_src_dir}/engine/Engine.cpp
        ${shared_src_dir}/material/Material.cpp
        ${shared_src_dir}/material/MaterialBuilder.cpp
        ${shared_src_dir}/material/MaterialColor.cpp
        ${shared_src_dir}/material/MaterialLightMap.cpp
        ${shared_src_dir}/mesh/Mesh.cpp
        ${shared_src_dir}/mesh/MeshBuilder.cpp
        ${shared_src_dir}/object/GameObject.cpp
        ${shared_src_dir}/scenes/Scene.cpp
        ${shared_src_dir}/scenes/SceneLightMap.cpp
        ${shared_src_dir}/shader/Shader.cpp
        ${shared_src_dir}/shader/ShaderManager.cpp
        ${shared_src_dir}/texture/TextureBuilder.cpp

        # platform dependent files
        src/main/cpp/AndroidUtils.cpp
        src/main/cpp/OSEngine.cpp
        src/main/cpp/stb_image.cpp
        src/main/cpp/Main.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# we are building everything into .so libs, so just like in VS we need to target the include directories
# of said libs' header files
target_include_directories(multiplatform-lib PRIVATE
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${ANDROID_NDK}/sources/android/native_app_glue
        ${shared_src_dir}
        ${shared_includes_dir}
        ${shared_includes_dir}/glm
        includes
        ${ndk_helper_dir}
        )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        multiplatform-lib

        # Links the target library to the log library
        # included in the NDK.
        android
        cpufeatures
        app-glue
        atomic
        EGL
        GLESv3
        ndk-helper
        dependency-lib
        ${log-lib} )	# log-lib is special, others no need ${}