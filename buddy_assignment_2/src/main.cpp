#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "undirected_csr.hpp"
#include "triangle_counting.hpp"
#include "betweenness_centrality.hpp"
#include "connected_components.hpp"

using Clock = std::chrono::steady_clock;

static double elapsed_ms(Clock::time_point t0, Clock::time_point t1) {
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

static void print_time(double ms) {
    std::cout << "Execution time: " << std::fixed << std::setprecision(3)
              << ms << " ms\n";
    std::cout.unsetf(std::ios::fixed);
}

static void print_usage(const char *prog) {
    std::cerr << "Usage: " << prog << " <tc|bc|cc> <input_file>\n"
              << "  tc  Triangle Counting     (unweighted undirected, CSR adjacency-list input)\n"
              << "  bc  Betweenness Centrality (unweighted undirected, CSR adjacency-list input)\n"
              << "  cc  Connected Components  (unweighted undirected, CSR adjacency-list input)\n"
              << "Run with no arguments for the interactive test-case menu.\n";
}

static int run_tc(const std::string &path) {
    UCSRGraph g;
    if (!load_undirected_graph(path, g)) {
        std::cerr << "Error: invalid or missing input file: " << path << "\n";
        return 1;
    }

    // Per Section 7.3: listing individual triangles is required only for the
    // two smallest graph sizes (10 and 100 vertices) and optional above that.
    bool collect_list = (g.V <= 100);
    long long total;
    std::vector<std::array<int, 3>> triangles;

    auto t0 = Clock::now();
    run_triangle_counting(g, total, collect_list, triangles);
    auto t1 = Clock::now();

    std::cout << "Algorithm: Triangle Counting\n";
    std::cout << "Total triangles: " << total << "\n";
    if (collect_list) {
        std::cout << "Triangles found:\n";
        for (auto &t : triangles)
            std::cout << "(" << t[0] << ", " << t[1] << ", " << t[2] << ")\n";
    }
    print_time(elapsed_ms(t0, t1));
    return 0;
}

static int run_bc(const std::string &path) {
    UCSRGraph g;
    if (!load_undirected_graph(path, g)) {
        std::cerr << "Error: invalid or missing input file: " << path << "\n";
        return 1;
    }

    std::vector<double> centrality;

    auto t0 = Clock::now();
    run_betweenness_centrality(g, centrality);
    auto t1 = Clock::now();

    std::cout << "Algorithm: Betweenness Centrality\n";
    std::cout << "Vertex Centrality\n";
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < g.V; i++)
        std::cout << i << " " << centrality[i] << "\n";
    std::cout.unsetf(std::ios::fixed);
    print_time(elapsed_ms(t0, t1));
    return 0;
}

static int run_cc(const std::string &path) {
    UCSRGraph g;
    if (!load_undirected_graph(path, g)) {
        std::cerr << "Error: invalid or missing input file: " << path << "\n";
        return 1;
    }

    std::vector<int> component;
    int num_components;

    auto t0 = Clock::now();
    run_connected_components(g, component, num_components);
    auto t1 = Clock::now();

    std::cout << "Algorithm: Connected Components\n";
    std::cout << "Number of components: " << num_components << "\n";
    std::cout << "Vertex Component\n";
    for (int i = 0; i < g.V; i++)
        std::cout << i << " " << component[i] << "\n";
    print_time(elapsed_ms(t0, t1));
    return 0;
}

// The known test files for each algorithm. Add a filename here to include
// it in that algorithm's submenu -- nothing else needs to change.
static const std::vector<std::string> TC_TESTS = {
    "demo_tc.txt", "tc_10.txt", "tc_100.txt",
    "tc_10000.txt", "tc_50000.txt", "tc_100000.txt",
};
static const std::vector<std::string> BC_TESTS = {
    "demo_bc.txt", "bc_10.txt", "bc_100.txt",
    "bc_1000.txt", "bc_5000.txt", "bc_10000.txt",
};
static const std::vector<std::string> CC_TESTS = {
    "demo_cc.txt", "cc_10.txt", "cc_100.txt",
    "cc_10000.txt", "cc_50000.txt", "cc_100000.txt",
};

static void print_menu() {
    std::cout << "\n===== CS509 Assignment 2 (Buddy Task) =====\n"
              << " 1) Triangle Counting\n"
              << " 2) Betweenness Centrality\n"
              << " 3) Connected Components\n"
              << " 0) Exit\n"
              << "Choice: ";
}

// Shows the test-case list for one algorithm and runs whichever one the
// user picks. Returns to the caller (the main menu) after each run, or
// immediately if the user chooses 0 (Back).
template <typename RunFn>
static void submenu(const std::string &title,
                     const std::vector<std::string> &tests,
                     RunFn run) {
    while (true) {
        std::cout << "\n--- " << title << " Test Cases ---\n";
        for (size_t i = 0; i < tests.size(); i++)
            std::cout << " " << (i + 1) << ") " << tests[i] << "\n";
        std::cout << " 0) Back\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) return;
        if (choice == 0) return;
        if (choice < 1 || static_cast<size_t>(choice) > tests.size()) {
            std::cout << "Unknown option.\n";
            continue;
        }

        std::string path = "tests/" + tests[choice - 1];
        std::cout << "\n--- Running: " << path << " ---\n";
        run(path);
    }
}

static void interactive_menu() {
    while (true) {
        print_menu();
        int choice;
        if (!(std::cin >> choice)) break;

        switch (choice) {
            case 1:
                submenu("Triangle Counting", TC_TESTS, run_tc);
                break;
            case 2:
                submenu("Betweenness Centrality", BC_TESTS, run_bc);
                break;
            case 3:
                submenu("Connected Components", CC_TESTS, run_cc);
                break;
            case 0:
                return;
            default:
                std::cout << "Unknown option.\n";
        }
    }
}

int main(int argc, char **argv) {
    // Direct CLI mode: unchanged, for scripted/graded single runs.
    if (argc == 3) {
        std::string algo = argv[1], path = argv[2];
        if (algo == "tc") return run_tc(path);
        if (algo == "bc") return run_bc(path);
        if (algo == "cc") return run_cc(path);
        std::cerr << "Error: unknown algorithm '" << algo << "'\n";
        print_usage(argv[0]);
        return 1;
    }
    if (argc != 1) {
        print_usage(argv[0]);
        return 1;
    }

    // No arguments: modular interactive menu that cycles through test cases.
    interactive_menu();
    return 0;
}
