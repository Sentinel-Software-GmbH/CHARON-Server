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



## Bulding Tools:

CMAKE:
For CMAKE use this Pattern in your Build folder:
Availabe Ports:
Windows
STM32F4
```
cmake ../uds_server --preset=Windows
```
Then use ninja to build.

Ceedling:
navigate to the Port folder and use one of the Ports:
use ceedling release to build exe or elf.