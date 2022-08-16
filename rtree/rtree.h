/**
 * rtree.h contains the typedefs for the algorithm developed
 * for this assignment.  See rtree.cc for the method docs.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 7 - 605.621.81
 */
#ifndef RTREE_H
#define RTREE_H

#include <vector>
#include "rational.h"


namespace rt {

  std::string polynomial_cost(
      const bool,
      const rational&, 
      const rational&, 
      const rational&); 

  std::string polynomial_log_cost(
      const bool divide, 
      const rational &size, 
      const rational &c, 
      const rational &d, 
      const rational &e); 

  std::string chip_size(const int&); 

  std::string div_depth_size(
      const int&,
      const rational&);

  std::string chip_depth_size(
      const int&,
      const int&);

  /**
   * A data container for the node format specified in the assignment.
   * Note the data is stored as string data. This matches the format in the 
   * supplementary documentation and thus tries to be an exact replica of
   * the specified data structure.
   */
  struct simple_node {
    const std::string size;
    const std::string cost;

    simple_node(
        const std::string sz,
        const std::string cst) :size(sz), cost(cst) { }
  }; 

  /**
   * A wrapper for each level of the recussion tree. Holds metadata needed to generate 
   * the level and also contains the nodes as the level.
   */
  class tree_node {
    private:
      const int _count;
      const rational _size;
      const rational _cost;

      const rational _c;
      const rational _d;

      std::vector<simple_node> nodes_at_depth;

    public:
      tree_node(
          const bool divide,
          const bool poly,
          const rational sz,
          const rational cst,
          const rational c,
          const rational d,
          const rational e,
          const int cnt) :_size(sz), _cost(cst), _c(c), _d(d), _count(cnt) { 

        for (int i=0; i < cnt; i++) 
          nodes_at_depth.emplace_back(simple_node{ (divide) ? sz.to_string(true) : rt::chip_size(sz.numerator()), (poly) ? rt::polynomial_cost(divide, sz, c, d) : rt::polynomial_log_cost(divide, sz, c, d, e) });
      }

      // Read-only
      inline rational size() const { return _size; }
      inline rational cost() const { return _cost; }
      inline int count() const { return _count; }
      inline rational total_cost() const { return rational{_count, 1} * _cost; }
      inline simple_node sample_node() const { return nodes_at_depth[0]; }
  };

  /**
   * A simple utility helper class for formatting nodes.
   */
  class output_adaptor {
    private:
      const bool divide;
      const bool log;
      const int a;
      const rational& b;
      const rational& c;
      const rational& d;
      const rational& e;

    public:
      inline output_adaptor(
          const bool divide,
          const bool log,
          const int a,
          const rational& b,
          const rational& c,
          const rational& d,
          const rational& e): divide(divide), log(log), a(a), b(b), c(c), d(d), e(e) { }


      void output(std::ostream&, const tree_node&); 
  };

  std::vector<rt::tree_node> expand_tree(
      const bool type,
      const bool poly,
      const int,
      const rational&,
      const rational&,
      const rational&,
      const rational&,
      const int,
      bool, int&);  
}

#endif
