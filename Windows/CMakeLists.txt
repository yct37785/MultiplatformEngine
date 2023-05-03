# This will set the minimum version of CMake required to generate the projects
# CMakeLists has its own language and features and as more and more versions are 
# added it will include new keywords and options
cmake_minimum_required(VERSION 3.4.1)

# This will give our project a name
set(project_name Multiplatform)
project(${project_name})

# preprocessor definitions
ADD_DEFINITIONS(-DPLATFORM_WINDOWS=1)

# VS directory
set(vs_project_dir ${project_name})

# include directories
# eg. include <directories/myfile.h>
set(windows_includes_dir includes)
set(shared_includes_dir ../Shared/includes)
set(shared_src_dir ../Shared/source)
set(project_dir ../Projects/Sample3D_Project)

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
		src/ctpl_stl.h
		src/glad.c
		src/main.cpp
		src/OSEngine.cpp
		src/OSEngine.h
		src/stb_image.cpp
		)
set(PROJECT_FILES
        ${project_dir}/SceneSample3D.cpp
        ${project_dir}/SceneSample3D.h
        ${project_dir}/ProjectSelector.cpp
		)

# set VS filters
source_group("Camera" FILES ${SHARED_CAMERA_FILES})
source_group("Engine" FILES ${SHARED_ENGINE_FILES})
source_group("Material" FILES ${SHARED_MATERIAL_FILES})
source_group("Mesh" FILES ${SHARED_MESH_FILES})
source_group("Object" FILES ${SHARED_OBJECT_FILES})
source_group("Scene" FILES ${SHARED_SCENE_FILES})
source_group("Shader" FILES ${SHARED_SHADER_FILES})
source_group("Texture" FILES ${SHARED_TEXTURE_FILES})
source_group("Thread" FILES ${SHARED_THREAD_FILES})
source_group("Top" FILES ${SHARED_TOP_FILES})
source_group("Platform" FILES ${PLATFORM_FILES})
source_group("Project" FILES ${PROJECT_FILES})

# generate executable for this project
add_executable(${project_name} 
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
        ${windows_includes_dir}
		${shared_includes_dir}
		${shared_src_dir}
        )

# retrieve path vars of non pre-built/system libs
find_library(glfw3d-lib NAMES glfw3d.lib
		PATHS
        libs/GLFW
		)

# link libs to the program
target_link_libraries(${project_name}
		opengl32
		${glfw3d-lib}
        )

# startup project
set_property(DIRECTORY ${vs_project_dir} PROPERTY VS_STARTUP_PROJECT ${project_name})