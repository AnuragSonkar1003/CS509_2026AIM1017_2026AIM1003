#include "kmeans.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;

namespace {

struct KMeansInput {
    int N = 0;
    int D = 0;
    int K = 0;
    int max_iter = 300;
    double tolerance = 1e-4;
    std::vector<std::vector<double>> points;
};

static bool load_kmeans_data(const std::string &path, KMeansInput &data) {
    std::ifstream in(path);
    if (!in) return false;

    int N = 0, D = 0, K = 0;
    if (!(in >> N >> D >> K)) return false;
    if (N <= 0 || D <= 0 || K <= 0 || K > N) return false;

    data.N = N;
    data.D = D;
    data.K = K;
    data.points.assign(N, std::vector<double>(D, 0.0));

    for (int i = 0; i < N; ++i) {
        for (int d = 0; d < D; ++d) {
            if (!(in >> data.points[i][d])) return false;
        }
    }

    std::string token;
    while (in >> token) {
        if (token == "MAX_ITERATIONS") {
            if (!(in >> data.max_iter)) return false;
        } else if (token == "TOLERANCE") {
            if (!(in >> data.tolerance)) return false;
        }
    }

    if (data.max_iter <= 0 || data.tolerance <= 0.0) return false;
    return true;
}

static double euclidean_sq(const std::vector<double> &a, const std::vector<double> &b) {
    double total = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        const double diff = a[i] - b[i];
        total += diff * diff;
    }
    return total;
}

static double elapsed_ms(Clock::time_point t0, Clock::time_point t1) {
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

}  // namespace

int run_kmeans(const std::string &path) {
    KMeansInput data;
    if (!load_kmeans_data(path, data)) {
        std::cerr << "Error: invalid or missing K-Means input file: " << path << "\n";
        return 1;
    }

    std::vector<std::vector<double>> centroids(data.K, std::vector<double>(data.D, 0.0));
    for (int k = 0; k < data.K; ++k) {
        centroids[k] = data.points[k];
    }

    std::vector<int> assignments(data.N, 0);
    bool converged = false;
    int actual_iterations = 0;

    auto t0 = Clock::now();
    for (int iter = 1; iter <= data.max_iter; ++iter) {
        std::vector<int> next_assignments(data.N, 0);
        for (int i = 0; i < data.N; ++i) {
            int best_k = 0;
            double best_dist = std::numeric_limits<double>::infinity();
            for (int k = 0; k < data.K; ++k) {
                const double dist = euclidean_sq(data.points[i], centroids[k]);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_k = k;
                }
            }
            next_assignments[i] = best_k;
        }

        std::vector<std::vector<double>> next_centroids(data.K, std::vector<double>(data.D, 0.0));
        std::vector<int> counts(data.K, 0);
        for (int i = 0; i < data.N; ++i) {
            const int cluster = next_assignments[i];
            ++counts[cluster];
            for (int d = 0; d < data.D; ++d) {
                next_centroids[cluster][d] += data.points[i][d];
            }
        }

        double max_shift = 0.0;
        for (int k = 0; k < data.K; ++k) {
            if (counts[k] > 0) {
                for (int d = 0; d < data.D; ++d) {
                    next_centroids[k][d] /= static_cast<double>(counts[k]);
                }
            } else {
                next_centroids[k] = centroids[k];
            }

            for (int d = 0; d < data.D; ++d) {
                max_shift = std::max(max_shift, std::abs(next_centroids[k][d] - centroids[k][d]));
            }
        }

        assignments = next_assignments;
        centroids = next_centroids;
        actual_iterations = iter;

        if (max_shift <= data.tolerance) {
            converged = true;
            break;
        }
    }
    auto t1 = Clock::now();

    double wcss = 0.0;
    for (int i = 0; i < data.N; ++i) {
        wcss += euclidean_sq(data.points[i], centroids[assignments[i]]);
    }

    std::cout << "Algorithm: K-Means Clustering\n";
    std::cout << "K: " << data.K << "\n";
    std::cout << "Point assignments:";
    for (int i = 0; i < data.N; ++i) {
        std::cout << " " << assignments[i];
    }
    std::cout << "\n";
    std::cout << "Final centroids:\n";
    for (int k = 0; k < data.K; ++k) {
        std::cout << k << ":";
        for (int d = 0; d < data.D; ++d) {
            std::cout << " " << std::fixed << std::setprecision(6) << centroids[k][d];
        }
        std::cout << "\n";
    }
    std::cout << "WCSS: " << std::fixed << std::setprecision(6) << wcss << "\n";
    std::cout << "Iterations: " << actual_iterations << "\n";
    std::cout << "Converged: " << (converged ? "true" : "false") << "\n";
    std::cout << "Execution time: " << std::fixed << std::setprecision(3)
              << elapsed_ms(t0, t1) << " ms\n";
    return 0;
}
