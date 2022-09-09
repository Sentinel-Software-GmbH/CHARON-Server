"""@package docstring
small script to instruct cmake;
"""
import argparse, subprocess, os , pathlib, re, shutil 


def deleteFilesAndDirectoriesInDir(directoryToClean):
    """ @brief wrapper for shutil.rmtree function and for loop attention function also looks in invisible directory's use with care;
        @param directoryToClean path of directory to clean up;
    """
    print(os.getcwd())
    for root, dirs, files in os.walk(directoryToClean, topdown = False):
        for item in dirs:
            path = os.path.join(root,item)
            if(re.search('System Volume Information', path)) is None:
                shutil.rmtree(path)
    for root, dirs, files in os.walk(directoryToClean, topdown = False):
        for item in files:
            path = os.path.join(root,item)
            if(re.search('System Volume Information', path)) is None:
                os.remove(path)


def argParser():
    """ @brief argument parser defines selectable options;
        @return namespace args used to make options available;
    """
    parser = argparse.ArgumentParser(description='Build your charon as you like; all target builds contain debug information by default;')
    parser.add_argument("-p","--port",choices=["Windows","windows","standalone","Standalone","STM32F4","stm32f4","UnitTest","unittest"],
                        help=" use -p/--port + Windows to build charon server for Windows;" 
                        " use -p/--port + STM32F4 to build charon server for stm32f4-discovery board;"
                        " use -p/--port + standalone to build charon server standalone library;"
                        " use -p/--port + UnitTest to run ceedling for UnitTests;"
                        )
    parser.add_argument("-r","--release",choices=["Windows","windows","standalone","Standalone","STM32F4","stm32f4"],
                        help=" use -r/--release + Windows to build charon server for Windows;" 
                        " use -r/--release + STM32F4 to build charon server for stm32f4-discovery board;"
                        " use -r/--release + standalone to build charon server standalone library;"
                        )
    args = parser.parse_args()
    return args


def cmakeAndNinja(cmdStream,ninja):
    """ @brief will execute commandline to configure cmake and execute ninja or ceedling
        @param cmdStream command to  execute in commandline;
        @param ninja True if ninja should be performed;
    """
    builder = "ninja"
    stream = subprocess.run(cmdStream, capture_output=True, text=True)
    output = stream.stdout
    print(output)
    if(ninja is True):
        stream = subprocess.run(builder, capture_output=True, text=True)
        output = stream.stdout
        print(output)


def prepBuildFolder(buildType):
    """ @brief preparing build folder depending on buildtype if debug or release for cmake config
        @param buildType defines which type of build will be used debug or release;
    """
    if(buildType == "release"):
        neededWorkspace = "build\\CMakeReleaseBuild"
    else:
        neededWorkspace = "build\\CMakeDebugBuild"
    cmakeCache = "CMakeCache.txt"
    cmakeCacheFolder = "CMakeFiles"
    if(os.path.isdir(neededWorkspace) is False):
        print("build folder does not exist and will be created")
        pathlib.Path(neededWorkspace).mkdir(parents=True,exist_ok=True)
    tem = os.getcwd() 
    if(tem is not neededWorkspace):
        os.chdir(neededWorkspace)
    if(os.path.isdir(cmakeCacheFolder) is True):
        deleteFilesAndDirectoriesInDir(cmakeCacheFolder)
    if(os.path.isfile(cmakeCache) is True):
        os.remove(cmakeCache)



def buildRunnerWindows(workspace,buildType):
    """ @brief windows build runner will create debug build for windows port;
        @param workspace current working directory;
        @param buildType defines which type of build will be used debug or release;    
    """
    if(buildType == "release"):
        cmdStream = "cmake ../../ --preset=windows -DCMAKE_BUILD_TYPE=Release"
    else:
        cmdStream = "cmake ../../ --preset=windows"
    prepBuildFolder(buildType)
    cmakeAndNinja(cmdStream,True)
    os.chdir(workspace)
    print(os.getcwd())


def buildRunnerSTM32(workspace,buildType):
    """ @brief STM32 build runner will create debug build for discovery board port; 
        @param workspace current working directory;
        @param buildType defines which type of build will be used debug or release;    
    """
    if(buildType == "release"):
        cmdStream = "cmake ../../ --preset=stm32f4 -DCMAKE_BUILD_TYPE=Release"
    else:
        cmdStream = "cmake ../../ --preset=stm32f4"
    prepBuildFolder(buildType)
    cmakeAndNinja(cmdStream,True)
    os.chdir(workspace)
    print(os.getcwd())


def buildRunnerStandalone(workspace,buildType):
    """ @brief standalone build runner will create debug build for windows port;
        @param workspace current working directory;
        @param buildType defines which type of build will be used debug or release;    
    """
    if(buildType == "release"):
        cmdStream = "cmake ../../ --preset=standalone -DCMAKE_BUILD_TYPE=Release"
    else:
        cmdStream = "cmake ../../ --preset=standalone"
    prepBuildFolder(buildType)
    cmakeAndNinja(cmdStream,True)
    os.chdir(workspace)
    print(os.getcwd())


def buildRunnerCeedling():
    """ @brief ceedling runner will run ceedling; """
    cmdStream = "ruby toolchain\\ceedling\\bin\\ceedling"
    cmakeAndNinja(cmdStream,False)


def mainRunner():
    args = argParser()
    workspace = os.getcwd()
    try:
        if(args.port in ("Windows", "windows")):
            buildRunnerWindows(workspace, "debug")
        if(args.port in ("STM32F4","stm32f4")):
            buildRunnerSTM32(workspace, "debug")
        if(args.port in ("standalone", "StandAlone", "Standalone")):
            buildRunnerStandalone(workspace, "debug")
        if(args.port in ("UnitTest", "unittest", "Unittest")):
            buildRunnerCeedling()
        if(args.release in ("Windows", "windows")):
            buildRunnerWindows(workspace, "release")
        if(args.release in ("STM32F4", "stm32f4")):
            buildRunnerSTM32(workspace, "release")
        if(args.release in ("standalone", "StandAlone", "Standalone")):
            buildRunnerStandalone(workspace, "release")
    except Exception as unwantedBehavior:
        print(unwantedBehavior)



if __name__ == "__main__":
    mainRunner()
