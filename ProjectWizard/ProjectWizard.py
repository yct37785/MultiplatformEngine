import os
import shutil
import math
import re
# Projects
projects_dir = '../Projects'
# ProjectWizard [root]
template_dir = 'template'
sample_projects_dir = 'sample_projects'
windows_sample_cmake_path = 'windows_CMakeLists.txt'
# Windows
windows_tmp_cmake_path = '../Windows/CMakeLists.txt'
windows_vs_path = '../Windows/VS'


def create_new_project(project_name):
    # copy over template
    new_dir = projects_dir + '/' + project_name
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


def create_project_vs(project_name):
    project_vs_path = windows_vs_path + '/' + project_name
    # open sample cmake and replace with project specific files
    with open(windows_sample_cmake_path, 'r') as f:
        windows_cmake_content = f.read()
        # replace project_name
        s = windows_cmake_content.find('set(project_name')
        e = windows_cmake_content.find(')', s)
        windows_cmake_content = windows_cmake_content[:s] + "set(project_name {}".format(project_name) + windows_cmake_content[e:]
        # replace project files
        s = windows_cmake_content.find('set(PROJECT_FILES')
        e = windows_cmake_content.find(')', s)
        files = os.listdir(projects_dir + '/' + project_name)
        set_project_files = 'set(PROJECT_FILES\n' + ''.join("\t\t${project_dir}/" + x + "\n" for x in files) + \
            '\t\t'
        windows_cmake_content = windows_cmake_content[:s] + set_project_files + windows_cmake_content[e:]
    # create tmp cmake
    with open(windows_tmp_cmake_path, 'w') as f:
        f.write(windows_cmake_content)
    # build VS project for Windows
    if not os.path.exists(project_vs_path):
        os.mkdir(project_vs_path)
    print("> Generating VS 2022 project for {}...".format(project_name))
    os.system("cd " + project_vs_path + " && " + \
              "cmake ../../CMakeLists.txt -G \"Visual Studio 17 2022\" && " + \
              "cmake --build .")
    print("> VS 2022 project generated")


def setup_new_user():
    # check for [root]/Projects folder, if not found = new user
    if os.path.exists(projects_dir):
        return
    print("[New user detected]")
    shutil.copytree(sample_projects_dir, projects_dir)
    print("> Projects folder created populated with sample projects")
    # create vs for sample projects
    create_project_vs('Sample3D')


'''
project selection/creation wizard
note that project names are the SAME as it's root folder and entrypoint!
* for example, the project Sample3D:
* project folder: [root]/Projects/Sample3D
* entrypoint: SceneSample3D.h and SceneSample3D.cpp
'''
if __name__ == '__main__':
    # setup new user if haven't
    setup_new_user()
    # project selection screen
    # while True:
    #     all_projects = next(os.walk(project_dir))[1]
    #     print("[Select start project]")
    #     for i in range(len(all_projects)):
    #         print("{}) {}".format(i + 1, all_projects[i]))
    #     print("{}) {}".format(len(all_projects) + 1, "new project"))
    #     print("Select choice: ")
    #     choice = int(input()) - 1
    #     if math.isnan(choice):
    #         print("> invalid input")
    #     # selected existing project
    #     elif choice < len(all_projects):
    #         print("Selected " + all_projects[choice])
    #         set_project(all_projects[choice])
    #         break
    #     # selected new project
    #     elif choice == len(all_projects):
    #         print("Enter new project name (no spaces, use CamelCase, no numeric as 1st chr): ")
    #         new_name = input()
    #         if not new_name.isalnum() or new_name[0].isnumeric():
    #             print("> invalid project name")
    #         else:
    #             create_new_project(new_name)
    #             set_project(new_name)
    #             break