# Mainpage {#mainpage}

Charon UDS Server
==================

Hello and welcome to the Charon UDS Server documentation!  
If you are reading this, chances are that you want to implement Charon into your software, and I'm really glad about that!

Charon stands for **C** ar **H** ealth **A** nalyze **R** ealtime **O** nboard **N** etwork.  
It means, that we want to provide an quick, easy and lightweight way to implement the UDS Standard into your automotive application.

Note that the Charon UDS Server is marked as a Work In Progress and that there still might be some bugs in the code, or functions that are not implemented yet.

## Usage

To use the Charon UDS Server you need to set up a few things beforehand.
The Server itself is meant to be used in cooperation with the Charon Client - also here on github [LINK](https://github.com/Sentinel-Software-GmbH/CHARON-Client).
UDS is a Standard based in automotive to fullfill certain services that are widespreadly used and normalizes them.
Like:
 - Bootlaoder / Update (over the air)
 - Parameter and Persistent Data Management
 - Error Logging
 - Basic Comunication Safety and Security
 - Runtime Execution
 - EOL Adaption
 - etc...

## Note 

The EA design will no longer be maintained. Instead the architecture documentation can be found via the following link:

https://drive.google.com/file/d/1WDjvAKW8tA1s4Mtgc6YLVcqvCr4p2PGO/view?usp=sharing

## Thanks

# Building Tools and small How To:

## Tools:
### required Tools in Path:
```
CMake 
Ninja
GNU gcc for Windows
ARM NONE EABI GCC
```
use the compiler based on your target port

### optional Tools in Path:
```
Ruby
Ceedling
Python
```
for automatic build tool and unit tests.

## How To:

### CMake:
For CMake you have to create a build folder. After that you can use one of the predefined patterns in your build folder:

If you want to build a Charon port in a build folder that is created outside your project folder:
```
cmake ../uds_server --preset=Windows    
```
If you want to build a Charon port in a build folder that is created inside your project folder:
```
cmake ../ --preset=Windows 
```

The commands above causes CMake to configure the Windows port as a debug build.
Use Ninja after configuring CMake to build your desired port.

If you want to switch ports without deleting the full build folder then follow the steps below.

Step 1:
Navigate to your build folder then find and delete the "CMakeCache.txt" if it exists.

Step 2:
Find and delete the "CMakeFiles" folder in your build folder if it exits.

Step 3:
Open CMD in your build folder and use one of the patterns below (use the port you like to build).
The previous steps causes CMake to reconfigure the project and to swap compilers. 
Then use Ninja to build.

### Ceedling:
You can also use Ceedling to build unit tests and the supported Charon ports.
To build a port, navigate to the port folder and use one of the ports.
Open CMD in the chosen port folder then use ```ceedling release```  in your CMD to build .exe or .elf.
To build unit tests, open CMD in your project folder and use ```ceedling.cmd``` . 

### runCharonBuilder:
You can use the runCharonBuilder.cmd to run and configure CMake and Ninja for you.
To use the runCharonBuilder, open a CMD in your project folder and use:
```
runCharonBuilder.cmd --port Windows
```
The command above will create a debug build folder inside your project folder. After that it will configure CMake for the Windows port,
it will also run Ninja.
You can choose between a debug and a release build.
To create a release build, use the command below.
```
runCharonBuilder.cmd --release Windows
```
The command above will also create a release build folder.
The commands for the other ports are the same as the CMake presets.
If you like you can use:
```
runCharonBuilder.cmd --port unittest
```
The command above will create a build folder and run Ceedling to create unit tests. 

## Note
The standalone build will only create a library without any port dependencies.
All CMake presets are containing debug information, if you want to build a release build, you need to use:

```
cmake ../uds_server --preset=Windows -DCMAKE_BUILD_TYPE=Release
```

# Available Presets
```
Windows
STM32F4
standalone
```

## Supported Ports:
```
Windows
STM32F4
```
