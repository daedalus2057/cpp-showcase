# Welcome to RTree

The included application is built with CMake. A recent version of cmake and a working c++ compiler
should be all that is needed to compile, test, and run the application. That said, I have only tested
this with clang 13.0.1 on MacOS arm64. If you have any issues, please do not hesitate to contact me. 

## Compiling

Use the following three commands to compile and assemble the project:

$ mkdir build && cd build
$ cmake ..
$ cmake --build .

## Testing

This project includes minimal unit tests written with the GoogleTest framework. To run the tests:

$ cd build
$ ./rtree_test

In addition to the automated tests there are argument files in ./test_input

To use the argument files, use a shell expansion like this:

$ ./rtree $(<../test_input/<name_of_file>.arg.txt)

Note there are no quotes around the expansion. 

## Running

$ cd build
$ ./rtree -h

**** Please note: This executable includes support for logarithmic non-recussive cost. ****
**** See the help command for more information. ****

## Included Files
ReadMe.md <-- this document
CMakeLists.txt <-- build file
rational.cc 
rational.h
main.cc
rtree.cc
rtree.h
rtree_test.cc

test_input/ <-- test arguments used
  chip_4-1.args.txt
  chip_4.3-1.args.txt
  chip_4.4-4.args.txt
  divide_4.18.args.txt
  divide_4.5.args.txt
  divide_4.7.args.txt

test_output/ <-- test output and trace data
  bench.dat <-- runtime data
  chip_4-1.output.txt <-- each file contains trace data
  chip_4.3-1.output.txt
  chip_4.4-4.output.txt
  divide_4.18.output.txt
  divide_4.5.output.txt
  divide_4.7.output.txt
