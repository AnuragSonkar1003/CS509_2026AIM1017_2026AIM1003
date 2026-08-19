#include "gd.hpp"
#include <chrono>
#include <cmath>

namespace {

// Horner's method: f(x) = c0 + c1*x + ... + cd*x^d
double poly_eval(const std::vector<double> &c, double x) {
    double result = 0.0;
    for (int i = static_cast<int>(c.size()) - 1; i >= 0; i--)
        result = result * x + c[i];
    return result;
}

// f'(x) = c1 + 2*c2*x + ... + d*cd*x^(d-1), evaluated via Horner's method
// on the (implicit) derivative coefficients i*c[i].
double poly_deriv_eval(const std::vector<double> &c, double x) {
    int d = static_cast<int>(c.size()) - 1;
    if (d < 1) return 0.0;
    double result = 0.0;
    for (int i = d; i >= 1; i--)
        result = result * x + static_cast<double>(i) * c[i];
    return result;
}

}  // namespace

GDResult gradient_descent(const std::vector<double> &coeffs, double x0,
                           double alpha, double tol, long long max_iter) {
    GDResult result;
    double x = x0;
    long long iter = 0;
    bool converged = false;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (; iter < max_iter; iter++) {
        double deriv = poly_deriv_eval(coeffs, x);
        if (std::fabs(deriv) <= tol) {
            converged = true;
            break;
        }
        x = x - alpha * deriv;
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    result.final_x = x;
    result.final_fx = poly_eval(coeffs, x);
    result.iterations = iter;
    result.converged = converged;
    result.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return result;
}

GDResult gradient_descent_averaged(const std::vector<double> &coeffs, double x0,
                                    double alpha, double tol, long long max_iter,
                                    int repeats) {
    GDResult result = gradient_descent(coeffs, x0, alpha, tol, max_iter);
    if (repeats > 1) {
        double total_ms = result.time_ms;
        for (int r = 1; r < repeats; r++)
            total_ms += gradient_descent(coeffs, x0, alpha, tol, max_iter).time_ms;
        result.time_ms = total_ms / repeats;
    }
    return result;
}
