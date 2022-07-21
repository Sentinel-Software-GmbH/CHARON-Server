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
    parser = argparse.ArgumentParser(description='Build your charon as you like! If needed by Force!')
    parser.add_argument("-p",'--port',choices=["Windows","standalone","STM32F4","UnitTest"],
                        help=" use -p + Windows to build charon server for Windows;" 
                        " use -p + STM32F4 to build charon server for stm32f4-discovery board;"
                        " use -p + standalone to build charon server standalone library;"
                        " use -p + UnitTest to run ceedling for UnitTests"
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


def prepBuildFolder():
    """ @brief preparing build folder for cmake config"""
    neededWorkspace = "build"
    cmakeCache = "CMakeCache.txt"
    cmakeCacheFolder = "CMakeFiles"
    os.chdir("..")
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


def buildRunnerWindows(workspace):
    """ @brief windows build runner will create build folder if needed and runs cmake for windows port;
        @param workspace current working directory;    
    """
    cmdStream = "cmake ../uds_server --preset=Windows"
    prepBuildFolder()
    cmakeAndNinja(cmdStream,True)
    os.chdir(workspace)
    print(os.getcwd())


def buildRunnerSTM32(workspace):
    """ @brief STM32 build runner will create build folder if needed and runs cmake for discovery board port;
        @param workspace current working directory;    
    """
    cmdStream = "cmake ../uds_server --preset=STM32F4"
    prepBuildFolder()
    cmakeAndNinja(cmdStream,True)
    os.chdir(workspace)
    print(os.getcwd())


def buildRunnerStandalone(workspace):
    """ @brief standalone build runner will create build folder if needed and runs cmake for windows port;
        @param workspace current working directory;    
    """
    cmdStream = "cmake ../uds_server --preset=standalone"
    prepBuildFolder()
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
        if(args.port in ("Windows")):
            buildRunnerWindows(workspace)
        if(args.port in ("STM32F4")):
            buildRunnerSTM32(workspace)
        if(args.port in ("standalone")):
            buildRunnerStandalone(workspace)
        if(args.port in ("UnitTest")):
            buildRunnerCeedling()
    except Exception as unwantedBehavior:
        print(unwantedBehavior)



if __name__ == "__main__":
    mainRunner()
