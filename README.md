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



### Building Tools:

## Supported Ports:
```
Windows
STM32F4
```
### Small How To:

# CMAKE:
For CMAKE use this Pattern in your Build folder:

```
cmake ../uds_server --preset=Windows

```
If you want to switch Ports without deleting the full build folder then follow the steps.

Step 1:
Navigate to your build folder and find and delete the CMakeCache.txt.

Step 2:
Find the and delete the CMakeFiles folder.

Step 3:
Open cmd in your build folder and use the pattern below(use the Port you like to build).
The previous steps causes CMake to reconfigure the project and swapping compilers. 
Then use ninja to build.

# Ceedling:
Navigate to the Port folder and use one of the Ports.
Use ceedling release to build .exe or .elf.


# runBuilder
You can use the runBuilder.cmd to run and configure cmake and ninja for you.
To Use the runBuilder open a cmd in your Project folder and use:
```
runBuilder.cmd --port Windows
```
The command above will create a build folder outside your project folder and will configure cmake to for the Windows Port,
it will also run ninja.
The command for the other ports are the same as the cmake presets.

## Note
standalone build will only create a library without any port dependencies.
All builds containing debug information for release build you need to use:

```
cmake ../uds_server --preset=Windows -DCMAKE_BUILD_TYPE=Release
```

# Available presets
```
Windows
STM32F4
standalone
```