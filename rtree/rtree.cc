/**
 * Implementation for the header rtree.h. See the header for class documentation. Functions are documented here.
 * and launching the application.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 7 - 605.621.81
 */
#include "rtree.h"
#include "rational.h"
#include <cmath>

/**
 * If we have a log based nonrecussive cost. Note that the form is:
 * c(log_e)^d n which is equal to 
 * c(log_e)^d 1*n = c(log_e)^d sz*n = c(log_e)^d(sz) + c(log_e)^d(n)
 *
 * This function handles the calculation of the first term for any given level
 * of the tree.
 * @param d - see above
 * @param e - see above
 * @param sz - the size for the current level
 * @return the calculated first term in the individual node cost.
 */
double calc_log_work_cost(
    const rational& c,
    const rational& d,
    const rational& e,
    const rational& sz) {

  // log_b x = log_2(x)/log_2(b)
  auto log_b = [](double base, double x) {
    return std::log2(x) / std::log2(base);
  };

  return c.to_real() * std::pow(log_b(e.to_real(), sz.to_real()), d.to_real());
};

/**
 * Formatter for the polynomial non-recursive cost function.
 * Supports divide and conqure as well as chip and conqure (via false divide param)
 *
 * @return the formatted cost
 */
std::string rt::polynomial_cost(const bool divide, const rational &size, const rational &c, const rational &d) {

  if (divide) {
  return c.open_paren().append(c.to_string()).append(c.close_paren())
    .append("(")
    .append(size.to_string(true))
    .append(")^")
    .append(d.open_paren()).append(d.to_string()).append(d.close_paren());
  }

  return c.open_paren().append(c.to_string()).append(c.close_paren())
    .append("(n - ")
    .append(size.to_string())
    .append(")^")
    .append(d.open_paren()).append(d.to_string()).append(d.close_paren());
}

/**
 * Formatter for the log-based non-recursive cost function.
 * Supports divide and conqure as well as chip and conqure (via false divide param)
 * 
 * @return the formatted cost
 */
std::string rt::polynomial_log_cost(
    const bool divide, 
    const rational &size, 
    const rational &c, 
    const rational &d, 
    const rational &e) {

  if (divide) {

    double lw = calc_log_work_cost(c, d, e, size);
    std::string s = std::to_string(std::abs(lw));

    if (lw <=0) {
      return c.open_paren().append(c.to_string()).append(c.close_paren())
        .append("log_").append(e.to_string(false, true)).append("^").append(d.to_string(false, true))
        .append("(n)").append(" - ").append(s);
    }
    
    return s.append(" + ").append(c.open_paren()).append(c.to_string()).append(c.close_paren())
      .append("log_").append(e.to_string(false, true)).append("^").append(d.to_string(false, true))
      .append("(n)");
  }

  return c.open_paren().append(c.to_string()).append(c.close_paren())
    .append("log_").append(e.to_string(false, true)).append("^").append(d.to_string(false, true)).append("(n - ")
    .append(size.to_string())
    .append(")");
}

std::string rt::chip_size(const int &sz) {
  return std::string("T(n - ").append(std::to_string(sz)).append(")");
}

/**
 * The main workhorse of the assignment. Takes a configuration and builds the tree specified by it.
 * Accepts modifiers including changing the max_depth as well as enabling trace and accumulation.
 *
 * The two boolean parameters control the behavior of the expansion. div = true indicates a divide
 * and conqure recurrence, false indicates a chip and conqure. In the case of the log parameter, if
 * this is true we are using a log based non-recurssive cost.
 *
 * the rest of the parameters are the same as specified in the assignment.
 *
 * @return max_depth levels of the recursion tree.
 */
std::vector<rt::tree_node> rt::expand_tree(
    const bool div, 
    const bool log, 
    const int a,
    const rational& b,
    const rational& c,
    const rational& d,
    const rational& e,
    const int max_depth,
    bool TRACE, int &ACC)  {
  
  ACC++;
  if (TRACE) std::cerr << "(TRACE) Entered expand_tree with params: " 
    << " div: " << div << " poly: " << !log << " a: " << a << " b: " 
    << b.to_string() << " c: " << c.to_string() << " d: " << d.to_string()
    << " e: " << e.to_string() << "  max_depth: " << max_depth << std::endl;
    
    
  std::vector<tree_node> levels;
  rational work_size = (div) ? rational { 1, 1 } : rational { 0, 1 };

  for (int depth = 0; depth < max_depth+1; depth++) {
    // create the node at depth
    // update our cost
    rational cost = (!log) ? work_size^d : work_size;

    if (TRACE) std::cerr << "(TRACE) Expanding level " << depth << " with work_size " << work_size.to_string() << " and node_cost " << cost.to_string() << std::endl;

    levels.emplace_back(tree_node {
        div, !log,
        work_size,
        // calculate the work_cost on the fly
        // but hold back the constant
        cost,
        c, d, e,
        // truncate (floor) as 3 > depth > 0
        (int)std::pow(a, depth) });

    ACC += (int)std::pow(a, depth);

    // update our work_copy
    work_size = (div) ? work_size * b : work_size + b; 
    ACC += 3;
  }

  return levels;
}

/**
 * The single output format function. Handles all types of relations. Formats according to a format
 * specified by Prof. Boon (with some enhancements).
 *
 * @param ost -- a output stream that will get the output
 * @param node -- the level wrapper for the depth being printed.
 */
void rt::output_adaptor::output(std::ostream &ost, const tree_node& node) {
  rt::simple_node sn = node.sample_node();
  ost << "Expanded Node Form: [ " << sn.size << " | " << sn.cost << " ]" << std::endl;
  if (divide) {
    if (log) {
      ost << "Total work: " << node.count() << "(" << polynomial_log_cost(divide, node.size(), c, d, e) << ")" << std::endl;
    } else {
      ost << "Total work: " << (node.total_cost() * c).to_string(false, true) << "n^" << d.to_string(false, true) << std::endl;
    }
  } else {
    if (log) {
      ost << "Total work: " << (rational{node.count(), 1} * c).to_string(false, true) << " * log_base(" << e.to_string() << ")^" << d.to_string(false, true) << rt::chip_size(node.size().numerator()) << std::endl;
    } else {
      ost << "Total work: " << (rational{node.count(), 1} * c).to_string(false, true) << "(n - " << node.size().numerator() << ")^" << d.open_paren() << d.to_string() << d.close_paren() << std::endl;
    }
  }
}
