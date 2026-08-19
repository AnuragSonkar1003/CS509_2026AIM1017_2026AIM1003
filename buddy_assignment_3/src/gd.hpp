#ifndef GD_HPP
#define GD_HPP

#include <vector>

struct GDResult {
    double final_x = 0.0;
    double final_fx = 0.0;
    long long iterations = 0;
    bool converged = false;
    double time_ms = 0.0;
};

// One-variable polynomial gradient descent, generic over degree:
//   f(x)  = c0 + c1*x + c2*x^2 + ... + cd*x^d
//   f'(x) = c1 + 2*c2*x + ... + d*cd*x^(d-1)
//   x_new = x - learning_rate * f'(x)
// Stops when |f'(x)| <= tolerance (converged) or max_iter iterations have
// run (not converged). The whole iteration loop is timed.
GDResult gradient_descent(const std::vector<double> &coeffs, double x0,
                           double alpha, double tol, long long max_iter);

// Runs gradient_descent() `repeats` times (all producing the same
// deterministic result) and reports the average execution time, per
// Section 6.3 / Section 8's allowance for very fast runs. The returned
// result's x/fx/iterations/converged come from a single representative run;
// only time_ms is averaged.
GDResult gradient_descent_averaged(const std::vector<double> &coeffs, double x0,
                                    double alpha, double tol, long long max_iter,
                                    int repeats);

#endif
