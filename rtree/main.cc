/**
 * The entrypoint for the application. Handles command line processing
 * and launching the application.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 7 - 605.621.81
 */
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <filesystem>
#include <random>

#include "rtree.h"
#include "rational.h"

/**
 * usage provides the user with a user friendly description of how to use the application.
 * @param name - the name of the executable.
 */
void usage(const char* name) {
  std::cout << "\nUsage: " << name << " ( -v | -p )  -a <int>  (-b <int>/<int> | -B <int>) -c <int>/<int> -d <int>/<int> [ -e <int>/<int> ] [ -t -z <depth> ]\n\n";

  std::cout << "-v : divide and conquer (excludes -p)\n" << std::endl;
  std::cout << "-c : chip and conquer (excludes -v)\n" << std::endl;
  std::cout << "-a, -b, -c, -d : T(n) = [a]T(n/[b]) + [c]n^[d]\n" << std::endl;
  std::cout << "-a, -B, -c, -d : T(n) = [a]T(n - [B]) + [c]n^[d]\n" << std::endl;
  std::cout << "-a, -b, -c, -d, -e : T(n) = [a]T(n/[b]) + [c]log_[d]^[e](n)\n" << std::endl;
  std::cout << "-a, -B, -c, -d, -e : T(n) = [a]T(n - [B]) + [c]log_[d]^[e](n)\n" << std::endl;
  std::cout << "Passing -e will cause the expression to be the log form.\nWithout it, the polynomial form is used. When not using\na <datafile> (-f), all other expression options are required\n\n" << std::endl;

  std::cout << "Note: for divide you must pass a rational (-b)\nand for chip you must pass an integer (-B).\n" << std::endl;

  std::cout << "Passing -t enables tracing output to stderr.\n" << std::endl;
  std::cout << "Passing -z will change the default depth to the value you specify.\n" << std::endl;
}

/**
 * A helper function for parsing command line options.
 *
 * @return 2 = param not found; 1 = parse error; 0 = success
 */
template<typename T>
short parse_opt(
    const char* opt,
    int argc,
    char** argv,
    int& i,
    bool has_arg = false,
    std::function<T(char*)> parse = nullptr,
    T& ret = nullptr) {

  if ((strlen(argv[i]) != 2) || strncmp(argv[i], opt, 2) != 0) {
    return 2;
  }

  if (!has_arg) {
    return 0;
  }

  if (parse == nullptr) {
    return 1;
  }

  if (i+1 >= argc) {
    std::cerr << "Error: The argument [" << opt << "] requires a parameter" << std::endl;
    usage(argv[0]);
    return 1;
  }
  i = i + 1;
  ret = parse(argv[i]);
  return 0;
}


/**
 * main entrypoint for the application. Handles the commandline argument parsing and the launching of
 * the application. In particular it drives the tree creation and output.
 */
int main(int argc, char** argv) {

  if (argc < 2) {
    usage(argv[0]);
    return 1;
  }

  bool divide, chip, log;
  int a, bi; rational b, c, d, e;
  int depth = 3;

  bool trace = false;
  bool bench = false;

  int ACC = 0;

  std::string trace_args = "[TRACE] Parsed args: ";

  for (int i=1; i < argc; i++) {

    int res = parse_opt<int>(
        "-a", argc, argv, i, true,
        [=](char* arg) { return atoi(arg); },
        a);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      trace_args.append(" -a ").append(std::to_string(a));
      continue;
    }

    res = parse_opt<int>(
        "-B", argc, argv, i, true,
        [=](char* arg) { return atoi(arg); },
        bi);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      b = rational{ bi, 1 };
      trace_args.append(" -B ").append(std::to_string(bi));
      continue;
    }

    res = parse_opt<int>(
        "-z", argc, argv, i, true,
        [=](char* arg) { return atoi(arg); },
        depth);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      trace_args.append(" -z ").append(std::to_string(depth));
      continue;
    }

    res = parse_opt<rational>(
        "-b", argc, argv, i, true,
        [=](char* arg) { return rational(arg); },
        b);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      trace_args.append(" -b ").append(b.to_string());
      continue;
    }

    res = parse_opt<rational>(
        "-c", argc, argv, i, true,
        [=](char* arg) { return rational(arg); },
        c);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      trace_args.append(" -c ").append(c.to_string());
      continue;
    }

    res = parse_opt<rational>(
        "-d", argc, argv, i, true,
        [=](char* arg) { return rational(arg); },
        d);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      trace_args.append(" -d ").append(d.to_string());
      continue;
    }

    res = parse_opt<rational>(
        "-e", argc, argv, i, true,
        [=](char* arg) { return rational(arg); },
        e);

    if (res == 1) {
      return 1;
    }
    if (res == 0) {
      log = true;
      trace_args.append(" -e ").append(e.to_string());
      continue;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-h", 2) == 0) {
      usage(argv[0]);
      return 0;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-p", 2) == 0) {
      trace_args.append("-p (chip) ");
      chip = true;
      continue;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-v", 2) == 0) {
      trace_args.append("-v (divide) ");
      divide = true;
      continue;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-t", 2) == 0) {
      trace = true;
      continue;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-n", 2) == 0) {
      bench = true;
      continue;
    }

    std::cerr << "Error: Unknown argument [" << argv[i] << "]" << std::endl;
    usage(argv[0]);
    return 1;
  }

  if (trace) {
    std::cerr << "[TRACE] Raw Args: ";
    for (int i=0; i < argc; i++)
      std::cerr << "[" << argv[i] << "] ";
    std::cerr << std::endl;

    std::cerr << trace_args << std::endl;

    ACC += argc;
  }

  
  if (bench) {
    std::cout << "Writing benchmark data to stderr for max depth 3 - 13. Format is [n ACC<endl>] where n is the max depth and ACC is the op count." << std::endl;

    for (int i=3; i<14; i++) {
      rt::expand_tree(true, false, 3, rational{ 1, 3 }, rational{ 1, 1}, rational{1, 1}, rational{ 0, 1 }, i, false, ACC);
      std::cerr << (i) << " " << ACC << std::endl;
      ACC = 0;
    }

    return 0;
  }

  if (divide && chip) {
    std::cerr << "ERROR: Only one of -v or -p can be specified" << std::endl;
    usage(argv[0]);
    return 1;
  }

  if (!divide && !chip) {
    std::cerr << "ERROR: At least one of -v or -p must be specified" << std::endl;
    usage(argv[0]);
    return 1;
  }

  if (divide && (a <= 1)) {
    std::cerr << "ERROR: for divide -a must be specified and must be > 1." << std::endl;
    usage(argv[0]);
    return 1;
  }

  if (chip && (a < 1)) {
    std::cerr << "ERROR: for chip -a must be specified and must be > 0." << std::endl;
    usage(argv[0]);
    return 1;

  }

  if ((b.numerator() <= b.denominator()) && (divide)) {
    std::cerr << "ERROR: -b must be specified and must be > 1." << std::endl;
    usage(argv[0]);
    return 1;
  }

  if (divide) {
    b = b.reciprocal();
  }

  if (chip && (bi < 1)) {
    std::cerr << "ERROR: if (chip) -bi must be specified and must be > 1." << std::endl;
    usage(argv[0]);
    return 1;
  }


  // we are good to build the tree
  // first set up our output adaptor
  rt::output_adaptor outa{ divide, log, a, b, c, d, e };

  std::cout << "\n\nGenerating First " << depth+1 << " levels of recursion tree: " << std::endl;
  std::cout << "*************************" << std::endl;
  if (divide) {
    std::cout << "T(n) = " << a << "T(" << b.to_string(true) << ") + ";
    if (log) {
      std::cout << c.to_string(false, true) << " * log_base(" << e.to_string() << ")^" << d.to_string(false, true) << "(n)" << std::endl;
    } else {
      std::cout << "(" << c.to_string() << ")n^(" << d.to_string() << ")" << std::endl;
    }
  } else {
    std::cout << "T(n) = " << a << "T(n - " << b.to_string() << ") + (";
    if (log) {
      std::cout << c.to_string(false, true) << " * log_base(" << e.to_string() << ")^" << d.to_string(false, true) << "(n - " << b.to_string() << "))" << std::endl;
    } else {
      std::cout << c.to_string(false, true) << "(n - " << b.to_string() << ")^" << d.to_string(false, true) << ")" << std::endl;
    }

  }
  auto levels = rt::expand_tree(divide, log, a, b, c, d, e, depth, trace, ACC);
  std::cout << "*************************" << std::endl;
  for (int i = 0; i < levels.size(); i++) {
    std::cout << "At Depth: " << i << ", # Nodes: " << (int)std::pow(a, i) << std::endl;
    outa.output(std::cout, levels[i]);
    std::cout << "*************************" << std::endl;
  }

  std::cout << "\n\n";
  return 0;
}
