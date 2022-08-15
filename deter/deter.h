#ifndef DETER_H
#define DETER_H

#include <chrono>
#include <functional>
#include <iostream>
#include <exception>
#include <cmath>
#include <cstdio>
#include <type_traits>
#include <memory>
#include <iomanip>
#include <chrono>

/**
 * The deter namespace contains all the functionality required for the assignment.
 * A namespace was deamed sufficient in this assignment as the needs were very 
 * simple. See the functions contained for more information.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 7 - 602.202.82
 */
namespace deter {

  /**
   * read_state is a simple enum used in the parsing of the input stream
   * see the implementation of read_matrices for more information.
   */
  enum read_state { wait, size, rows };

  /**
   * A very simple function to determine if the given
   * character is numeric.
   * @param c - the character to test.
   * @return true if the character is numeric, false otherwise.
   */
  bool isNumeric(const char &c);

  /**
   * read_matrices is the most lengthy function in our application. It is complicated by
   * the need to validate the input. The parser is quite lenient in terms of whitespace
   * but does not tolerate non-numeric data or invalid sizes or ordering. The passed in
   * input stream should consist of a series of an integer representing the order of the 
   * matrix with only that digit on the command line, followed by a square matrix of 
   * that order where each row contains the correct number of elements and there are the
   * correct number of complete rows. Any errant newlines or characters will result in
   * exection being halted.
   *
   * @param s - the input stream containing the matrix data to read.
   * @param o - the output stream to write the result data to.
   * @param compute - a function capable of computing the determinant for a given matrix.
   * @param report - a reporting function that can format the result in a pleasing manner.
   *
   * @return an overall status, 0 being success and non-zero signaling failure.
   */
  template<typename T>
    int read_matrices(
        std::istream &s, 
        std::ostream &o,
        std::function<T(const int, const std::unique_ptr<T[]>&)> compute, 
        std::function<void(std::ostream &o, const int, const std::unique_ptr<T[]>&, const T, std::chrono::milliseconds)> report) {

      static_assert(std::is_arithmetic<T>::value, "Arithmetic type is required.");

      auto expect_ws_to_num = [](std::istream &in) {
        while (in) {
          if (isNumeric(in.peek())) {
            return true;
          }

          if ((in.peek() == 0x20) || (in.peek() == 0x9)) {
            in.get();
            continue;
          }

          return false;
        }

        return true; // eof
      };

      auto expect_wscr_to_num = [](std::istream &in, bool require) {
        bool cr_seen = !require;
        while (in) {
          if (isNumeric(in.peek())) {
            return cr_seen && true;
          }

          if ((in.peek() == 0x20) || (in.peek() == 0x9)) {
            in.get();
            continue;
          }

          if ((in.peek() == 0xa) || (in.peek() == 0xd)) {
            cr_seen = true;
            in.get();
            continue;
          }

          if (in.eof()) return !require; 

          return false; 
        }

        return !require; // eof
      };

      auto report_error = [&](const std::string &msg, const char &badc) {
        o << "ERROR -- Processing stopped. " << std::endl;
        o << "Error during processing: " << msg << "0x" << std::hex << (int) badc << std::endl;

        //std::cerr << "Error during processing: " << msg << "0x" << std::hex << (int) badc << std::endl;
      };

      // let's be a state machine
      read_state current_state = wait;
      int current_size = -1;
      std::unique_ptr<T[]> m;

      while (s) {
        switch (current_state) {
          case wait:
            if (!expect_wscr_to_num(s, false)) {
              report_error(
                  "Expected to find whitespace or newlines until numeric but found:", 
                  s.peek());
              return 1;
            }

            current_state = size;
            break;
          case size:
            if (!isNumeric(s.peek())) { 
              report_error(
                  "Expected to read size but found: ", 
                  s.peek());
              return 1;
            }

            s >> current_size;
            m.reset(new T[current_size * current_size]);
            if (!expect_wscr_to_num(s, true)) {
              report_error(
                  "Expected to find whitespace or newlines until numeric on next line but found:", 
                  s.peek());
              return 1;
            }

            current_state = rows;

            break;
          case rows:
            // we expect current_size x current_size rows. Let's validate that.
            for (int i=0; i < (current_size*current_size); i++) {
              s >> m[i]; 

              if (((i+1) % current_size) == 0) {
                if (!expect_wscr_to_num(s, (i+1 < (current_size*current_size)))) {
                  report_error("Invalid data and end of row: ", s.peek());
                  return 1;
                }
                continue;
              }

              // otherwise, we expect just whitespace
              if (!expect_ws_to_num(s)) {
                report_error("Invalid row data: ", s.peek());
                return 1;
              }
            }

            std::chrono::milliseconds start = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            );
            T det = compute(current_size, m);
            std::chrono::milliseconds end = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            );

            report(o, current_size, m, det, end - start);

            current_state = wait;
            break;
        }
      }

      return 0;
    }

  /**
   * given a matrix and a row and column index, produce the minor for that index.
   *
   * @param size - the size of the source matrix
   * @param m - the matrix to compute the minor from
   * @param r - the row index (from 0)
   * @param c - the column index (from 0)
   * @return the minor/cofactor for the given index.
   */
  template<typename T>
    std::unique_ptr<T[]> minor(const int size, const std::unique_ptr<T[]> &m, int r, int c) {
      static_assert(std::is_arithmetic<T>::value, "Arithmetic type is required.");

      int sz = size - 1;
      auto mi = std::make_unique<T[]>(sz*sz);

      int x = 0; 

      for (int i=0; i < size; i++) {
        if (i == r) continue;
        for (int j=0; j < size; j++) {
          if (j == c) continue;

          mi[x] = m[(i*size)+j]; 
          x++;
        }
      }

      return mi;
    }

  /**
   * Recursively computes the determinant of a given matrix. Given a matrix of size N
   * this will operate at O(n!). The reason is that for any matrix we perform an operation
   * for each row or column and then for each one of those we do the same at the matrix of
   * size N-1. This defines the factorial function N*N-1*N-2*...N-(N-1). Technically, we 
   * operate at O(n!/2). The runtime is future influenced by the number of zeros in 
   * the source matrix, the more zeros the shorter the runtime.
   *
   * @param size - The size of the starting matrix to compute.
   * @param matrix - The matrix data.
   * @return the determinant for the given matrix.
   */
  template<typename T> 
    T computeDeterminant(const int size, const std::unique_ptr<T[]> &matrix)  {
      static_assert(std::is_arithmetic<T>::value, "Arithmetic type is required.");

      std::function<T(const int, const std::unique_ptr<T[]>&)> det;
      int i = 0; // lock to first row as it will always exist
      
      det = [&](const int sz, const std::unique_ptr<T[]> &m)->T {

        if (sz == 1) return m[0];

        // additional stopping case -- 
        // bottom out at size = 2 b/c it is easy to compute.
        if (sz == 2) return (m[3] * m[0]) - (m[1] * m[2]);

        T sum = 0;

        for (int j=0; j<sz; j++) {
          if (m[(sz * i) + j] == 0) continue; // skip
          sum += pow(-1, (i + j)) * m[(sz * i) + j] * det(sz-1, deter::minor(sz, m, i, j));
        }

        return sum;
      };

      return det(size, matrix);
    }

  /**
   * reportResult is meant to be used at the end of the processing stream, taking the original
   * matrix and the computed determinant and outputting to the supplied stream. The formatting
   * is minimal and wholly contained in this function.
   *
   * @param outs - the stream to output the result to.
   * @param size - the order of the matrix
   * @param m - the matrix that was computed
   * @param deter - the determinant for the matrix
   */
  template<typename T> 
    void reportResult(std::ostream &outs, const int size, const std::unique_ptr<T[]> &m, const T deter, std::chrono::milliseconds ms) {
      static_assert(std::is_arithmetic<T>::value, "Arithmetic type is required.");

      /** 
       * utility used for sizing output, get the max width of any value in the matrix x
       */
      auto maxw = [](const int sz, const std::unique_ptr<T[]> &x) {
        int max = -1;
        char buf[16];
        for (int i=0; i<(sz*sz); i++) {
          // strange c++, is getting the min len actually this hard?
          // https://stackoverflow.com/a/13709929
          std::string str = std::to_string(x[i]);
          str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
          str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
          int len = str.size();
          
          if (max < len) max = len;    
        }

        return max;
      };

      int maxlen = maxw(size, m);

      outs << "Given Matrix (M size: " << size << "):";
      int i = 0;
      for (int j=0; j < size; j++) {
        outs << "\n| ";
        for (int k=0; k < size; k++) {
          outs << std::setw(maxlen) << m[(size * j) + k] << " " ;  
        }
        outs << "|";  
      }
      
      outs << " = det(M) = " << deter << "(" << ms.count() << "ms)" << std::endl << std::endl;
    }

}

#endif
