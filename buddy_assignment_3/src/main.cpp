#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "csr.hpp"
#include "gd.hpp"
#include "gd_io.hpp"
#include "maxflow.hpp"
#include "maxflow_io.hpp"

namespace fs = std::filesystem;

static const char *GD_DIR = "tests/gd";
static const char *MAXFLOW_DIR = "tests/maxflow";
static const int GD_REPEATS = 1000;   // averaging repeats for fast GD runs

// ---------- helpers ----------

static std::vector<std::string> list_txt_files(const std::string &dir) {
    std::vector<std::string> files;
    if (fs::exists(dir))
        for (const auto &entry : fs::directory_iterator(dir))
            if (entry.path().extension() == ".txt") files.push_back(entry.path().string());
    std::sort(files.begin(), files.end());
    return files;
}

// ---------- Gradient Descent ----------

static bool run_gd_on_file(const std::string &path, GDInput &in, GDResult &r) {
    if (!load_gd_input(path, in)) return false;   // untimed setup
    r = gradient_descent_averaged(in.coeffs, in.x0, in.alpha, in.tol, in.max_iter, GD_REPEATS);
    return true;
}

static void print_gd_result(const GDInput &in, const GDResult &r) {
    std::cout << "Algorithm: Gradient Descent\n";
    std::cout << "Degree: " << in.degree << "\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Final x: approximately " << r.final_x << "\n";
    std::cout << "Final f(x): approximately " << r.final_fx << "\n";
    std::cout << "Iterations: " << r.iterations << "\n";
    std::cout << "Converged: " << (r.converged ? "true" : "false") << "\n";
    std::cout << std::setprecision(3);
    std::cout << "Execution time: " << r.time_ms << " ms (avg of " << GD_REPEATS << " runs)\n";
}

static void run_gd_and_print(const std::string &path) {
    GDInput in;
    GDResult r;
    if (!run_gd_on_file(path, in, r)) return;
    print_gd_result(in, r);
}

static void menu_gd() {
    auto files = list_txt_files(GD_DIR);
    if (files.empty()) {
        std::cout << "No .txt test files found in " << GD_DIR << "\n";
        return;
    }
    while (true) {
        std::cout << "\n--- Gradient Descent Test Cases ---\n";
        for (size_t i = 0; i < files.size(); i++)
            std::cout << " " << (i + 1) << ") " << fs::path(files[i]).filename().string() << "\n";
        std::cout << " 0) Back\nChoice: ";
        int choice;
        if (!(std::cin >> choice)) return;
        if (choice == 0) return;
        if (choice < 1 || choice > static_cast<int>(files.size())) {
            std::cout << "Invalid choice.\n";
            continue;
        }
        std::cout << "\n";
        run_gd_and_print(files[choice - 1]);
    }
}

static void report_gd() {
    auto files = list_txt_files(GD_DIR);
    std::ofstream out("report_gd.md");
    out << "# Gradient Descent Results\n\n";
    out << "| File | Degree | x0 | Rate | Tol | Max Iter | Actual Iter | Final x | "
           "Final f(x) | Converged | Time (ms, avg of " << GD_REPEATS << ") | Status |\n";
    out << "|---|---|---|---|---|---|---|---|---|---|---|---|\n";

    for (const auto &path : files) {
        GDInput in;
        GDResult r;
        if (!run_gd_on_file(path, in, r)) continue;

        out << "| " << fs::path(path).filename().string()
            << " | " << in.degree << " | " << in.x0 << " | " << in.alpha
            << " | " << std::scientific << std::setprecision(1) << in.tol << std::defaultfloat
            << " | " << in.max_iter << " | " << r.iterations
            << " | " << std::fixed << std::setprecision(6) << r.final_x
            << " | " << r.final_fx
            << " | " << (r.converged ? "true" : "false")
            << " | " << std::setprecision(3) << r.time_ms
            << " | " << (r.converged ? "Pass" : "Fail") << " |\n";

        std::cout << fs::path(path).filename().string()
                   << ": degree=" << in.degree
                   << " final_x=" << std::fixed << std::setprecision(6) << r.final_x
                   << " iters=" << r.iterations
                   << " converged=" << (r.converged ? "true" : "false")
                   << " time=" << std::setprecision(3) << r.time_ms << "ms\n";
    }
    std::cout << "\nWrote report_gd.md\n";
}

// ---------- Maxflow-Mincut ----------

static bool run_maxflow_on_file(const std::string &path, int &V, int &E, int &source, int &sink,
                                 MaxflowResult &r) {
    CSRGraph g;
    if (!load_maxflow_graph(path, g, source, sink)) return false;   // untimed setup
    V = g.V;
    E = g.E;   // directed edge count (each listed edge counted once)
    r = maxflow_mincut(g, source, sink);
    return true;
}

static void print_maxflow_result(int source, int sink, const MaxflowResult &r) {
    std::cout << "Algorithm: Maxflow-Mincut\n";
    std::cout << "Source: " << source << "\n";
    std::cout << "Sink: " << sink << "\n";
    std::cout << "Maximum flow: " << r.max_flow_value << "\n";
    std::cout << "Minimum cut capacity: " << r.min_cut_capacity << "\n";
    std::cout << "Source side:";
    for (int v : r.source_side) std::cout << " " << v;
    std::cout << "\n";
    std::cout << "Sink side:";
    for (int v : r.sink_side) std::cout << " " << v;
    std::cout << "\n";
    std::cout << "Cut edges:\n";
    for (const auto &e : r.cut_edges) std::cout << e.u << " " << e.v << " " << e.cap << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Execution time: " << r.time_ms << " ms\n";
}

static void run_maxflow_and_print(const std::string &path) {
    int V, E, source, sink;
    MaxflowResult r;
    if (!run_maxflow_on_file(path, V, E, source, sink, r)) return;
    print_maxflow_result(source, sink, r);
}

static void menu_maxflow() {
    auto files = list_txt_files(MAXFLOW_DIR);
    if (files.empty()) {
        std::cout << "No .txt test files found in " << MAXFLOW_DIR << "\n";
        return;
    }
    while (true) {
        std::cout << "\n--- Maxflow-Mincut Test Cases ---\n";
        for (size_t i = 0; i < files.size(); i++)
            std::cout << " " << (i + 1) << ") " << fs::path(files[i]).filename().string() << "\n";
        std::cout << " 0) Back\nChoice: ";
        int choice;
        if (!(std::cin >> choice)) return;
        if (choice == 0) return;
        if (choice < 1 || choice > static_cast<int>(files.size())) {
            std::cout << "Invalid choice.\n";
            continue;
        }
        std::cout << "\n";
        run_maxflow_and_print(files[choice - 1]);
    }
}

static void report_maxflow() {
    auto files = list_txt_files(MAXFLOW_DIR);
    std::ofstream out("report_maxflow.md");
    out << "# Maxflow-Mincut Results\n\n";
    out << "| File | V | E | Source | Sink | Max Flow | Min Cut Capacity | "
           "Equal? | Time (ms) | Status |\n";
    out << "|---|---|---|---|---|---|---|---|---|---|\n";

    for (const auto &path : files) {
        int V, E, source, sink;
        MaxflowResult r;
        if (!run_maxflow_on_file(path, V, E, source, sink, r)) continue;

        bool equal = r.max_flow_value == r.min_cut_capacity;
        out << "| " << fs::path(path).filename().string()
            << " | " << V << " | " << E
            << " | " << source << " | " << sink
            << " | " << r.max_flow_value << " | " << r.min_cut_capacity
            << " | " << (equal ? "Yes" : "No")
            << " | " << std::fixed << std::setprecision(3) << r.time_ms
            << " | " << (equal ? "Pass" : "Fail") << " |\n";

        std::cout << fs::path(path).filename().string()
                   << ": V=" << V << " E=" << E << " source=" << source << " sink=" << sink
                   << " max_flow=" << r.max_flow_value
                   << " min_cut=" << r.min_cut_capacity
                   << " equal=" << (equal ? "Yes" : "No")
                   << " time=" << std::fixed << std::setprecision(3) << r.time_ms << "ms\n";
    }
    std::cout << "\nWrote report_maxflow.md\n";
}

// ---------- main ----------

int main(int argc, char **argv) {
    if (argc == 3 && std::string(argv[1]) == "gd") {
        run_gd_and_print(argv[2]);
        return 0;
    }
    if (argc == 3 && std::string(argv[1]) == "maxflow") {
        run_maxflow_and_print(argv[2]);
        return 0;
    }
    if (argc == 2 && std::string(argv[1]) == "report") {
        report_gd();
        report_maxflow();
        return 0;
    }

    while (true) {
        std::cout << "\n===== CS509 Assignment 3 (Buddy) =====\n";
        std::cout << " 1) Gradient Descent\n";
        std::cout << " 2) Maxflow-Mincut\n";
        std::cout << " 3) Generate results tables (report_gd.md, report_maxflow.md)\n";
        std::cout << " 0) Exit\n";
        std::cout << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) break;
        if (choice == 0) break;
        else if (choice == 1) menu_gd();
        else if (choice == 2) menu_maxflow();
        else if (choice == 3) { report_gd(); report_maxflow(); }
        else std::cout << "Invalid choice.\n";
    }
    return 0;
}
