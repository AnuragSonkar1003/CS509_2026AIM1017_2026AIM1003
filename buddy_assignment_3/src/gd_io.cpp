#include "gd_io.hpp"
#include <fstream>
#include <iostream>

bool load_gd_input(const std::string &path, GDInput &in) {
    std::ifstream fin(path);
    if (!fin) {
        std::cerr << "Error: cannot open input file: " << path << "\n";
        return false;
    }

    std::string tok;

    if (!(fin >> tok) || tok != "DEGREE" || !(fin >> in.degree)) {
        std::cerr << "Error: expected DEGREE line in " << path << "\n";
        return false;
    }
    if (in.degree < 0) {
        std::cerr << "Error: invalid degree " << in.degree << "\n";
        return false;
    }

    if (!(fin >> tok) || tok != "COEFFICIENTS") {
        std::cerr << "Error: expected COEFFICIENTS line in " << path << "\n";
        return false;
    }
    in.coeffs.resize(in.degree + 1);
    for (int i = 0; i <= in.degree; i++) {
        if (!(fin >> in.coeffs[i])) {
            std::cerr << "Error: expected " << (in.degree + 1)
                      << " coefficients (degree+1) in " << path << "\n";
            return false;
        }
    }

    if (!(fin >> tok) || tok != "INITIAL_X" || !(fin >> in.x0)) {
        std::cerr << "Error: expected INITIAL_X line in " << path << "\n";
        return false;
    }
    if (!(fin >> tok) || tok != "LEARNING_RATE" || !(fin >> in.alpha)) {
        std::cerr << "Error: expected LEARNING_RATE line in " << path << "\n";
        return false;
    }
    if (in.alpha <= 0.0) {
        std::cerr << "Error: learning rate must be positive\n";
        return false;
    }
    if (!(fin >> tok) || tok != "TOLERANCE" || !(fin >> in.tol)) {
        std::cerr << "Error: expected TOLERANCE line in " << path << "\n";
        return false;
    }
    if (in.tol <= 0.0) {
        std::cerr << "Error: tolerance must be positive\n";
        return false;
    }
    if (!(fin >> tok) || tok != "MAX_ITERATIONS" || !(fin >> in.max_iter)) {
        std::cerr << "Error: expected MAX_ITERATIONS line in " << path << "\n";
        return false;
    }
    if (in.max_iter <= 0) {
        std::cerr << "Error: MAX_ITERATIONS must be positive\n";
        return false;
    }

    return true;
}
