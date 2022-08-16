# Scorp: a scorpion solitare simulator.

Author: Donovan Nye <donovan.nye@gmail.com>
Version: 0.1

Note: This project has been built on the mac os M1 platform and the amd 64 bit linux kernal. It has NOT been tested on Windows, YMMV. 

## Building
Building is simple:

$ mkdir build && cd build
$ cmake ..
$ cmake --build . && ./scorp_test

The last command will build and run the test suite. 

## Running
The main binary is called "scorp" and will be in the build directory after build completes. 
Usage can be viewed with the parameter -h

$ ./scorp -h 

Normal usage is:

$ ./scorp -f <input_file> -o <output_file> -n <count>

The filenames should be relative to the directory running the binary and as such the above command could be:

$ ./scorp -f ../required_input.txt 

A special running mode (dare I say an enhancement?? :)) is activated when passing a -c, this activates 
the complete game metrics mode where the application will run 100,000 simulated games and report 
the number of complete games for the current player heuristics. This is meant to be a tool for 
evaluating heuristic performance and optimizing the algorithm. This mode overrides all other parameters:

$ ./scorp -c 

outputs the following.

There were 3 out of 100,000 games, for a completion rate of 0.003%.

Horrible, I know, but at least we have a way to improve now. 

## Enhancements

Two things I'd like to offer as enhancements are the use of a hybrid multilist structure for the 
game and the implementation of the metrics mode detailed above.

## Data Files
Several data files are included. 
required\_input.txt - the required test data, additional tests are done with the -n parameter
### Invalid data tests
err\_num\_cards\_input.txt invalid test data
err\_invalid\_char\_input.txt
err\_multiline.txt
