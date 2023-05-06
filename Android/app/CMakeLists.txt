# This will set the minimum version of CMake required to generate the projects
# CMakeLists has its own language and features and as more and more versions are 
# added it will include new keywords and options
cmake_minimum_required(VERSION 3.4.1)

# This will give our project a name
set(project_name Sample3D)
project(${project_name})

# preprocessor definitions
ADD_DEFINITIONS (-DANDROID_VERSION)
ADD_DEFINITIONS (-DPLATFORM_ANDROID=1)
ADD_DEFINITIONS (-DANDROID_ABI=arm64-v8a)	# output for diff. mobile CPUs
# ADD_DEFINITIONS (-DANDROID_ABI=x86_64)
# ADD_DEFINITIONS (-DANDROID_ABI=armeabi-v7a)
# ADD_DEFINITIONS (-DANDROID_ABI=x86)

# include directories
# eg. include <directories/myfile.h>
set(android_includes_dir includes)
set(ndk_helper_dir ${android_includes_dir}/ndk_helper)
set(shared_includes_dir ../../Shared/includes)
set(shared_src_dir ../../Shared/source)
set(android_src_dir src/main/cpp)
set(project_dir ../../Projects/${project_name})

# build native_app_glue as a static lib (Android includes it as source code only, must built into a .so lib)
add_library(app-glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
		)
add_library(cpufeatures STATIC
        ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
		)

# Build dependency lib to overwrite definitions for subsequent libs/cpp
add_library(dependency-lib
        STATIC
        ${android_includes_dir}/Android_dep/Android_Dependencies.cpp
        )
target_include_directories(dependency-lib
		PRIVATE
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${ANDROID_NDK}/sources/android/native_app_glue
		)
target_link_libraries(
        dependency-lib
        app-glue
        cpufeatures
        android
		)

#add RUNTIME dependency files here for subsequent libs/cpp
SET(CMAKE_CXX_FLAGS "-include ../../../../includes/Android_dep/Android_Dependencies.h")
SET(CMAKE_C_FLAGS "-include ../../../../includes/Android_dep/Android_Dependencies.h")

# build the ndk-helper library (builds ndk_helper CMakeLists.txt)
add_subdirectory(${ndk_helper_dir} ndk_helper)

# group files into one var
set(SHARED_CAMERA_FILES
		${shared_src_dir}/camera/Camera.cpp
		${shared_src_dir}/camera/Camera.h
		)
set(SHARED_ENGINE_FILES
		${shared_src_dir}/engine/Engine.cpp
		${shared_src_dir}/engine/Engine.h
		)
set(SHARED_MATERIAL_FILES
		${shared_src_dir}/material/Material.cpp
		${shared_src_dir}/material/Material.h
        ${shared_src_dir}/material/MaterialBuilder.cpp
        ${shared_src_dir}/material/MaterialBuilder.h
        ${shared_src_dir}/material/MaterialColor.cpp
        ${shared_src_dir}/material/MaterialColor.h
        ${shared_src_dir}/material/MaterialLightMap.cpp
        ${shared_src_dir}/material/MaterialLightMap.h
		)
set(SHARED_MESH_FILES
		${shared_src_dir}/mesh/Mesh.cpp
		${shared_src_dir}/mesh/Mesh.h
        ${shared_src_dir}/mesh/MeshBuilder.cpp
        ${shared_src_dir}/mesh/MeshBuilder.h
		)
set(SHARED_OBJECT_FILES
		${shared_src_dir}/object/GameObject.cpp
		${shared_src_dir}/object/GameObject.h
		)
set(SHARED_SCENE_FILES
		${shared_src_dir}/scenes/Scene.cpp
		${shared_src_dir}/scenes/Scene.h
		${shared_src_dir}/scenes/ProjectSelector.h
		)
set(SHARED_SHADER_FILES
		${shared_src_dir}/shader/Shader.cpp
		${shared_src_dir}/shader/Shader.h
        ${shared_src_dir}/shader/ShaderManager.cpp
        ${shared_src_dir}/shader/ShaderManager.h
		)
set(SHARED_TEXTURE_FILES
		${shared_src_dir}/texture/TextureBuilder.cpp
		${shared_src_dir}/texture/TextureBuilder.h
		)
set(SHARED_THREAD_FILES
		${shared_src_dir}/thread/ThreadPoolBase.cpp
		${shared_src_dir}/thread/ThreadPoolBase.h
		)
set(SHARED_TOP_FILES
		${shared_src_dir}/top/Enums.h
		${shared_src_dir}/top/Includes.h
		${shared_src_dir}/top/Utility.cpp
		${shared_src_dir}/top/Utility.h
		)
SET(PLATFORM_FILES
        ${android_src_dir}/AndroidUtils.cpp
        ${android_src_dir}/OSEngine.cpp
        ${android_src_dir}/stb_image.cpp
        ${android_src_dir}/Main.cpp
		)
set(PROJECT_FILES
		${project_dir}/ProjectSelector.cpp
		${project_dir}/SceneSample3D.cpp
		${project_dir}/SceneSample3D.h
		)

# Create and name a library, set it as either STATIC
# or SHARED, and provide the relative paths to it's source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
add_library(${project_name} SHARED
		${SHARED_CAMERA_FILES}
		${SHARED_ENGINE_FILES}
		${SHARED_MATERIAL_FILES}
		${SHARED_MESH_FILES}
		${SHARED_OBJECT_FILES}
		${SHARED_SCENE_FILES}
		${SHARED_SHADER_FILES}
		${SHARED_TEXTURE_FILES}
		${SHARED_THREAD_FILES}
		${SHARED_TOP_FILES}
		${PLATFORM_FILES}
		${PROJECT_FILES}
		)

# link the include directories
target_include_directories(${project_name} PRIVATE
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${ANDROID_NDK}/sources/android/native_app_glue
        ${shared_src_dir}
        ${shared_includes_dir}
        ${shared_includes_dir}/glm
        ${android_includes_dir}
        ${ndk_helper_dir}
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library(log-lib
        log
		)

# link libs to the program
target_link_libraries(${project_name}
        android
        cpufeatures
        app-glue
        atomic
        EGL
        GLESv3
        ndk-helper
        dependency-lib
        ${log-lib}
		)