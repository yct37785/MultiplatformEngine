import os
import shutil
import math
import re
project_dir = '../Projects'
template_dir = 'template'
sample_projects_dir = 'sample_projects'
windows_cmake_path = '../Windows/CMakeLists.txt'
windows_vs_path = '../Windows/Multiplatform'

def create_new_project(project_name):
    # copy over template
    new_dir = project_dir + '/' + project_name
    shutil.copytree(template_dir, new_dir)
    # for each .cpp/.h file in new dir
    for file in os.listdir(new_dir):
        file_path = os.path.join(new_dir, file)
        # rename all instances of SceneTemplate to Scene<project_name>
        with open(file_path, 'r') as f:
            file_content = f.read()
            file_content = file_content.replace("SceneTemplate", "Scene" + project_name)
        with open(file_path, 'w') as f:
            f.write(file_content)
        # rename SceneTemplate.cpp and .h
        new_file_name = file.replace("SceneTemplate", "Scene" + project_name)
        os.rename(file_path, os.path.join(new_dir, new_file_name))
    print("> Project {} has been created in [root]/Projects".format(project_name))


def set_project(project_name):
    # Windows cmake
    with open(windows_cmake_path, 'r') as f:
        windows_cmake_content = f.read()
        # replace set project_dir
        s = windows_cmake_content.find('set(project_dir ../Projects/')
        e = windows_cmake_content.find(')', s)
        windows_cmake_content = windows_cmake_content[:s] + \
            "set(project_dir ../Projects/{}".format(project_name) + windows_cmake_content[e:]
        # replace project files
        s = windows_cmake_content.find('set(PROJECT_FILES')
        e = windows_cmake_content.find(')', s)
        files = os.listdir(project_dir + '/' + project_name)
        set_project_files = 'set(PROJECT_FILES\n' + ''.join("\t\t${project_dir}/" + x + "\n" for x in files) + \
            '\t\t'
        windows_cmake_content = windows_cmake_content[:s] + set_project_files + windows_cmake_content[e:]
    with open(windows_cmake_path, 'w') as f:
        f.write(windows_cmake_content)
    # build VS project for Windows
    if not os.path.exists(windows_vs_path):
        os.mkdir(windows_vs_path)
    print("> Generating VS 2022 project...")
    os.system("cd " + windows_vs_path + " && " + \
              "cmake ../CMakeLists.txt -G \"Visual Studio 17 2022\" && " + \
              "cmake --build .")
    print("> VS 2022 project generated")
    # Android cmake
    # emcc
    print("> Startup project set as {}".format(project_name))
    print("VS 2022 project Multiplatform created in [root]/Windows")
    print("> actions required:")
    print(" > * Android: run gradle sync before building")
    print(" > * Web: generate new wasm files")


'''
project selection/creation wizard
note that project names are the SAME as it's root folder and entrypoint!
* for example, the project Sample3D:
* project folder: [root]/Projects/Sample3D
* entrypoint: SceneSample3D.h and SceneSample3D.cpp
'''
if __name__ == '__main__':
    # check for [root]/Projects folder, if not found = new user
    if not os.path.exists(project_dir):
        print("[New user detected]")
        shutil.copytree(sample_projects_dir, project_dir)
        print("> Projects folder created populated with sample projects")
    # project selection screen
    while True:
        all_projects = next(os.walk(project_dir))[1]
        print("[Select start project]")
        for i in range(len(all_projects)):
            print("{}) {}".format(i + 1, all_projects[i]))
        print("{}) {}".format(len(all_projects) + 1, "new project"))
        print("Select choice: ")
        choice = int(input()) - 1
        if math.isnan(choice):
            print("> invalid input")
        # selected existing project
        elif choice < len(all_projects):
            print("Selected " + all_projects[choice])
            set_project(all_projects[choice])
            break
        # selected new project
        elif choice == len(all_projects):
            print("Enter new project name (no spaces, use CamelCase, no numeric as 1st chr): ")
            new_name = input()
            if not new_name.isalnum() or new_name[0].isnumeric():
                print("> invalid project name")
            else:
                create_new_project(new_name)
                set_project(new_name)
                break