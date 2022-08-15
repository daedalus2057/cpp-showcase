#include <iostream>
#include <fstream>
#include <string.h>

#include "deter.h"

/**
 * usage provides the user with a user friendly description of how to use the application.
 * @param name - the name of the executable.
 */
void usage(const char* name) {
  std::cout << "\n\nUsage: " << name << " -f <filename> [ -o <filename> ]\n\n";
  std::cout << "This program requires a single argument which is the name of the file containing the matrices to compute. An optional second argument [-o] can be supplied to output to a named file.\n\nThe data file should be formatted with nothing but numerical values formated such as:";
  
  std::cout << R"(

3  
3 -2 4 
-1 5 2 
-3 6 4 

)";

  std::cout << "Notice that the order of the matrix preceeds the matrix data. If the order is incorrect or the matrix is not square, execution will stop. \n\nEnjoy.\n\n" << std::endl;

}

/**
 * main entrypoint for the application. Handles the commandline argument parsing and the launching of
 * the application. In particular it manages the opening and closing of files.
 */
int main(int argc, char** argv) {
  
  if (argc < 3) {
    usage(argv[0]);
    return 1;
  }

  const char* fname;
  const char* out_fname = nullptr;
  bool isInt = true;

  for (int i=1; i < argc; i++) {
    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-f", 2) == 0) {
      if (i+1 >= argc) {
        std::cout << "Error: The argument [-f] requires a parameter <filename>" << std::endl;
        return 1;
      }
      i = i + 1;
      fname = argv[i];
      continue;
    }

     
    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-h", 2) == 0) {
      usage(argv[0]);
      return 0;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-o", 2) == 0) {
      if (i+1 >= argc) {
        std::cout << "Error: The argument [-o] requires a parameter <filename>" << std::endl;
        return 1;
      }
      i = i + 1;
      out_fname = argv[i];
      continue;
    }

    std::cout << "Error: Unknown argument [" << argv[i] << "]" << std::endl;
    usage(argv[0]);
    return 1;

  }

  // is this a readable file?
  std::ifstream data(fname);

  if (!data.is_open()) {
    std::cout << "The file [ " << fname << " ] could not be opened for reading." << std::endl;
    return 1;
  }

  // are we writing to a file?
  if (out_fname != nullptr) {
    std::ofstream outs(out_fname, std::ofstream::trunc);
    if (!outs.is_open()) {
      std::cout << "The file [ " << out_fname << " ] could not be opened for writing." << std::endl;
      return 1;
    }

    deter::read_matrices<double>(
        data, 
        outs,
        &deter::computeDeterminant<double>, 
        &deter::reportResult<double>);

    // clean up
    outs.close();
    data.close();

    return 0;
  }
    

  deter::read_matrices<double>(
      data, 
      std::cout,
      &deter::computeDeterminant<double>, 
      &deter::reportResult<double>);

  data.close();
  return 0;


}
