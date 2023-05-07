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

# include directories
# eg. include <directories/myfile.h>
set(android_includes_dir includes)
set(shared_includes_dir ../../Shared/includes)
set(shared_src_dir ../../Shared/source)
set(android_src_dir src/main/cpp)
set(project_dir ../../Projects/${project_name})

# Add the packages from AGDK
find_package(games-frame-pacing REQUIRED CONFIG)
find_package(games-performance-tuner REQUIRED CONFIG)
find_package(game-activity REQUIRED CONFIG)

# Export GameActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
		"${CMAKE_SHARED_LINKER_FLAGS} -u GameActivity_onCreate")

# Set common compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

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
        ${android_src_dir}/OSEngine2.cpp
        ${android_src_dir}/stb_image.cpp
        ${android_src_dir}/android_main.cpp
		${android_src_dir}/jni_util.cpp
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
        ${shared_src_dir}
        ${shared_includes_dir}
        ${shared_includes_dir}/glm
		${shared_includes_dir}/common
        ${android_includes_dir}
        )

# link libs to the program
target_link_libraries(${project_name}
        android
        atomic
        EGL
        GLESv3
        log
		games-frame-pacing::swappy_static
		games-performance-tuner::tuningfork_static
		game-activity::game-activity_static
		)