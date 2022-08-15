# Deter: a matrix determenet calculator

Author: Donovan Nye <donovan.nye@gmail.com>
Version: 0.1

Note: This project has been built on the mac os M1 platform and the amd 64 bit linux kernal. It has NOT been tested on Windows, YMMV. 

## Building
Building is simple:

$ mkdir build && cd build
$ cmake ..
$ cmake --build . && ./deter_test

The last command will build and run the test suite. 

## Running
The main binary is called "deter" and will be in the build directory after build completes. Usage can be viewed with the parameter -h

$ ./deter -h 

Normal usage is:

$ ./deter -f <input_file> -o <output_file>

The filename should be relative to the directory running the binary and as such the above command could be:

$ ./deter -f ../real_big_input.txt # test up to order 12 random decimal values

## Data Files
Several data files are included. 
required_input.txt - the required test data
err_input.txt - invalid test data
random_data.txt - a different form of errant data
real_input.txt - a matrix with real values
wild_input.txt - valid data formatted with wild whitespace
real_big_input.txt - a randomly generated test set ranging from 4 to 12 real valued matrices.

Included is the python script generate_matrix.py. This script generates the random matrix data found in the real_big_input.txt file and could be used to generate more test data if needed.
