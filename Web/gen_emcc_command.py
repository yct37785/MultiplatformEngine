# gen emcc command for LearnOpenGL project
# auto retrieve all .cpp files
import os


def include_files_from_dir(path, nested_level, excluded_dir):
    file_list = []
    prepend = ''.join('../' for i in range(nested_level))
    for r, d, f in os.walk(path):
        if r in excluded_dir:
            print(r)
            continue
        for file in f:
            if file.endswith('cpp') or file.endswith('c'):
                file_list.append(prepend + os.path.join(r, file).replace("\\", "/"))
    return file_list


if __name__ == '__main__':
    cmd = 'em++ '
    # retrieve relative path and names of all .c or .cpp files
    file_list = []
    # param 1: relative to .py file
    # param 2: relative to [root]\Web\emsdk\upstream\emscripten
    file_list += include_files_from_dir('source', 3, [])
    file_list += include_files_from_dir('../Shared/source', 3, ['../Shared/source\scenes\obselete'])
    cmd += ' '.join(file_list)
    # note that relative dir is [root]\Web\emsdk\upstream\emscripten
    # include directories
    cmd += ' -I../../../../Shared/includes -I../../../../Shared/source -I../../../includes'
    # output, linkage and flags
    cmd += ' -O2 -o hello.html -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -std=c++17 -s TOTAL_MEMORY=268435456 -s LLD_REPORT_UNDEFINED'
    # preload textures
    textures = ['tile-floor.png', 'tile-floor-specular.png', 'wooden-crate-face.png', 'wooden-crate-face-specular.png']
    for i in range(len(textures)):
        cmd += ' --preload-file assets/assets/textures/' + textures[i]
    # preload shaders
    shaders = ['lightMap/vertexshader.cpp', 'lightMap/fragmentshader.cpp', 'color/vertexshader.cpp', 'color/fragmentshader.cpp']
    for i in range(len(shaders)):
        cmd += ' --preload-file assets/shaders/' + shaders[i]
    # write
    print(cmd)
    with open('emcc.txt', 'w') as f:
        f.write(cmd)