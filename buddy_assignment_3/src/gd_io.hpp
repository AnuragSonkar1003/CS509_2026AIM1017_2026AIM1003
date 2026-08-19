#ifndef GD_IO_HPP
#define GD_IO_HPP

#include <string>
#include <vector>

// Section 6.1 input format:
//   DEGREE d
//   COEFFICIENTS c0 c1 c2 ... cd
//   INITIAL_X x0
//   LEARNING_RATE alpha
//   TOLERANCE epsilon
//   MAX_ITERATIONS n
struct GDInput {
    int degree = 0;
    std::vector<double> coeffs;   // size degree+1, ascending power order
    double x0 = 0.0;
    double alpha = 0.0;
    double tol = 0.0;
    long long max_iter = 0;
};

// Loads and validates a Gradient Descent input file. Per Section 11,
// rejects: invalid degree, coefficient count != degree+1, non-positive
// learning rate, non-positive tolerance, non-positive MAX_ITERATIONS.
// Pure file I/O / parsing -- never timed.
bool load_gd_input(const std::string &path, GDInput &in);

#endif
