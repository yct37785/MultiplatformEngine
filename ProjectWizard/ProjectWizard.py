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
windows_vs_path = '../Windows/VS'


'''
utils
'''
def select_project_menu():
    all_projects = next(os.walk(projects_dir))[1]
    print("> Select project:")
    for i in range(len(all_projects)):
        print("* ({}) {}".format(i + 1, all_projects[i]))
    choice = int(input())
    if math.isnan(choice) or choice < 1 or choice > len(all_projects):
        print("> invalid input")
        return None
    return all_projects[choice - 1]


'''
create new project from template in [root]/Projects
'''
def create_new_project_files(project_name):
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


'''
create new VS sln for project in [root]/Windows/VS
'''
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
    # create empty VS project folder
    if not os.path.exists(project_vs_path):
        os.makedirs(project_vs_path)
    # create cmake file in VS project folder
    with open(project_vs_path + '/CMakeLists.txt', 'w') as f:
        f.write(windows_cmake_content)
    # build VS project for Windows
    print("> Generating VS 2022 project for {}...".format(project_name))
    os.system("cd " + project_vs_path + " && " + \
              "cmake CMakeLists.txt -G \"Visual Studio 17 2022\"")
    print("> VS 2022 project for {} generated".format(project_name))


'''
setup for new user
'''
def setup_new_user():
    # check for [root]/Projects folder, if not found = new user
    if os.path.exists(projects_dir):
        return
    print("[new user detected]")
    shutil.copytree(sample_projects_dir, projects_dir)
    print("> Projects folder created populated with sample projects")
    # create vs for sample projects
    create_project_vs('Sample3D')


'''
create new project wizard
'''
def create_new_project_wizard():
    print("> Enter new project name (no spaces, use CamelCase, no numeric as 1st chr): ")
    new_name = input()
    if not new_name.isalnum() or new_name[0].isnumeric():
        print("> invalid project name")
        return
    create_new_project_files(new_name)
    create_project_vs(new_name)


'''
configure Android
We do not create individual Android Studio sln for each project as Android Studio is only used
for releasing an Android build for the specified project, not for editing like in VS
'''
def configure_project_for_android(project_name):
    pass


def configure_project_for_android_wizard():
    project_name = select_project_menu()
    print(project_name)

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
    # configuration screen
    while True:
        print("[select options]")
        print("* (1) create new project")
        print("* (2) recreate VS sln for project")
        print("* (3) configure Android for project")
        print("* (4) configure WASM for project")
        print("* (5) update VS sln for all projects")
        print("* (6) quit")
        choice = int(input())
        if math.isnan(choice) or choice < 1 or choice > 6:
            print("> invalid input")
        elif choice == 1:
            create_new_project_wizard()
        elif choice == 2:
            pass
        elif choice == 3:
            configure_project_for_android_wizard()
            break
        elif choice == 4:
            pass
        elif choice == 5:
            pass
        elif choice == 6:
            break