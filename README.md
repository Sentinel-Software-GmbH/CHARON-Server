# Mainpage {#mainpage}

Charon UDS Server
==================

Hello and Welcome to the Charon UDS Server Documentation!  
If you are reading this, chances are that you want to implement Charon into your Software, and I'm really glad about that!

Charon stands for **C** ar **H** ealth **A** nalyze **R** ealtime **O** nboard **N** etwork.  
It means, that we want to provide an quick, easy and lightweight way to implement the UDS Standard into your Automotive Application.

Note that the Charon UDS Server is marked as a Work In Progress and that there still might be some Bugs in the Code, or Functions that are not implemented yet.

## Usage

To use the Charon UDS Server you need to set up a few things:

## Note 

The EA design will no longer maintained. Instead the architecture documentation can be found at the following link:

https://drive.google.com/file/d/1WDjvAKW8tA1s4Mtgc6YLVcqvCr4p2PGO/view?usp=sharing

## Thanks

# Building Tools and small How To:

### CMAKE:
For CMAKE use one of the predefined pattern in your build folder as example:

If you want to build your port in a build folder that is outside your project folder:
```
cmake ../uds_server --preset=Windows    
```
If you want to build your port in a build folder that is inside your project folder:
```
cmake ../ --preset=Windows 
```
The command above causes CMake to configure the windows port as debug build.
Use ninja after configuring CMake to build your desired port.

If you want to switch Ports without deleting the full build folder then follow the steps.

Step 1:
Navigate to your build folder then find and delete the "CMakeCache.txt" if it exists.

Step 2:
Find and delete the "CMakeFiles" folder in your build folder if it exits.

Step 3:
Open cmd in your build folder and use one of the pattern below(use the Port you like to build).
The previous steps causes CMake to reconfigure the project and swapping compilers. 
Then use ninja to build.

### Ceedling:
Navigate to the Port folder and use one of the Ports.
Open cmd in the chosen port folder then use "ceedling release" ind your cmd to build .exe or .elf.


### runCharonBuilder:
You can use the runCharonBuilder.cmd to run and configure cmake and ninja for you.
To Use the runCharonBuilder open a cmd in your project folder and use:
```
runCharonBuilder.cmd --port Windows
```
The command above will create a debug build folder inside your project folder. After that it will configure cmake to for the Windows Port,
it will also run ninja.
You can choose between debug and release build, to create a release build use the command below.
```
runCharonBuilder.cmd --release Windows
```
The command above will also create a release build folder.
The command for the other ports are the same as the cmake presets.
If you like you can use:
```
runCharonBuilder.cmd --port unittest
```
The command above will create a build folder and run ceedling to create unit tests. 

## Note
standalone build will only create a library without any port dependencies.
All CMake presets containing debug information, if you want a release build you need to use:

```
cmake ../uds_server --preset=Windows -DCMAKE_BUILD_TYPE=Release
```

# Available presets
```
Windows
STM32F4
standalone
```

## Supported Ports:
```
Windows
STM32F4-Discovery
```