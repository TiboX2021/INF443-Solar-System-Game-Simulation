# Projet INF443

## Getting started

Clone this repository using git :

-   If you have set a ssh key : `git clone git@gitlab.binets.fr:thibaut.de-saivre/projet-inf443.git`
-   Else (using your binet credentials) : `git clone https://gitlab.binets.fr/thibaut.de-saivre/projet-inf443.git`

This will create a `projet-inf443` folder and copy all the files from the repo in it.

## Build the executable

### On Windows

Execute the `windows_cmake_visual.bat` script located in the `scripts/` folder. It will create a `build/` folder with a file ending in **.sln** in it.
Double click this file to open the project in **Visual Studio**.

### On Linux

Execute the following commands :

```
mkdir build
cd build
cmake ..
make
```

## Adding / removing files from the project

**Do not forget to run the `cmake ..` command in the `build/` directory each time a c++ file is added or removed from the project ! This ensures that each files get compiled in a corresponding .o output file during compilation, avoiding the need to recompile unchanged files each time another one is changed**

## Test the project

The executable is named **project** (set in _CMakeLists.txt_).
