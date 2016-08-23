# puddi testbench
A project for developing and testing the puddi 3D graphics engine.

CMake is required as well as puddi's dependencies.

To set up the puddi submodule:
  git submodule init
  git submodule update --remote --merge

To build Debug:
  mkdir Debug
  cd Debug
  cmake -D CMAKE_BUILD_TYPE=Debug ..
  make

To build Release:
  mkdir Release
  cd Relase
  cmake -D CMAKE_BUILD_TYPE=Release ..
  make

When running the executable, make sure the working directory is the project root directory.
e.g., from the project root, type Debug/src/puddi_testbench
