#include "fastmap.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Clock = std::chrono::steady_clock;

namespace {

struct FastMapInput {
    int N = 0;
    int K = 0;
    std::vector<std::vector<double>> dist;
};

static bool load_fastmap_data(const std::string &path, FastMapInput &data) {
    std::ifstream in(path);
    if (!in) return false;

    int N = 0, K = 0;
    if (!(in >> N >> K)) return false;
    if (N <= 0 || K <= 0 || K >= N) return false;

    data.N = N;
    data.K = K;
    data.dist.assign(N, std::vector<double>(N, 0.0));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!(in >> data.dist[i][j])) return false;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j && std::abs(data.dist[i][j]) > 1e-12) return false;
            if (i != j && data.dist[i][j] < 0.0) return false;
            if (i != j && std::abs(data.dist[i][j] - data.dist[j][i]) > 1e-9) return false;
        }
    }

    return true;
}

static double elapsed_ms(Clock::time_point t0, Clock::time_point t1) {
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

}  // namespace

int run_fastmap(const std::string &path) {
    FastMapInput input;
    if (!load_fastmap_data(path, input)) {
        std::cerr << "Error: invalid or missing FastMap input file: " << path << "\n";
        return 1;
    }

    std::vector<std::vector<double>> coords(input.N, std::vector<double>(input.K, 0.0));
    std::vector<std::pair<int, int>> pivots(input.K, {0, 1});

    auto t0 = Clock::now();
    std::vector<std::vector<double>> dist = input.dist;

    for (int dim = 0; dim < input.K; ++dim) {
        int a = 0;
        double max_dist = -1.0;
        for (int i = 0; i < input.N; ++i) {
            if (dist[a][i] > max_dist) {
                max_dist = dist[a][i];
                a = i;
            }
        }

        int b = a;
        max_dist = -1.0;
        for (int i = 0; i < input.N; ++i) {
            if (dist[a][i] > max_dist) {
                max_dist = dist[a][i];
                b = i;
            }
        }

        const double c = dist[a][b];
        pivots[dim] = {a, b};
        if (c <= 1e-12) {
            for (int i = 0; i < input.N; ++i) {
                coords[i][dim] = 0.0;
            }
            continue;
        }

        for (int i = 0; i < input.N; ++i) {
            const double da = dist[a][i];
            const double db = dist[b][i];
            coords[i][dim] = (da * da + c * c - db * db) / (2.0 * c);
        }

        for (int i = 0; i < input.N; ++i) {
            for (int j = i + 1; j < input.N; ++j) {
                const double dx = coords[i][dim] - coords[j][dim];
                const double old_sq = dist[i][j] * dist[i][j];
                const double new_sq = std::max(0.0, old_sq - dx * dx);
                dist[i][j] = dist[j][i] = std::sqrt(new_sq);
            }
        }
    }
    auto t1 = Clock::now();

    std::cout << "Algorithm: FastMap\n";
    std::cout << "Target dimensions: " << input.K << "\n";
    std::cout << "Pivots per dimension:\n";
    for (int dim = 0; dim < input.K; ++dim) {
        std::cout << "Dim " << (dim + 1) << ": " << pivots[dim].first << " " << pivots[dim].second << "\n";
    }
    std::cout << "Object coordinates:\n";
    for (int i = 0; i < input.N; ++i) {
        std::cout << i << ":";
        for (int dim = 0; dim < input.K; ++dim) {
            std::cout << " " << std::fixed << std::setprecision(6) << coords[i][dim];
        }
        std::cout << "\n";
    }
    std::cout << "Execution time: " << std::fixed << std::setprecision(3)
              << elapsed_ms(t0, t1) << " ms\n";
    return 0;
}
